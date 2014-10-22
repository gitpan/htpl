/***************************************************************
 * HTPL by Ariel Brosh                                         *
 * This program can be used freely, but the source should not  *
 * be modified without notice.                                 *
 * Copyright 1997-1999 Ariel Brosh                             *
 ***************************************************************/

/*********************************************
 * htpl-l.c - HTPL service library           *
 *********************************************/

#include "htpl.h"

/*********************************************
 * Read one token from a string              *
 *********************************************/

void eat(line, token)
    STR *line, token; {
    pchar save = token;
    while (isdelim(**line)) (*line)++;
    for (; !isdelim(**line) && **line; token++, (*line)++) 
        *token = **line;
    while (isdelim(**line)) (*line)++;
    *token = '\0';
}

/*********************************************
 * Copy input file to output file            *
 *********************************************/

long fcpy(i, o, how)
    int how;
    FILE *i, *o; {
    long red;
    long total = 0;
    STR buff;

    buff = malloc(BUFF_SIZE);

    while (red = fread(buff, 1, BUFF_SIZE, i)) {
        if (!how) fwrite(buff, 1, red,o);
        else outblock(o, buff);
        total += red;
    }

    free(buff);
    return red;
}


/****************************************
 * Open a file, complain if failed      *
 ****************************************/

FILE *FOPEN(filename, mode)
    STR filename, mode; {
    FILE *f;

    f = fopen(filename, mode);
    if (f) return f;
#ifndef __DEBUG__
    printf("Content-type: text/plain\n\n");
#endif
    printf("Could not open %s", filename);
    if (strchr(mode, 'w')) printf(" for write"); else
    if (strchr(mode, 'r')) printf(" for read");
#ifndef _WIN32
    printf(": %s%s", sys_errlist[errno], NEWLINE);
#endif
    printf(NEWLINE);
    exit(0);
}


/* This is basically for debugging */
void emergency() {
    printf("Content-type: text/plain\n\nEmergency exit");
    exit(0);
}
/*
** This is obsolette
void mychdir(filename) 
    STR filename; {
    pchar ch;
    char dir[80];
    strcpy(dir, filename);
    ch = strrchr(dir, SLASH_CHAR);
    if (!ch) return;
    *ch = '\0';
    chdir(dir);
}
*/

/*********************************************************************
 * Make a temporary filename for the specific session for a specific *
 * file type                                                         *
 *********************************************************************/

void maketemp(filename, ext)
    STR filename, ext; {

    sprintf(filename, "%s%c~~htpl-%05d-%05d~~.%s", tmpdir, SLASH_CHAR,
                      getpid(), time(NULL), ext);
}

/*******************************************************************
 * Duplicate a filename, changing extension                        *
 * Add a reference to cache subdirectory if such exists            *
 *******************************************************************/

void makecache(src, rdst, ext)
    STR src, rdst, ext; {
    pchar ch;
    FILENAME dir, dir2;
    DIR *d;
    pchar dst;

/* Get the directory of the file */

    finddir(src, dir);

    dst = dir;

#ifdef __DEBUG__
    if (!noweb && *ext) {
#endif

/* Check for cache directory */

    sprintf(dir2, "%s%chtpl-cache", dir, SLASH_CHAR);
    if (d = opendir(dir2)) {
        closedir(d);
        dst = dir2;
    }

#ifdef __DEBUG__
    }
#endif

/* Copy the filename */

    strcat(dst, SLASH_STR);
    ch = strrchr(src, SLASH_CHAR);
    if (!ch) ch = src; else ch++;
    strcat(dst, ch);

/* Find the extension */

    ch = strrchr(strrchr(dst, SLASH_CHAR), '.');
    if (!ch) ch = &dst[strlen(dst)];

/* Add extension */

    if (!*ext) {
        *ch = '\0';
        return;
    }
    *ch++ = '.';

    strcpy(ch, ext);
    strcpy(rdst, dst);
}

/********************************************
 * Find histroic dependencies between files *
 ********************************************/

long depend(filename1, filename2) 
    STR filename1, filename2; {

    long time1 = getftime(filename1);
    long time2 = getftime(filename2);

    return time2 - time1;
}

/***********************************
 * Get file change time            *
 ***********************************/

long getftime(filename)
    STR filename; {
    struct stat st;

    if (stat(filename, &st) < 0) return 0;

    return st.st_mtime;
}

/******************************************
 * Extract directory out of filename      *
 ******************************************/

void finddir(file, dir)
    STR file, dir; {
    pchar ch;
    FILENAME tempdir;

    strcpy(dir, file);
    ch = strrchr(dir, SLASH_CHAR);
    if (!ch) {
        getcwd(dir, sizeof(FILENAME));
    } else {
        *++ch = '\0';
        getcwd(tempdir, sizeof(FILENAME));
        chdir(dir);
        getcwd(dir, sizeof(FILENAME));
        chdir(tempdir);
    }
/* CYGWIN is being nice - no thanks */
    for (ch = dir; *ch; ch++ )
       if (*ch == '/') *ch = SLASH_CHAR;
/* Append slash */
/*    strcpy(ch, SLASH_STR);*/
}

