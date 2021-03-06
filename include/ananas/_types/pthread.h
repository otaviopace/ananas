/*-
 * SPDX-License-Identifier: Zlib
 *
 * Copyright (c) 2009-2018 Rink Springer <rink@rink.nu>
 * For conditions of distribution and use, see LICENSE file
 */
#include <machine/_pthread_types.h>

#ifndef __PTHREAD_T_DEFINED
#define __PTHREAD_T_DEFINED
/*
 * We use pointers to avoid any userland dependency on
 * their size/layout; also, at least some software (LLVM) depends on being able
 * to compare these using < instead of pthread_equal().
 */
typedef struct pthread_attr* pthread_attr_t;
typedef struct pthread_cond* pthread_cond_t;
typedef struct pthread_condattr* pthread_condattr_t;
typedef struct pthread_mutex* pthread_mutex_t;
typedef struct pthread_mutexattr* pthread_mutexattr_t;
typedef struct pthread_rwlock* pthread_rwlock_t;
typedef struct pthread_rwlockattr* pthread_rwlockattr_t;
typedef struct pthread* pthread_t;

// Define the simpler types immediately
typedef unsigned int pthread_key_t;
typedef int pthread_once_t;
#endif
