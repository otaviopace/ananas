/*-
 * SPDX-License-Identifier: Zlib
 *
 * Copyright (c) 2009-2018 Rink Springer <rink@rink.nu>
 * For conditions of distribution and use, see LICENSE file
 */
#include <ananas/types.h>
#include <ananas/errno.h>
#include "kernel/fd.h"
#include "kernel/process.h"
#include "kernel/result.h"
#include "kernel/thread.h"
#include "kernel/trace.h"
#include "kernel/vfs/core.h"
#include "kernel/vm.h"
#include "syscall.h"

TRACE_SETUP;

Result sys_readlink(Thread* t, const char* path, char* buf, size_t buflen)
{
    TRACE(SYSCALL, FUNC, "t=%p, path='%p'", t, path);
    Process& proc = *t->t_process;
    DEntry* cwd = proc.p_cwd;

    // Attempt to map the buffer write-only
    void* buffer;
    if (auto result = syscall_map_buffer(*t, buf, buflen, VM_FLAG_WRITE, &buffer);
        result.IsFailure())
        return result;

    // Open the link
    struct VFS_FILE file;
    if (auto result = vfs_open(&proc, path, cwd, &file, VFS_LOOKUP_FLAG_NO_FOLLOW);
        result.IsFailure())
        return result;

    // And copy the contents
    auto result = vfs_read(&file, buf, buflen);
    vfs_close(&proc, &file);
    if (result.IsFailure())
        return result;

    TRACE(SYSCALL, FUNC, "t=%p, success: size=%u", t, result.AsStatusCode());
    return result;
}
