/*
 * File: md5.c
 *
 * MD5 digest utility for OS/2
 *
 * Bob Eager   October 2000
 *
 */

/*
 * Parts of this program are:
 *
 *  Copyright (C) 1990-2, RSA Data Security, Inc. Created 1990. All
 *  rights reserved.
 *
 *  RSA Data Security, Inc. makes no representations concerning either
 *  the merchantability of this software or the suitability of this
 *  software for any particular purpose. It is provided "as is"
 *  without express or implied warranty of any kind.
 *
 *  These notices must be retained in any copies of any part of this
 *  documentation and/or software.
 */

/*
 * Parts of this program are:
 *
 * Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
 * rights reserved.
 * 
 * License to copy and use this software is granted provided that it
 * is identified as the "RSA Data Security, Inc. MD5 Message-Digest
 * Algorithm" in all material mentioning or referencing this software
 * or this function.
 * 
 * License is also granted to make and use derivative works provided
 * that such works are identified as "derived from the RSA Data
 * Security, Inc. MD5 Message-Digest Algorithm" in all material
 * mentioning or referencing the derived work.
 * 
 * RSA Data Security, Inc. makes no representations concerning either
 * the merchantability of this software or the suitability of this
 * software for any particular purpose. It is provided "as is"
 * without express or implied warranty of any kind.
 * 
 * These notices must be retained in any copies of any part of this
 * documentation and/or software.
 */

/* MD5 context */

typedef struct MD5Context {
UINT	state[4];	/* state (ABCD) */
UINT	count[2];	/* number of bits, modulo 2^64 (lsb first) */
UCHAR	buffer[64];	/* input buffer */
} MD5_CTX, *PMD5_CTX;

extern	VOID	MD5Init(PMD5_CTX);
extern	VOID	MD5Update(PMD5_CTX, const PUCHAR, UINT);
extern	VOID	MD5Pad(PMD5_CTX);
extern	VOID	MD5Final(UCHAR[16], PMD5_CTX);
extern	PUCHAR	MD5End(PMD5_CTX, PUCHAR);
extern	PUCHAR	MD5File(const PUCHAR, PUCHAR);
extern	PUCHAR	MD5Data(const PUCHAR, UINT, PUCHAR);

/*
 * End of file: md5.h
 *
 */

