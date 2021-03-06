/*-
 * SPDX-License-Identifier: Zlib
 *
 * Copyright (c) 2009-2018 Rink Springer <rink@rink.nu>
 * For conditions of distribution and use, see LICENSE file
 */
#include <dlfcn.h>

#pragma weak dlerror

char* dlerror() { return "dynamic linker not available"; }
