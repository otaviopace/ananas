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

Result sys_unlink(Thread* t, const char* path)
{
    TRACE(SYSCALL, FUNC, "t=%p, path='%s'", t, path);

    /* TODO */
    return RESULT_MAKE_FAILURE(EIO);
}
