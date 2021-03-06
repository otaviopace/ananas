/*-
 * SPDX-License-Identifier: Zlib
 *
 * Copyright (c) 2009-2018 Rink Springer <rink@rink.nu>
 * For conditions of distribution and use, see LICENSE file
 */
#include <sched.h>
#include <errno.h>

int sched_setscheduler(pid_t pid, int policy, const struct sched_param* param)
{
    errno = ENOSYS;
    return -1;
}
