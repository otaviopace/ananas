/*-
 * SPDX-License-Identifier: Zlib
 *
 * Copyright (c) 2009-2018 Rink Springer <rink@rink.nu>
 * For conditions of distribution and use, see LICENSE file
 */
#include <ananas/types.h>
#include <ananas/errno.h>
#include "kernel/result.h"
#include "kernel/trace.h"

TRACE_SETUP;

struct Thread;

Result sys_link(Thread* t, const char* oldpath, const char* newpath)
{
    TRACE(SYSCALL, FUNC, "t=%p, oldpath='%s' newpath='%s'", t, oldpath, newpath);

    return RESULT_MAKE_FAILURE(EPERM); // until we implement this
}

Result sys_symlink(Thread* t, const char* oldpath, const char* newpath)
{
    TRACE(SYSCALL, FUNC, "t=%p, oldpath='%s' newpath='%s'", t, oldpath, newpath);

    return RESULT_MAKE_FAILURE(EPERM); // until we implement this
}
