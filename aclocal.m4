AC_DEFUN(AB_PERL_CHECK, [
    AC_MSG_CHECKING(if your Perl version has $1)
    $PERL -e 'require $1;' 2> /dev/null && ab_misfit=  || ab_misfit=1 
    [ if [ -z "$ab_misfit" ]; then ]
        echo "yes"
	$2
    else
        echo "no"
	$3
    fi
])

AC_DEFUN(AB_PROG_PERL, [
    ab_perl_path=$PATH:/usr/bin:/usr/local/bin:/usr/contrib/bin
    AC_ARG_WITH(perl, --with-perl=PERL	Location of Perl, PERL=$withval; ab_with_perl=1, [
        AC_PATH_PROG(PERL, perl, , $ab_perl_path)])

    [ if [ -n "$PERL" ]; then ]
	AB_PERL_CHECK($1, $2, [
	    [ if [ -n "$ab_with_perl" ]; then ]
	        $3
	    else
                AC_PATH_PROG(ab_perl5, perl5, , $ab_perl_path)
                PERL=$ab_perl5
                [ if [ -n "$PERL" ]; then ]
                    AB_PERL_CHECK($1, $2, $3)
                fi
            fi
        ])
    else
        $3
    fi

    AC_SUBST(PERL)
])

AC_DEFUN(AB_CHECK_EMBEDDED_PERL, [
    SAVECFLAGS=$CFLAGS
[ CFLAGS="$CFLAGS `$PERL -MExtUtils::Embed -e ccopts -e ldopts`" ]
    SAVELIBS=$LIBS
    LIBS="$LIBS -lperl"
    AC_MSG_CHECKING(for embedded Perl)
    AC_TRY_RUN( [

#include <EXTERN.h>
#include <perl.h>
static PerlInterpreter *my_perl;

int main(int argc, char **argv, char **env) {
               int argc;
               char *argvv[3];
               char *myself = "htpl.cgi";
               argc = 2;
               argvv[0] = myself;
               argvv[1] = "-e 'exit(0);'"; 
               my_perl = perl_alloc();
               perl_construct(my_perl);
               perl_parse(my_perl, NULL, argc, argvv, (char **)NULL);
               perl_run(my_perl);
               perl_destruct(my_perl);
               perl_free(my_perl);
               exit(0);
           }
], [ echo yes; 
$1 
], [ echo no; LIBS=$SAVELIBS; CFLAGS=$SAVECFLAGS; 
$2 
], [ echo no; LIBS=$SAVELIBS; CFLAGS=$SAVECFLAGS; 
$2 
] )
])

AC_DEFUN(AB_CHECK_TEMP, [
    AC_MSG_CHECKING(if users can write in $1)
     ab_dir="$1"
[   ab_fn="$$ `date` ab setup" ]
[    ab_fn=$ab_dir/"`echo $ab_fn | sed 's/ /_/g'`" ]
[    if [ "$UID" = 0 ]; then ]
        su nobody -c "touch $ab_fn" 2> /dev/null
    else
        touch $ab_fn 2> /dev/null
    fi

[    if [ -f $ab_fn ]; then ]
        rm $ab_fn
        echo "yes"
        $2
    else
        echo "no"
        $3
    fi 
])

