#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "../htpl.h"

MODULE = Apache::HTPL		PACKAGE = Apache::HTPL		

int
compile(page, script, bin)
char * page
char * bin
char * script
CODE:
    xsub_entry(page, script, bin);