/********************************************************
 * Chop directory information out of filename           *
 ********************************************************/

void nodir(spec, file)
    STR spec, file; {

    pchar ch;
    ch = strrchr(spec, SLASH_CHAR);
    if (!ch) ch = file; else ch++;
    strcpy(file, ch);
}

/***********************************************************
 * strrcmp library function                                *
 ***********************************************************/

int strrcmp(s1, s2)
    STR s1, s2; {

    pchar ch1, ch2;
    int l1, l2;

    l1 = strlen(s1);
    l2 = strlen(s2);

    for (ch2 = s2; l1 < l2; ch2++, l2--) ;
    for (ch1 = s1; l1 > l2; ch1++, l1--) ;
    return strcmp(ch1, ch2);
}

/************************************************
 * Line print input into output                 *
 ************************************************/

void flpt(i, o)
    FILE *i, *o; {

    STR buff;
    int line = 0, line2 = 0, nline, l;
    FILENAME filename, newfilename;
    TOKEN s;

    buff = malloc(BUFF_SIZE);

    strcpy(filename, "");
    while (!feof(i)) {
        fgets(buff, BUFF_SIZE, i);
        buff[strlen(buff) - 1] = '\0';
        if (feof(i)) break;
        if (sscanf(buff, "#line %d %s", &nline, newfilename) == 2) {
            if (!strcmp(newfilename, thescript)) {
                filename[0] = '\0';
                line2 = 0;
            } else {
                line2 = nline;
                nodir(newfilename, filename);
            }
        } else if (line2) fprintf(o, "[%s(%d)] ", filename, line2++);
        fprintf(o, "%d> %s%s", ++line, buff, NEWLINE);
    }
}

/**********************************************
 * Output debug information                   *
 **********************************************/

void outdbg(o, line)
    FILE *o;
    STR line; {
    outf(o, "# Line: %d File: %s >> %s", nline, infile, line);
/*    outline(o, thescript, rline + 1 + 1);*/
}

/***************************************************
 * Format a string into temporary storage          *
 ***************************************************/
LISTER(mysprintf, STR, )
    RETVAL = r;
    r = NULL;
ENDLISTER

/****************************
 * Dump scope stack         *
 ****************************/

void dumpscopes() {
    if (scopelevel > 0) {
        printf("Dumping scope stack:%s", NEWLINE);
        while (scopelevel > 0 && currscope) {
            printf("%d) %s in line %d.%s", scopelevel,
              scope_names[currscope->scope], currscope->nline, NEWLINE);
            popscope();
        }
    }
}

/******************************************************
 * Return string or a blank string for a null pointer *
 ******************************************************/

STR nprot(s) 
    STR s; {

    static STR blank = "";

    if (s) return s;
    return blank;
}

/********************************************************
 * Convert time to ASCII                                *
 ********************************************************/

STR convtime(t)
    time_t t; {

    static char stat[1024];
    pchar ch, dst;

    struct tm *timep;

    timep = localtime(&t);

    ch = asctime(timep);
    dst = stat;

    while (*ch && *ch != '\n') *dst++ = *ch++;
    *dst = '\0';
    return stat;
}

/*************************************
 * Stack implementation              *
 *************************************/

/********************
 * Push             *
 ********************/
    
void ppush(stack, elem)
    LINK_EL *stack;
    void *elem; {

    LINK_EL curr;

    curr = malloc(sizeof(struct link_el));
    curr->data = elem;
    curr->next = *stack;
    *stack = curr;
}

/*************
 * Pop       *
 *************/

void *ppop(stack) 
    LINK_EL *stack; {

    void *elem, *old;

    if (!*stack) return NULL;
    free((*stack)->data);
    old = *stack;
    *stack = (*stack)->next;
    free(old);
    if (*stack) elem = (*stack)->data; else elem = NULL;
    return elem;
}

/*************************************
 * Simple setenv function            *
 *************************************/

void SETENV(key, val)
    STR key;
    STR val; {

    TOKEN s;
    sprintf(s, "%s=%s", key, val);
    putenv(s);
}

/*******************************************************
 * Spawn a Perl process                                *
 *******************************************************/

