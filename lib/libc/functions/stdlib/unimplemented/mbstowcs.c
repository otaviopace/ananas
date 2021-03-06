/*-
 * SPDX-License-Identifier: Zlib
 *
 * Copyright (c) 2009-2018 Rink Springer <rink@rink.nu>
 * For conditions of distribution and use, see LICENSE file
 */
#include <stdlib.h>
#include <errno.h>

size_t mbstowcs(wchar_t* pwcs, const char* s, size_t n)
{
    errno = EILSEQ;
    return (size_t)-1;
}
