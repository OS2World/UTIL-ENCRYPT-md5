/*
 * File: md5hl.c
 *
 * MD5 digest utility for OS/2
 *
 * Miscellaneous routines
 *
 * Bob Eager   October 2000
 *
 */

/*
 * Derived from work by the below:
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <phk@login.dkuug.dk> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 *
 */

#include <os2.h>

#include <sys/types.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "md5.h"

#define	LENGTH	16


PUCHAR MD5End(PMD5_CTX ctx, PUCHAR buf)
{	UINT i;
	UCHAR digest[LENGTH];
	static const UCHAR hex[]="0123456789abcdef";

	if(buf == (PUCHAR) NULL)
		buf = malloc(2*LENGTH + 1);
	if(buf == (PUCHAR) NULL)
		return((PUCHAR) NULL);

	MD5Final(digest, ctx);
	for(i = 0; i < LENGTH; i++) {
		buf[i+i] = hex[digest[i] >> 4];
		buf[i+i+1] = hex[digest[i] & 0x0f];
	}
	buf[i+i] = '\0';
	return(buf);
}


PUCHAR MD5File(const PUCHAR filename, PUCHAR buf)
{	UCHAR buffer[BUFSIZ];
	MD5_CTX ctx;
	UINT j;
	INT i, f;

	MD5Init(&ctx);
	f = open(filename, O_RDONLY | O_BINARY);
	if(f < 0) return((PUCHAR) NULL);

	while((i = read(f, buffer, sizeof(buffer))) > 0) {
		MD5Update(&ctx,buffer,i);
	}
	j = errno;
	close(f);
	errno = j;
	if(i < 0) return((PUCHAR) NULL);

	return(MD5End(&ctx, buf));
}


PUCHAR MD5Data (const PUCHAR data, UINT len, PUCHAR buf)
{	MD5_CTX ctx;

	MD5Init(&ctx);
	MD5Update(&ctx,data,len);

	return(MD5End(&ctx, buf));
}

/*
 * End of file: md5hl.c
 *
 */

