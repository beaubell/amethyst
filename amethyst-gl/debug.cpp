/***********************************************************************
 Amethyst-GL
  - Debugging helper functions

 Authors (c):
 2006-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "debug.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <malloc.h>
#ifdef HAVE_EXECINFO_H
  #include <execinfo.h>
#endif

#define MAX_BT 30

namespace amethyst {
namespace client {

void print_trace(void)
{
#ifdef HAVE_EXECINFO_H
    printf ("Printing BackTrace:..");

    void *array[MAX_BT];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace (array, MAX_BT);
    strings = backtrace_symbols (array, size);

    printf ("Obtained %zd stack frames.\n", size);

    for (i = 0; i < size; i++)
        printf ("%s\n", strings[i]);

    free (strings);
#endif
}

} // namespace client
} // namespace amethyst
