#include <ananas/types.h>
#include <ananas/errno.h>
#include <ananas/syscalls.h>
#include <ananas/syscall-vmops.h>
#include "kernel/process.h"
#include "kernel/result.h"
#include "kernel/thread.h"
#include "kernel/trace.h"
#include "kernel/vm.h"
#include "kernel/vmspace.h"
#include "kernel/vfs/dentry.h"
#include "syscall.h"

TRACE_SETUP;

static Result
sys_vmop_map(Thread* curthread, struct VMOP_OPTIONS* vo)
{
	if (vo->vo_len == 0)
		return RESULT_MAKE_FAILURE(EINVAL);
	if ((vo->vo_flags & (VMOP_FLAG_PRIVATE | VMOP_FLAG_SHARED)) == 0)
		return RESULT_MAKE_FAILURE(EINVAL);

	int vm_flags = VM_FLAG_USER | VM_FLAG_FAULT;
	if (vo->vo_flags & VMOP_FLAG_READ)
		vm_flags |= VM_FLAG_READ;
	if (vo->vo_flags & VMOP_FLAG_WRITE)
		vm_flags |= VM_FLAG_WRITE;
	if (vo->vo_flags & VMOP_FLAG_EXECUTE)
		vm_flags |= VM_FLAG_EXECUTE;
	if (vo->vo_flags & VMOP_FLAG_PRIVATE)
		vm_flags |= VM_FLAG_PRIVATE;

	VMSpace& vs = *curthread->t_process->p_vmspace;
	addr_t dest_addr = reinterpret_cast<addr_t>(vo->vo_addr);
	if ((vo->vo_flags & VMOP_FLAG_FIXED) == 0)
		dest_addr = vmspace_determine_va(vs, vo->vo_len);

	VMArea* va;
	if (vo->vo_flags & VMOP_FLAG_HANDLE) {
		struct HANDLE* h;
		RESULT_PROPAGATE_FAILURE(
			syscall_get_handle(*curthread, vo->vo_handle, &h)
		);

		if (h->h_type != HANDLE_TYPE_FILE)
			return RESULT_MAKE_FAILURE(EBADF);
		DEntry* dentry = h->h_data.d_vfs_file.f_dentry;
		if (dentry == nullptr)
			return RESULT_MAKE_FAILURE(EBADF);

		RESULT_PROPAGATE_FAILURE(
			vs.MapToDentry(dest_addr, vo->vo_len, *dentry, vo->vo_offset, vo->vo_len, vm_flags, va)
		);
	} else {
		RESULT_PROPAGATE_FAILURE(
			vs.MapTo(dest_addr, vo->vo_len, vm_flags, va)
		);
	}

	vo->vo_addr = (void*)va->va_virt;
	vo->vo_len = va->va_len;
	return Result::Success();
}

static Result
sys_vmop_unmap(ARG_CURTHREAD struct VMOP_OPTIONS* vo)
{
	/* XXX implement me */
	return RESULT_MAKE_FAILURE(EINVAL);
}

Result
sys_vmop(ARG_CURTHREAD struct VMOP_OPTIONS* opts)
{
	TRACE(SYSCALL, FUNC, "t=%p, opts=%p", curthread, opts);

	/* Obtain options */
	struct VMOP_OPTIONS* vmop_opts;
	RESULT_PROPAGATE_FAILURE(
		syscall_map_buffer(*curthread, opts, sizeof(*vmop_opts), VM_FLAG_READ | VM_FLAG_WRITE, (void**)&vmop_opts)
	);
	if (vmop_opts->vo_size != sizeof(*vmop_opts))
		return RESULT_MAKE_FAILURE(EINVAL);

	switch(vmop_opts->vo_op) {
		case OP_MAP:
			return sys_vmop_map(curthread, vmop_opts);
		case OP_UNMAP:
			return sys_vmop_unmap(curthread, vmop_opts);
		default:
			return RESULT_MAKE_FAILURE(EINVAL);
	}

	/* NOTREACHED */
}