int execperl(argv, output, postdata, error, redir) 
    int redir;
    char **argv;
    STR output, postdata, error; {

    int code;
    int chld;
    int nerr, nout, nin;
    FILENAME cmd;

    if (!redir) {
        if (!(chld=fork())) {
            execv(PERL_BIN, argv);
        }
        waitpid(chld, &code, 0);
        return code;
    }

/* Ok, we need to capture STDOUT and STDERR, and to feed STDIN */

    nout=creat(output, 0777);
    nin=open(postdata, O_RDONLY);
    nerr=creat(error, 0777);

    if (!(chld=fork())) {

        dup(0);
        close(0);
        dup(nin);

        dup(1);
        close(1);
        dup(nout);

        dup(2);
        close(2);
        dup(nerr);
        execv(PERL_BIN, argv);
    } else {
        waitpid(chld, &code, 0);
        close(nerr);
        close(nin);
        close(nout);
    }
    return code;
}
/*
int dumbexecperl(argv, output, postdata, error, redir) 
    int redir;
    char **argv;
    STR output, postdata, error; {

    COMMAND line, cmd;

    strcpy(line, PERL_BIN);
    while (*argv) {
        strcat(line, " ");
        strcat(line, *argv++);
    }
    if (redir) sprintf(cmd, "%s <
    system(line);
}
*/

/****************************************************
 * Encode string for HTTP                           *
 ****************************************************/

void htencode(str, decstr)
    STR str, decstr; {

    TOKEN c;
    pchar ch, dst;
    static STR HEX = "0123456789ABCDEF";

    for (ch = str, dst = decstr; *ch; ch++) {
        if (*ch == ' ') *dst++ = '+'; 
        else if (isalnum(*ch)) *dst++ = *ch;
        else {
            *dst++ = '%';
            *dst++ = HEX[(*ch) / 16];
            *dst++ = HEX[(*ch) % 16];
        }
    }
    *dst = '\0';
}

/****************************************************
 * Encode SGML strings with entities                *
 ****************************************************/

/* Mostly taken from e-perl. Allows perl runtime macroes to process HTML
code with entities. */

struct html2char {
    STR h;
    char c;
};

