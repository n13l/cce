#ifndef __BSD_STRING_H__
#define __BSD_STRING_H__

#include <ctype.h>

static inline int
strnchr(char *token,char s)
{
	if (!token || s=='\0')
		return 0;

	for (;*token; token++)
		if (*token == s)
			return 1;

	return 0;
}

static inline void
strrm_inplace(char *str, const char *c)
{
	char *src = str, *dst = str;
	while(*src)
		if(strnchr(c, *src))
			src++;
		else
			*dst++ = *src++;

	*dst = 0;
}

#endif
