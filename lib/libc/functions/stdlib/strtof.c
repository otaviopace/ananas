/* float strtof( const char * nptr, char * * endptr )


   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <stdlib.h>

#ifndef REGTEST

float strtof( const char * nptr, char * * endptr )
{
    return (float)strtod( nptr, endptr );
}

#endif