static struct html2char html2char[] = {
    { "copy",   '�' },    /* Copyright */
    { "die",    '�' },    /* Di�resis / Umlaut */
    { "laquo",  '�' },    /* Left angle quote, guillemot left */
    { "not",    '�' },    /* Not sign */
    { "ordf",   '�' },    /* Feminine ordinal */
    { "sect",   '�' },    /* Section sign */
    { "um",     '�' },    /* Di�resis / Umlaut */
    { "AElig",  '�' },    /* Capital AE ligature */
    { "Aacute", '�' },    /* Capital A, acute accent */
    { "Acirc",  '�' },    /* Capital A, circumflex */
    { "Agrave", '�' },    /* Capital A, grave accent */
    { "Aring",  '�' },    /* Capital A, ring */
    { "Atilde", '�' },    /* Capital A, tilde */
    { "Auml",   '�' },    /* Capital A, di�resis / umlaut */
    { "Ccedil", '�' },    /* Capital C, cedilla */
    { "ETH",    '�' },    /* Capital Eth, Icelandic */
    { "Eacute", '�' },    /* Capital E, acute accent */
    { "Ecirc",  '�' },    /* Capital E, circumflex */
    { "Egrave", '�' },    /* Capital E, grave accent */
    { "Euml",   '�' },    /* Capital E, di�resis / umlaut */
    { "Iacute", '�' },    /* Capital I, acute accent */
    { "Icirc",  '�' },    /* Capital I, circumflex */
    { "Igrave", '�' },    /* Capital I, grave accent */
    { "Iuml",   '�' },    /* Capital I, di�resis / umlaut */
    { "Ntilde", '�' },    /* Capital N, tilde */
    { "Oacute", '�' },    /* Capital O, acute accent */
    { "Ocirc",  '�' },    /* Capital O, circumflex */
    { "Ograve", '�' },    /* Capital O, grave accent */
    { "Oslash", '�' },    /* Capital O, slash */
    { "Otilde", '�' },    /* Capital O, tilde */
    { "Ouml",   '�' },    /* Capital O, di�resis / umlaut */
    { "THORN",  '�' },    /* Capital Thorn, Icelandic */
    { "Uacute", '�' },    /* Capital U, acute accent */
    { "Ucirc",  '�' },    /* Capital U, circumflex */
    { "Ugrave", '�' },    /* Capital U, grave accent */
    { "Uuml",   '�' },    /* Capital U, di�resis / umlaut */
    { "Yacute", '�' },    /* Capital Y, acute accent */
    { "aacute", '�' },    /* Small a, acute accent */
    { "acirc",  '�' },    /* Small a, circumflex */
    { "acute",  '�' },    /* Acute accent */
    { "aelig",  '�' },    /* Small ae ligature */
    { "agrave", '�' },    /* Small a, grave accent */
    { "amp",    '&' },    /* Ampersand */
    { "aring",  '�' },    /* Small a, ring */
    { "atilde", '�' },    /* Small a, tilde */
    { "auml",   '�' },    /* Small a, di�resis / umlaut */
    { "brkbar", '�' },    /* Broken vertical bar */
    { "brvbar", '�' },    /* Broken vertical bar */
    { "ccedil", '�' },    /* Small c, cedilla */
    { "cedil",  '�' },    /* Cedilla */
    { "cent",   '�' },    /* Cent sign */
    { "curren", '�' },    /* General currency sign */
    { "deg",    '�' },    /* Degree sign */
    { "divide", '�' },    /* Division sign */
    { "eacute", '�' },    /* Small e, acute accent */
    { "ecirc",  '�' },    /* Small e, circumflex */
    { "egrave", '�' },    /* Small e, grave accent */
    { "eth",    '�' },    /* Small eth, Icelandic */
    { "euml",   '�' },    /* Small e, di�resis / umlaut */
    { "frac12", '�' },    /* Fraction one-half */
    { "frac14", '�' },    /* Fraction one-fourth */
    { "frac34", '�' },    /* Fraction three-fourths */
    { "gt",     '>' },    /* Greater than */
    { "hibar",  '�' },    /* Macron accent */
    { "iacute", '�' },    /* Small i, acute accent */
    { "icirc",  '�' },    /* Small i, circumflex */
    { "iexcl",  '�' },    /* Inverted exclamation */
    { "igrave", '�' },    /* Small i, grave accent */
    { "iquest", '�' },    /* Inverted question mark */
    { "iuml",   '�' },    /* Small i, di�resis / umlaut */
    { "lt",     '<' },    /* Less than */
    { "macr",   '�' },    /* Macron accent */
    { "micro",  '�' },    /* Micro sign */
    { "middot", '�' },    /* Middle dot */
    { "nbsp",   ' ' },    /* Non-breaking Space */
    { "ntilde", '�' },    /* Small n, tilde */
    { "oacute", '�' },    /* Small o, acute accent */
    { "ocirc",  '�' },    /* Small o, circumflex */
    { "ograve", '�' },    /* Small o, grave accent */
    { "ordm",   '�' },    /* Masculine ordinal */
    { "oslash", '�' },    /* Small o, slash */
    { "otilde", '�' },    /* Small o, tilde */
    { "ouml",   '�' },    /* Small o, di�resis / umlaut */
    { "para",   '�' },    /* Paragraph sign */
    { "plusmn", '�' },    /* Plus or minus */
    { "pound",  '�' },    /* Pound sterling */
    { "quot",   '"' },    /* Quotation mark */
    { "raquo",  '�' },    /* Right angle quote, guillemot right */
    { "reg",    '�' },    /* Registered trademark */
    { "shy",    '�' },    /* Soft hyphen */
    { "sup1",   '�' },    /* Superscript one */
    { "sup2",   '�' },    /* Superscript two */
    { "sup3",   '�' },    /* Superscript three */
    { "szlig",  '�' },    /* Small sharp s, German sz */
    { "thorn",  '�' },    /* Small thorn, Icelandic */
    { "times",  '�' },    /* Multiply sign */
    { "uacute", '�' },    /* Small u, acute accent */
    { "ucirc",  '�' },    /* Small u, circumflex */
    { "ugrave", '�' },    /* Small u, grave accent */
    { "uuml",   '�' },    /* Small u, di�resis / umlaut */
    { "yacute", '�' },    /* Small y, acute accent */
    { "yen",    '�' },    /* Yen sign */
    { "yuml",'\255' },    /* Small y, di�resis / umlaut */
    { NULL, 0 }
};

void htmldecode(src, dst) 
    STR src, dst; {

    pchar ch1, ch2, ch3;
    int p;
    TOKEN token;
    int i;

    ch1 = src;
    ch2 = dst;

    while (*ch1) {
        if (*ch1 != '&')
            *ch2++ = *ch1++;
        else {
            ch3 = token;
            ch1++;
            while (*ch1 && *ch1 != ';') *ch3++ = *ch1++;
            if (*ch1) ch1++;
            *ch3 = '\0';
            if (token[0] == '#') {
                ch3 = token + 1;
                p = atoi(ch3);
                *ch2++ = (char)p;
            } else {
                p = 1;
                i = 0;
                while (p && html2char[i].c) {
                    if (!strncasecmp(token, html2char[i].h,
                           strlen(token))) {
                               *ch2++ = html2char[i].c;
                               p = 0;
                    }
                    i++;
                }
                if (p) {
                    *ch2++ = '&';
                    ch3 = token;
                    while (*ch3) *ch2++ = *ch3++;
                }
            }
        }
    }
    *ch2 = '\0';
}

/************************************************
 * Fork a subprocess to compile a needed module *
 ************************************************/

void compile(src, dst)
    STR src, dst; {
    int oout, nout;
    int p[2];
    int code;
    int pid;
    FILE *e, *o;
    FILENAME thedir;

    oout = dup(1);
    pipe(p);
    if (!(pid = fork())) {
        close(1);
        dup(p[1]);
        finddir(src, thedir);
        chdir(thedir);
        execl(myself, myself, "-t", "-w", "-o", dst, src, NULL);
        exit(0);
    }
    waitpid(pid, &code, 0);

    if (!WIFEXITED(code)) {
        e = fdopen(p[0], "r");
        o = fdopen(oout, "w");
        fcpy(e, o, 0);
        fclose(e);
        fclose(o);
        close(p[1]);
        exit(1);
    }
    close(1);
    close(p[0]);
    close(p[1]);
    dup(oout);
}

