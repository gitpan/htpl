#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

MODULE = Crypt::Simple		PACKAGE = Crypt::Simple		

SV *
encrypt(string, key)
SV * string
SV * key
CODE:
	char *ch = SvPV(key, PL_na);
	STRLEN ln;
	long i = 0;
	char *src = SvPV(string, ln);
	char *dst = malloc(ln);
        char *ptr = dst;

        while (i++ < ln) {
		*ptr++ = *src++ ^ *ch++;
		if (!*ch) ch = SvPV(key, PL_na);
	}
	*ptr = '\0';
	RETVAL = newSVpv(dst, ln);
	free(dst);
	OUTPUT: 
	RETVAL
