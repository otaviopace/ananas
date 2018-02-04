#ifndef __SYS_HANDLE_H__
#define __SYS_HANDLE_H__

#include <ananas/util/list.h>
#include "kernel/cbuffer.h"
#include "kernel/lock.h"
#include "kernel/vfs/types.h"

typedef unsigned int handle_event_t;
typedef unsigned int handle_event_result_t;

#define HANDLE_TYPE_ANY		-1
#define HANDLE_TYPE_UNUSED	0
#define HANDLE_TYPE_FILE	1
#define HANDLE_TYPE_PIPE	2

#define HANDLE_VALUE_INVALID	0

struct Process;
struct Thread;
struct HANDLE_OPS;
class Result;

struct HANDLE_PIPE_BUFFER {
	Mutex hpb_mutex;
	refcount_t hpb_read_count;
	refcount_t hpb_write_count;
	CBUFFER_FIELDS;
	char hpb_buffer[1];
};

struct HANDLE_PIPE_INFO {
	int hpi_flags;
#define HPI_FLAG_READ	0x0001
#define HPI_FLAG_WRITE	0x0002
	struct HANDLE_PIPE_BUFFER* hpi_buffer;
	/*
	 * All HANDLE_PIPE_INFO structs that belong to the same buffer are
	 * in a circular linked list - this is done so that they can be
	 * woken up once any of them is written/read/closed. This also
	 * means we need the handle itself. XXX KLUDGE
	 */
	struct HANDLE_PIPE_INFO* hpi_next;
	struct HANDLE* hpi_handle;
};

struct HANDLE : util::List<HANDLE>::NodePtr {
	int h_type = 0;				/* one of HANDLE_TYPE_... */
	int h_flags = 0;			/* flags */
	Process* h_process = nullptr;		/* owning process */
	Mutex h_mutex{"handle"};		/* mutex guarding the handle */
	struct HANDLE_OPS* h_hops = nullptr;	/* handle operations */

	/* Waiters are those who are waiting on this handle */
	union {
		struct VFS_FILE d_vfs_file;
		struct HANDLE_PIPE_INFO d_pipe;
	} h_data{};
};

typedef util::List<HANDLE> HandleList;

/*
 * Handle operations map almost directly to the syscalls invoked on them.
 */
struct OPEN_OPTIONS;
struct CREATE_OPTIONS;
struct SUMMON_OPTIONS;
struct CLONE_OPTIONS;
typedef Result (*handle_read_fn)(Thread* thread, handleindex_t index, struct HANDLE* handle, void* buf, size_t* len);
typedef Result (*handle_write_fn)(Thread* thread, handleindex_t index, struct HANDLE* handle, const void* buf, size_t* len);
typedef Result (*handle_open_fn)(Thread* thread, handleindex_t index, struct HANDLE* result, const char* path, int flags, int mode);
typedef Result (*handle_free_fn)(Process& proc, struct HANDLE* handle);
typedef Result (*handle_unlink_fn)(Thread* thread, handleindex_t index, struct HANDLE* handle);
typedef Result (*handle_clone_fn)(Process& proc_in, handleindex_t index, struct HANDLE* handle, struct CLONE_OPTIONS* opts, Process& proc_out, struct HANDLE** handle_out, handleindex_t index_out_min, handleindex_t* index_out);

struct HANDLE_OPS {
	handle_read_fn hop_read;
	handle_write_fn hop_write;
	handle_open_fn hop_open;
	handle_free_fn hop_free;
	handle_unlink_fn hop_unlink;
	handle_clone_fn hop_clone;
};

/* Registration of handle types */
struct HandleType : util::List<HandleType>::NodePtr {
	HandleType(const char* name, int id, struct HANDLE_OPS* hops)
	 : ht_name(name), ht_id(id), ht_hops(hops)
	{
	}
	const char* ht_name;
	int ht_id;
	struct HANDLE_OPS* ht_hops;
};
typedef util::List<HandleType> HandleTypeList;

void handle_init();
Result handle_alloc(int type, Process& p, handleindex_t index_from, struct HANDLE** handle_out, handleindex_t* index_out);
Result handle_free(struct HANDLE* h);
Result handle_free_byindex(Process& p, handleindex_t index);
Result handle_lookup(Process& p, handleindex_t index, int type, struct HANDLE** handle_out);
Result handle_clone(Process& p_in, handleindex_t index, struct CLONE_OPTIONS* opts, Process& p_out, struct HANDLE** handle_out, handleindex_t index_out_min, handleindex_t* index_out);

/* Only to be used from handle implementation code */
Result handle_clone_generic(struct HANDLE* handle, Process& p_out, struct HANDLE** out, handleindex_t index_out_min, handleindex_t* index);
Result handle_register_type(HandleType& ht);
Result handle_unregister_type(HandleType& ht);

#define HANDLE_TYPE(id, name, ops) \
	static HandleType ht_##id(name, id, &ops); \
	static Result register_##id() { \
		return handle_register_type(ht_##id); \
	}; \
	static Result unregister_##id() { \
		return handle_unregister_type(ht_##id); \
	}; \
	INIT_FUNCTION(register_##id, SUBSYSTEM_HANDLE, ORDER_SECOND); \
	EXIT_FUNCTION(unregister_##id)

#endif /* __SYS_HANDLE_H__ */