/****************************************************
 * Try to find a filename with a list of extensions *
 ****************************************************/

void tryexts(src, dst, exts)
    STR src, dst, exts; {

    pchar ptr, ch;
    FILE *t;
    TOKEN ext;
 
    ext[0] = '\0';

    ptr = exts;

    while (ptr) {
        strcpy(dst, src); 
        strcat(dst, ext);   
        t = fopen(dst, "r");
        if (t) {
            fclose(t);
            return;
        }
        ptr = strchr(ptr, '.');
        if (!ptr) break;
        strcpy(ext, ptr);
        ch = strchr(ext + 1, '.');
        if (ch) *ch = '\0';
        ptr++;
    }
    strcpy(dst, src);
}

/***********************************
 * Dependency database             *
 ***********************************/

#ifdef __DEPEND_DB__

#include <db_185.h>

#define VECTOR 6
typedef int dst_t[2][VECTOR];

int checksum(filename)
    STR filename; {

    int r = 0, x = 0;
    FILE *i = FOPEN(filename, "r");
    while (!feof(i)) r += (getw(i) ^ ++x);
    fclose(i);
    return r;
}

/*******************************************
 * Create a block from file statistics     *
 *******************************************/

int loadstat(ary, filename)
    STR filename;
    int ary[]; {
    struct stat st; 

    if (stat(filename, &st) < 0) return 0;
    bzero(ary, sizeof(int) * VECTOR);
    ary[0] = st.st_mtime;
    ary[1] = st.st_ctime;
    ary[2] = st.st_mode;
    ary[3] = st.st_size;
    ary[4] = st.st_ino;
    ary[5] = checksum(filename);
    return 1;
}

/****************************************************************
 * Validate or store information about two dependent files      *
 ****************************************************************/

short fit(filename1, filename2, action) 
    STR filename1, filename2;
    int action; {

    FILENAME dbfn;
    TOKEN skey;
    DB *db;
    dst_t *rec, facts;
    DBT key, datum;
    int code;
    pchar ch;
    char c, c2;

    loadstat(facts[0], filename1);
    if (!loadstat(facts[1], filename2)) return 0;

    c = (char)(facts[0][3] + facts[1][3]);
    c2 = (char)(facts[0][0] + facts[1][0]);

    sprintf(dbfn, "%s/htpl-depend.db", scriptdir);
    sprintf(skey, "%s//\\\\//%s", filename1, filename2);

    for (ch = skey; *ch; ch++, c = c + c2) *ch ^= c;

    db = dbopen(dbfn, O_CREAT | O_RDWR, S_IREAD | S_IWRITE, DB_HASH, NULL);
    
    key.data = (PTR)&skey[0];
    key.size = strlen(skey);
    if (action == 0) {
        if (db->get(db, &key, &datum, 0)) return 1;
        rec = datum.data;
        code = memcmp(rec, &facts, sizeof(dst_t));
        db->close(db);
        return code;
    }
    if (action == 1) {
        datum.data = &facts;
        datum.size = sizeof(dst_t);
        db->put(db, &key, &datum, 0);
        db->close(db);
        return;
    }
}

#else

/************************************************************
 * Always succeed if dependency database is not implemented *
 ************************************************************/

short fit(filename1, filename2, action) 
    STR filename1, filename2;
    int action; {

    return 0;
}

#endif /* __DEPEND_DB__ */

/*********************************
 * Output #line numbers          *
 *********************************/

void outline(o, filename, line)
    FILE *o;
    STR filename;
    long line; {
    outf(o, "#line %d %s", line, filename);
}

/*****************************************
STR mystrdup(src)
    STR src; {

    STR dst = malloc(strlen(src) + 1);
    strcpy(dst, src);
    return dst;
}
*/

/******************************
 * Initialize a vector        *
 ******************************/

void vectorinit(vec)
    VECTOR vec; {
    vec->alloc = 4;
    vec->num = 0;
    vec->data = calloc(vec->alloc, sizeof(PTR));
}

/****************************************
 * Push element to a vector             *
 ****************************************/

void vectorpush(vec, el)
    VECTOR vec;
    PTR el; {
/*    if (!vec->data) vectorinit(vec);*/
    if (vec->num >= vec->alloc - 1) {
        vec->alloc += 4;
        vec->data = realloc(vec->data, vec->alloc * sizeof(PTR));
    }
    vec->data[vec->num++] = el;
}

/*************************************
 * Destroy a vector                  *
 *************************************/

