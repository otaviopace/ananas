/*-
 * SPDX-License-Identifier: Zlib
 *
 * Copyright (c) 2009-2018 Rink Springer <rink@rink.nu>
 * For conditions of distribution and use, see LICENSE file
 */
#ifndef ANANAS_DLFCN_H
#define ANANAS_DLFCN_H

#include <sys/cdefs.h>

__BEGIN_DECLS

#define RTLD_LAZY   1
#define RTLD_NOW    2

#define RTLD_GLOBAL 4
#define RTLD_LOCAL  0

void* dlopen(const char* file, int mode);
void* dlsym(void* handle, const char* name);
int dlclose(void* handle);
char* dlerror(void);

/* Extensions */
typedef struct dl_info {
    const char* dli_fname;
    void* dli_fbase;
    const char* dli_sname;
    void* dli_saddr;
} Dl_info;

int dladdr(const void* addr, Dl_info* info);

__END_DECLS

#endif /* ANANAS_DLFCN_H */
