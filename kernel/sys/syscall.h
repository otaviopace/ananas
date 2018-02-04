#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include <ananas/types.h>

struct SYSCALL_ARGS {
	register_t	number;
	register_t	arg1, arg2, arg3, arg4, arg5;
};

class Result;
struct HANDLE;
struct VFS_FILE;
struct Thread;

register_t syscall(struct SYSCALL_ARGS* args);

Result syscall_get_handle(Thread& t, handleindex_t handle, struct HANDLE** out);
Result syscall_get_file(Thread& t, handleindex_t handle, struct VFS_FILE** out);
Result syscall_map_string(Thread& t, const void* ptr, const char** out);
Result syscall_map_buffer(Thread& t, const void* ptr, size_t len, int flags, void** out);
Result syscall_fetch_size(Thread& t, const void* ptr, size_t* out);
Result syscall_set_size(Thread& t, void* ptr, size_t len);
Result syscall_set_handleindex(Thread& t, handleindex_t* ptr, handleindex_t index);
Result syscall_fetch_offset(Thread& t, const void* ptr, off_t* out);
Result syscall_set_offset(Thread& t, void* ptr, off_t len);

#endif /* __SYSCALL_H__ */