void vectorkill(vec)
    VECTOR vec; {
    int i;
    if (!vec || !vec) return;
    for (i = 0; i < vec->num; i++) free(vec->data[i]);
    free(vec->data);
}

/***************************************************
 * Report compile time error                       *
 ***************************************************/

LISTER(croak, int, )
    if (fatal) return;
    errstr = r;
    r = NULL;
    fatal = 1;
    sprintf(errloc, "File %s, Line %d", infile, nline);
    RETVAL = 0;
ENDLISTER

/*************************************
 * Write a formatted output line     *
 *************************************/

LISTER(outf, int, LISTERADDVAR(o, FILE *))
    outblock(o, r);
ENDLISTER


/**********************************************************
 * Escape variables Cold Fusion Style                     *
 **********************************************************/

STR escapevars(code)
    STR code; {
    pchar ch;
    short flag = 0;
    STR buff = malloc(strlen(code) * 2);
    pchar dst = buff;
    char alt[100];
    pchar save;

    ch = code;
    while (*ch) {
        if (*ch == '#') {
            switch (flag) {
                case 0: flag = 1;
                        save = dst;
                        dst = alt;
                        break;
                case 1: dst = save;
                        flag = 0;
                        *dst++ = '#';
                        break;
                case 2: *dst = '\0';
                        *save++ = '$';
                        *save++ = '{';
                        strcpy(save, alt);
                        dst = save + strlen(save);
                        *dst++ = '}';
                        flag = 0;
                        break;
            } 
        } else if (isalnum(*ch) || *ch == '_') {
            if (flag == 1) flag = 2;
            *dst++ = *ch;
        } else {
            if (flag) {
                *dst = '\0';
                *save++ = '#';
                strcpy(save, alt);
                dst = save + strlen(save);
                flag = 0;
            }
            *dst++ = *ch;
        }
        ch++;
    }
    *dst = 0;
    return buff;
}

/*********************************************
 * Write text to output file, counting lines *
 *********************************************/

void outblock(o, buffer)
    FILE *o;
    STR buffer; {
    pchar ch2, ch = buffer;
    STR top;

    if (!*buffer) {
        fprintf(o, NEWLINE);
        rline++;
        return;
    }

    while (ch && *ch) {
        ch2 = strchr(ch, '\n'); 
        if (ch2) {
            *ch2 = '\0';
            ch2++;
        }
        fprintf(o, "%s%s", ch, NEWLINE);
        ch = ch2;
        rline++;
    }
}

/*******************************************************
 * Convert slash to local slash                        *
 *******************************************************/

STR qualify(filename, thread)
    STR filename;
    int thread; {

    static FILENAME buffer[4];
    pchar src = filename;
    pchar dst = buffer[thread];

    if (SLASH_CHAR != '\\') return filename;
    while (*src) {
        if (*src == '\\') *dst++ = '\\';
        *dst++ = *src++;
    }
    *dst = '\0';

    return buffer[thread];
}

/***********************************************************
 * Count lines in a buffer                                 *
 ***********************************************************/

int countlines(buff)
    STR buff; {

    pchar ch2, ch = buff;
    int num = 0;

    if (!*buff) return 0;
    while (*ch) {
        ch2 = strchr(ch, '\n');
        if (!ch2) return num;
        num++;
        ch = ch2 + 1;
    }
    return num;
}

/**************************************************************
 * Replace occurences of one specific char                    *
 **************************************************************/

void replacechar(src, ch1, ch2)
    char ch1, ch2;
    STR src; {

    pchar pch;
    if (ch1 == ch2) return;
    for (pch = src; *pch; pch++) if (*pch == ch1) *pch = ch2;
}

/********************************************************
 * Copy files                                           *
 ********************************************************/

void fcopy(src, dst)
    STR src, dst; {

    FILE *i = FOPEN(src, "rb");
    FILE *o = FOPEN(dst, "wb");

    fcpy(i, o, 0);

    fclose(i);
    fclose(o);
}

/*********************************************************
 * Add to binary tree                                    *
 *********************************************************/

BTREE *btreeadd(tree, key, data)
    BTREE *tree;
    STR key;
    PTR data; {

    int cmp;

    while (1) {
        if (!*tree) {
            *tree = malloc(sizeof(struct btree));
            (*tree)->data = data;
            (*tree)->key = strdup(key);
            (*tree)->left = (*tree)->right = NULL;
            return tree;
        }
        cmp = strcmp((*tree)->key, key);
        if (cmp) cmp = (int)(cmp / abs(cmp));
        switch (cmp) {
            case 0 : free((*tree)->data);
                     (*tree)->data = data;
                     return;
            case 1 : tree = &((*tree)->left);
                     break;
            case -1: tree = &((*tree)->right);
                     break;
        }
    } 
}

/***************************************************
 * Scan a binary tree                              *
 ***************************************************/

