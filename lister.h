/******************************************************************
Interface for variant arguments.
    LISTER(function to define, return type, variables to define)
    Return type should not be void.
    The variables will be poped of the arg list. Then a format string and
    parameters will be pulled, and formatted into a a string on the heap 
    called r. 
    You can then do anything with r, and assign a variable called RETVAL
    the return value. If you are passing r as a pointer to a string to be
    saved, set it to NULL so that ENDLISTER does not free it.

    ENDLISTER 
    To end a function

    LISTERADDVAR(variable, type)
    To add a variable before the format string.

Example:
    LISTER(printftimes, int, int j; LISTERADDVAR(times, int))
    for (i = 0; i < times; i++) {
        puts(r);
        if (getchar() == EOF) {
            RETVAL = i;
            break;
        }
    }
    ENDLISTER

    x = printftimes(10, "%s is %i on %f", "Hello world", 3, 1.5);

Written by Ariel Brosh
*****************************************************/

#ifndef __LISTER_H__
#define __LISTER_H__
#include <varargs.h>

#define LISTER(name, ret, add) ret name(va_alist) va_dcl { \
    char *r, *msg; \
    ret RETVAL; \
    va_list va_start(ap); \
    add \
    r = malloc(2048); \
    msg = va_arg(ap, char *); \
    vsprintf(r, msg, ap); \
    va_end(ap); 

#define ENDLISTER \
if (r) free(r); \
return RETVAL; \
}

#define LISTERADDVAR(v, t) \
t v = va_arg(ap, t);

#endif // __LISTER_H__
