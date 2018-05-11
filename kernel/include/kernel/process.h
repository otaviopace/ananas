#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <ananas/limits.h>
#include <ananas/util/list.h>
#include "kernel/lock.h"

struct DEntry;
struct PROCINFO;
class Result;

struct Thread;
class VMSpace;

#define PROCESS_STATE_ACTIVE	1
#define PROCESS_STATE_ZOMBIE	2

struct Process;

namespace process {

// Internal stuff so we can work with children and all nodes
namespace internal {

template<typename T>
struct AllNode
{
	static typename util::List<T>::Node& Get(T& t) {
		return t.p_NodeAll;
	}
};

template<typename T>
struct ChildrenNode
{
	static typename util::List<T>::Node& Get(T& t) {
		return t.p_NodeChildren;
	}
};

template<typename T>
struct GroupNode
{
	static typename util::List<T>::Node& Get(T& t) {
		return t.p_NodeGroup;
	}
};

template<typename T> using ProcessAllNodeAccessor = typename util::List<T>::template nodeptr_accessor<AllNode<T> >;
template<typename T> using ProcessChildrenNodeAccessor = typename util::List<T>::template nodeptr_accessor<ChildrenNode<T> >;
template<typename T> using ProcessGroupNodeAccessor = typename util::List<T>::template nodeptr_accessor<GroupNode<T> >;

} // namespace internal

typedef util::List<::Process, internal::ProcessAllNodeAccessor<::Process> > ChildrenList;
typedef util::List<::Process, internal::ProcessChildrenNodeAccessor<::Process> > ProcessList;
typedef util::List<::Process, internal::ProcessGroupNodeAccessor<::Process> > GroupMemberList;

// Callbacks so we can organise things when needed
typedef Result (*process_callback_t)(Process& proc);
struct Callback : util::List<Callback>::NodePtr {
	Callback(process_callback_t func) : pc_func(func) { }
	process_callback_t pc_func;
};
typedef util::List<Callback> CallbackList;

struct ProcessGroup;

} // namespace process

struct Process {
	void Lock()
	{
		p_lock.Lock();
	}

	void Unlock()
	{
		p_lock.Unlock();
	}

	unsigned int p_state = 0;	/* Process state */
	refcount_t p_refcount = 0;		/* Reference count of the process, >0 */

	pid_t	p_pid = 0;/* Process ID */
	int	p_exit_status = 0;	/* Exit status / code */

	Process* 	p_parent = nullptr;	/* Parent process, if any */
	VMSpace*	p_vmspace = nullptr;	/* Process memory space */

	struct PROCINFO* p_info = nullptr; 	/* Process startup information */
	addr_t p_info_va = 0;

	Thread* p_mainthread = nullptr;		/* Main thread */

	struct HANDLE* p_handle[PROCESS_MAX_HANDLES] = {0};	/* Handles */

	DEntry*	p_cwd = nullptr;		/* Current path */

	process::ChildrenList	p_children;	// List of this process' children

	process::ProcessGroup*	p_group = nullptr;

	// Node pointers to maintain membership of several lists
	util::List<Process>::Node p_NodeAll;
	util::List<Process>::Node p_NodeChildren;
	util::List<Process>::Node p_NodeGroup;

	void Ref();
	void Deref();
	void Exit(int status);

	Result SetArguments(const char* args, size_t args_len);
	Result SetEnvironment(const char* env, size_t env_len);
	Result Clone(int flags, Process*& out_p);

	Result WaitAndLock(int flags, Process*& p_out);

private:
	Mutex p_lock{"plock"};	/* Locks the process */
	Semaphore p_child_wait{0};
};

Result process_alloc(Process* parent, Process*& dest);

Process* process_lookup_by_id_and_ref(pid_t pid);

/*
 * Process callback functions are provided so that modules can take action upon
 * creating or destroying of processes.
 */

Result process_register_init_func(process::Callback& pc);
Result process_register_exit_func(process::Callback& pc);
Result process_unregister_init_func(process::Callback& pc);
Result process_unregister_exit_func(process::Callback& pc);

#define REGISTER_PROCESS_INIT_FUNC(fn) \
	static process::Callback cb_init_##fn(fn); \
	static Result register_##fn() { \
		return process_register_init_func(cb_init_##fn); \
	} \
	static Result unregister_##fn() { \
		return process_unregister_init_func(cb_init_##fn); \
	} \
	INIT_FUNCTION(register_##fn, SUBSYSTEM_THREAD, ORDER_FIRST); \
	EXIT_FUNCTION(unregister_##fn)

#define REGISTER_PROCESS_EXIT_FUNC(fn) \
	static process::Callback cb_exit_##fn(fn); \
	static Result register_##fn() { \
		return process_register_exit_func(cb_exit_##fn); \
	} \
	static Result unregister_##fn() { \
		return process_unregister_exit_func(cb_exit_##fn); \
	} \
	INIT_FUNCTION(register_##fn, SUBSYSTEM_THREAD, ORDER_FIRST); \
	EXIT_FUNCTION(unregister_##fn)

#endif /* __PROCESS_H__ */