BTREE *btreescan(tree, proc, level, tag, when) 
    BTREE *tree; 
    int level;
    PTR tag;
    int when;
    btreecallback proc; {

    BTREE *res;

    if (tree && *tree) {
#define SCAN(node) if (res = btreescan(&((*tree)->node), proc, level + 1, tag, when)) \
                     return res;
#define DOIT(w) if (when == w && proc(tree, level, tag)) return tree;
        DOIT(BTREE_PREFIX)
        SCAN(left) 
        DOIT(BTREE_INFIX)
        SCAN(right)
        DOIT(BTREE_POSTFIX)
#undef SCAN
    }
    return NULL;
}

int __btreekillnode(node, level, tag)
    BTREE *node;
    int level; 
    PTR tag; {
    free((*node)->key);
    if ((*node)->data) free((*node)->data);
    free(*node);
    *node = NULL;
    return 0;
}
/****************************************
 * Kill a binary tree                   *
 ****************************************/

void btreekill(tree) 
    BTREE *tree; {
    btreescan(tree, &__btreekillnode, 0, NULL, BTREE_POSTFIX);
}

int __btreechknode(node, level, tag)
    BTREE *node;
    PTR tag;
    int level; {
    return !strcmp((*node)->key, (STR)tag);
}

BTREE *btreesearch2(tree, key)
    BTREE *tree;
    STR key; {
    
    return btreescan(tree, &__btreechknode, 0, (PTR)key, BTREE_INFIX);
}

/***********************************************************
 * Search for a key in a binary tree                       *
 ***********************************************************/

PTR btreesearch(tree, key)
    BTREE tree;
    STR key; {
    
    BTREE *node = btreesearch2(&tree, key);
    if (!node || !*node) return NULL;
    return (*node)->data;
}

/**************************************************************
 * Erase a node in a binary tree                              *
 **************************************************************/

PTR btreedel(node)
    BTREE *node; {
    BTREE left, right, ptr;
    PTR datum = (*node)->data;
    left = (*node)->left;
    right = (*node)->right;
    __btreekillnode(node, 0, NULL);
    ptr = *node = left;
    if (!ptr) {
        *node = right;
        return;
    }
    while (ptr && ptr->right) ptr = ptr->right;
    ptr->right = right;
    return datum;
}

/********************************************************
 * Erase a node in a binary tree by key                 *
 ********************************************************/

PTR btreedelkey(tree, key)
    BTREE *tree;
    STR key; {
    BTREE *node = btreesearch2(tree, key);
    if (node) return btreedel(node);
    return NULL;
}

int __btreesimple(node, level, tag)
    BTREE *node;
    int level;
    PTR tag; {

    btreesimpleproc proc = (btreesimpleproc)tag;
    proc((*node)->data);
    return 0;
}

/***********************************************
 * Simply scan a binary tree                   *
 ***********************************************/

void btreesimplescan(tree, proc)
    BTREE tree;
    btreesimpleproc proc; {

    btreescan(&tree, __btreesimple, 0, (PTR)proc, BTREE_INFIX);
}

int makecomponent(filename) 
    STR filename; {
    char *argv[5];
    FILENAME work, work2;
    FILENAME dirbefore, thisdir;
    FILENAME temp;
    int code;
    pchar ch;

    getcwd(dirbefore, sizeof(FILENAME));
    finddir(filename, thisdir);
    chdir(thisdir);

    nodir(filename, work2);
    ch = strchr(work2, '.');
    if (ch) *ch ='\0';
    sprintf(work, "%s%chtplp", bindir, SLASH_CHAR);
    argv[0] = PERL_BIN;
    argv[1] = work;
    argv[2] = work2;
    argv[3] = NULL;


    maketemp(temp, "comp");
    code = EXECPERL(3, argv, NULL, NULL, temp, 1);
    if (code) {
        FILE *i;
#ifdef __DEBUG__
        if (runit)
#endif
        printf("Content-type: text/plain\n\n");
        printf("Component compiler (htplp) failed:\n");
        i = fopen(temp, "r");
        fcpy(i, stdout, 0);
        fclose(i);
        unlink(temp);
        exit(1);
    }
    unlink(temp);
    chdir(dirbefore);
}

/********************************************************
 * Pipe a string through a process                      *
 ********************************************************/

