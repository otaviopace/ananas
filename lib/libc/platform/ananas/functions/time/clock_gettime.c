/*-
 * SPDX-License-Identifier: Zlib
 *
 * Copyright (c) 2009-2018 Rink Springer <rink@rink.nu>
 * For conditions of distribution and use, see LICENSE file
 */
#include <ananas/types.h>
#include <ananas/syscalls.h>
#include <time.h>
#include "_map_statuscode.h"

int clock_gettime(clockid_t id, struct timespec* ts)
{
    statuscode_t status = sys_clock_gettime(id, ts);
    return map_statuscode(status);
}
