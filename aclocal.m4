AC_DEFUN(AB_PROG_PERL, [
AC_ARG_WITH(perl, --with-perl=PERL	Location of Perl, PERLNAME=$withval; WITHPERL=1, [
    AC_PATH_PROG(PERLNAME, perl, , $PERLPATH)])

[
checkperl() {
]
AC_MSG_CHECKING(if your Perl version is sufficient)
[
    TOOOLD=
    $PERLNAME -e 'require 5.004;' 2> /dev/null && echo "yes" || TOOOLD=1
    test -n "$TOOOLD" && echo "no"
}

perltooold() {
    echo Your perl version is too old. HTPL requires Perl 5.004
    exit
}

if [ -n "$PERLNAME" ]; then
    checkperl
    if [ -n "$TOOOLD" ] && [ -z "$WITHPERL" ]; then
]

        AC_PATH_PROG(PERL5NAME, perl5, , $PERLPATH)
        PERLNAME=$PERL5NAME
[
        if [ -n "$PERLNAME" ]; then
            checkperl
            if [ -n "$TOOOLD" ]; then
                perltooold
            fi
        else
                perltooold
        fi
    fi
else
    echo "Perl not found"
    exit
fi

PERL=$PERLNAME
AC_SUBST(PERL)
])