STR preprocess(str, cmd)
    STR str, cmd; {

    int dad_rdr, dad_wtr, kid_rdr, kid_wtr, dad_err, kid_err;
    int pi[2], po[2], pe[2];
    int pid;
    FILE *f, *i;
    int code;
    STR buff;
    char chunk[BUFF_SIZE];
    int len, red;

    fflush(stdout);

    pipe(pi);
    pipe(po);
    pipe(pe);


    dad_rdr = po[0];
    dad_wtr = pi[1];
    kid_rdr = pi[0];
    kid_wtr = po[1];
    dad_err = pe[0];
    kid_err = pe[1];
    

    if ((pid = fork()) == 0) {
        close(dad_wtr);
        close(dad_rdr);
        close(dad_err);
        close(0);
        dup(kid_rdr);
        close(1);
        dup(kid_wtr);
        close(2);
        dup(kid_err);
        system(cmd);
        fflush(stdout);
        exit(0);
    }

    if (pid < 0) {
        croak("PP: fork failed: %s", sys_errlist[errno]);
        close(kid_err);
        close(kid_rdr);
        close(kid_wtr);
        close(dad_wtr);
        close(dad_rdr);
        close(dad_err);
      
        return strdup("");
    }

    close(kid_err);
    close(kid_rdr);
    close(kid_wtr);

    write(dad_wtr, str, strlen(str));
    close(dad_wtr);

    waitpid(pid, &code, 0);



    if (!WIFEXITED(code)) {
        TOKEN err;
        FILE *e = fdopen(dad_err, "r");
        close(dad_rdr);
        if (e) {
            fgets(err, sizeof(err), e);
            err[strlen(err) - 1] = '\0';
        }
        fclose(e);
        croak("PP: Child returned error: %s", err);
        return strdup("");
    }

    close(dad_err);

    buff = strdup("");
    len = 0;

    while ((red = read(dad_rdr, chunk, sizeof(chunk))) > 0) {
        buff = realloc(buff, len + red);
        memcpy(buff + len, chunk, red);
        len += red;
    }

    close(dad_rdr);
    buff[len] = '\0';
    return buff;
}

/**************************************************************
 * Open a text file, optionally via a filter if such exists   *
 * The second parameter contains the filename to search for   *
 * filter instructions inside                                 *
 **************************************************************/

FILE* openif(filename, filter)
    STR filename, filter; {
    FILENAME dir;
    FILENAME try;
    FILE *f;
    TOKEN pn;

    finddir(filename, dir);
    sprintf(try, "%s%c%s", dir, SLASH_CHAR, filter);
    if (f = fopen(try, "r")) {
        fgets(pn, sizeof(pn), f);
        fclose(f);
        pn[strlen(pn) - 1] = '\0';
        if (!strstr(pn, "%s")) strcat(pn, " %s");
        sprintf(try, pn, filename);
        if (f = popen(try, "r")) return f;
        puts("Content-type: text/plain\n");
        printf("Could not spawn filter %s: %s", filter, sys_errlist[errno]);
        exit(-1);
    }
    return FOPEN(filename, "r");
}

/*********************************************************
 * Open an HTPL source                                   *
 *********************************************************/

FILE* opensource(filename)
    STR filename; {
    return openif(filename, "htsource.pre");
}

/*********************************************************
 * Open output of an HTPL script                         *
 *********************************************************/

FILE* openoutput(filename)
    STR filename; {
    return openif(filename, "htout.pre");
}

short debugforbidden(out) 
    FILE *out; {

    unsigned long a, b, c, d, e;
    unsigned long rh, sn, nm;
    FILE *i;
    FILENAME inp;
    TOKEN line;
    pchar ch, ch2;

#define VOUS(a, b, c, d) (((((((a) << 8) + (b)) << 8) + (c)) << 8) + (d))

    static STR mask ="%s%chtpl.dbg";

    STR remote = getenv("REMOTE_ADDR");
    if (!remote) return 0;
    sprintf(inp, mask, scriptdir, SLASH_CHAR);
    i = fopen(inp, "r");

    if (!i) {
        sprintf(inp, mask, bindir, SLASH_CHAR);
        i = fopen(inp, "r");
    }

    if (!i) return 0;

    sscanf(remote, "%d.%d.%d.%d", &a, &b, &c, &d);
    rh = VOUS(a, b, c, d);
    if (rh == VOUS(127, 0, 0, 1)) {
        fclose(i);
        return 0;
    }

    for (;;) {
        fgets(line, sizeof(line), i);
        if (feof(i)) {
            fclose(i);
            return 1;
        }
        ch = &line[strlen(line)];
        while (*ch == '\n' || isspace(*ch)) *ch-- = '\0';
        strcpy(ch + 1, "\n");
        ch2 = line;
        while (isspace(*ch2)) ch2++;
        ch = strpbrk(ch2, "\t ");
        if (!ch) break;
        *ch++ = '\0';
        d = -1;
        sscanf(ch2, "%d.%d.%d.%d", &a, &b, &c, &d);
        if (d < 0) break;
        sn = VOUS(a, b, c, d);
        while (isspace(*ch)) ch++;
        d = -1;
        sscanf(ch, "%d.%d.%d.%d%c", &a, &b, &c, &d, &e);
        if (e != '\n') break;
        nm = VOUS(a, b, c, d);

        if ((sn & nm) == (rh & nm)) {
            fclose(i);
            return 0;
        }
    }    
    fprintf(out, "%sFormat of %s wrong%s", NEWLINE, inp, NEWLINE);
    fclose(i);
    return 1;
}
