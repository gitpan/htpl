#ifdef __cplusplus
extern "C" {
#endif
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#ifdef __cplusplus
}
#endif


/* The origin of this code is unknown */

#include <string.h>
#include <stdlib.h>

void strreverse(char *s1,char *s2)
{
	char *s_start,*s_end;
	char s_buf[5000],s_buf2[5000],*s=s_buf,*old_s1;

	old_s1=s1;

	strcpy(s_buf2,s1);
	s_end=s_buf2+(s2-s1);
	s_start=s_buf2;


	while (s_start!=s_end) {
		*s=*s_end;
		s_end--; s++;
	}
	*s=*s_end; s++;

	*s=0; s=s_buf;
	
	while (*s) {
		*s1=*s;
		s++; s1++;
	}
	s1=old_s1;
}
void eng_strreverse(char *s1)
{
	char *s,*s_start,*s_end,*olds1;
	int revflag=1,flag_start=1;
	
	olds1=s1;
	s=s1;
	while (*s)
	{
		if (*s=='(') *s=')'; else
			 if (*s==')') *s='(';
		if (((*s<'à') || (*s>'ú')) && (*s!='.') && (*s!=' ')) {
			if (revflag) {
				s_start=s;
				revflag=0;
			}
		 } else if (!revflag)  {
				s_end=s-1;
				revflag=1;
				strreverse(s_start,s_end);
			}
		s++;
	}
	if (!revflag) {
		 s_end=s-1;
		 strreverse(s_start,s_end);
	}
	s1=olds1;
}

char *hebrew_str(const char *s1)
{
	static char hebrew_str[5000];

	strcpy(hebrew_str,s1);
	strreverse(hebrew_str,&hebrew_str[strlen(hebrew_str)-1]);

	eng_strreverse(hebrew_str);

	return hebrew_str;
}

char *hebrew(char *s)
{
	static char new_str[5000];
	char *s2,*s1;
	char strbuf[5000],*str_ptr;

	new_str[0]=0;
	s1=strdup(s);
	s2=strtok(s1,"|");
	strcpy(strbuf,s);
	str_ptr=strbuf;

	while (s2!=NULL) {
		strcat(new_str,hebrew_str(s2));
		str_ptr=strbuf+strlen(new_str);
		while (*str_ptr=='|') { strcat(new_str,"|"); str_ptr++; }
		//strcat(new_str,"|");
		s2=strtok(NULL,"|");
	}
	free(s1);
	sprintf(s,"%s",new_str);
	return s;
}

MODULE = Locale::Hebrew		PACKAGE = Locale::Hebrew		


SV *
hebrewflip(s)
SV * s
CODE:
    int l;
    char *src, *dst;
    SV *r;

    src = SvPV(s, l);
    src[l] = '\0';
    dst = hebrew_str(src);
    r = newSVpv(dst, strlen(dst));
    RETVAL = r;
    OUTPUT:
    RETVAL
    
