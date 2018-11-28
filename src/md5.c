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
 * History:
 *
 *	1.0	Initial version.
 *
 */

#pragma	strings(readonly)

#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>

#include "md5.h"

/* Definitions */

#define	VERSION			1	/* Major version */
#define	EDIT			0	/* Edit number within major version */

#define TEST_BLOCK_LEN 10000		/* Length of test block */
#define TEST_BLOCK_COUNT 100000		/* Number of test blocks */

/* Forward references */

static	VOID	error(PUCHAR, ...);
static	VOID	MDString(PUCHAR, BOOL, BOOL);
static	VOID	MDTimeTrial(VOID);
static	VOID	MDTestSuite(BOOL, BOOL);
static	VOID	MDFilter(BOOL);
static	VOID	putusage(VOID);

/* Local storage */

static	PUCHAR	progname;		/* Name of program, as a string */
static	BOOL	qflag = FALSE;
static	BOOL	rflag = FALSE;

/* Help text */

static	const	PUCHAR helpinfo[] = {
"%s: MD5 digest utility",
"Synopsis: %s [-pqrtx] [-s string] [file ...]",
" Options:",
"    -h           display this help",
"    -p           echo input to output, and append the digest value",
"    -q           quiet mode; generate only the MD5 digest. Overrides -r",
"    -r           reverse the format of the output; does nothing when",
"                 combined with the -ptx options",
"    -s string    generate a digest of string",
"    -t           run a built-in time trial",
"    -x           run a built-in test script; -q and -r must precede this",
"                 option to be effective",
" ",
"The program generates a 128-bit fingerprint (or 'digest') of a string",
"or file. If no file or string is specified, standard input is used.",
""
};


/*
 * Parse arguments and handle options.
 *
 */

INT main(INT argc, PUCHAR argv[])
{	UINT ch, i;
	UINT files = 0;
	PUCHAR p, argp;
	UCHAR buf[33];

	progname = strrchr(argv[0], '\\');
	if(progname != (PUCHAR) NULL)
		progname++;
	else
		progname = argv[0];
	p = strchr(progname, '.');
	if(p != (PUCHAR) NULL) *p = '\0';
	strlwr(progname);

	/* Process input options */

	for(i = 1; i < argc; i++) {
		argp = argv[i];
		if(argp[0] == '-') {		/* Option */
			switch(argp[1]) {
				case 'h':	/* Display help */
					putusage();
					files++;
					break;

				case 'p':	/* Copy and append */
					MDFilter(TRUE);
					files++;
					break;

				case 'q':	/* Quiet mode */
					qflag = TRUE;
					break;

				case 'r':	/* Reverse format */
					rflag = TRUE;
					break;

				case 's':	/* String */
					if(argp[2] != '\0') {
						MDString(&argp[2], qflag, rflag);
					} else {
						if(i == argc - 1) {
							error("no arg for -s");
							exit(EXIT_FAILURE);
						} else {
							MDString(
								argv[++i],
								qflag,
								rflag);
						}
					}
					files++;
					break;

				case 't':	/* Time trial */
					MDTimeTrial();
					files++;
					break;

				case 'x':	/* Test script */
					MDTestSuite(qflag, rflag);
					files++;
					break;

				case '\0':
					error("missing flag after '-'");
					exit(EXIT_FAILURE);

				default:
					error("invalid flag '%c'", argp[1]);
					exit(EXIT_FAILURE);
			}
		} else {
			files++;
			p = MD5File(argp, buf);
			if(!p) {
				error("warning: %s", argp);
			} else {
				if (qflag)
					printf("%s\n", p);
				else if (rflag)
					printf("%s %s\n", p, argp);
				else
					printf("MD5 (%s) = %s\n", argp, p);
			}
		}
	}

	if(files == 0)			/* No files - use standard input */
		MDFilter(FALSE);

	return(EXIT_SUCCESS);
}


/*
 * Digest a string and print the result.
 *
 */

static VOID MDString(PUCHAR string, BOOL qflag, BOOL rflag)
{	size_t len = strlen(string);
	UCHAR buf[33];

	if(qflag == TRUE) {
		fprintf(stdout, "%s\n", MD5Data(string, len, buf));
	} else {
		if(rflag == TRUE) {
			fprintf(
				stdout,
				"%s \"%s\"\n",
				MD5Data(string, len, buf),
				string);
		} else {
				fprintf(
					stdout,
					"MD5 (\"%s\") = %s\n",
					string,
					MD5Data(string, len, buf));
		}
	}
}


/*
 * Measure the time to digest TEST_BLOCK_COUNT TEST_BLOCK_LEN-byte blocks.
 *
 */

static VOID MDTimeTrial(VOID)
{	MD5_CTX context;
	time_t endTime, startTime, runTime;
	UCHAR block[TEST_BLOCK_LEN];
	UINT i;
	PUCHAR p;
	UCHAR buf[33];

	fprintf(
		stdout,
		"MD5 time trial. Digesting %d %d-byte blocks ...",
		TEST_BLOCK_COUNT,
		TEST_BLOCK_LEN);
	fflush(stdout);

	/* Initialize block */

	for(i = 0; i < TEST_BLOCK_LEN; i++)
		block[i] = (UCHAR) (i & 0xff);

	/* Start timer */

	time(&startTime);

	/* Digest blocks */

	MD5Init(&context);
	for(i = 0; i < TEST_BLOCK_COUNT; i++)
		MD5Update(&context, block, TEST_BLOCK_LEN);
	p = MD5End(&context,buf);

	/* Stop timer */

	time(&endTime);
	runTime = endTime - startTime;

	fputs(" done\n", stdout);
	fprintf(stdout, "Digest = %s", p);
	fprintf(stdout, "\nTime = %ld seconds\n", (LONG) runTime);

	fprintf(
		stdout,
		"Speed = %ld bytes/second\n",
		(LONG) TEST_BLOCK_LEN * (LONG) TEST_BLOCK_COUNT /
		(runTime != 0 ? runTime : 1));
}


/*
 * Digest a reference suite of strings and print the results.
 *
 */

static VOID MDTestSuite(BOOL qflag, BOOL rflag)
{	fputs("MD5 test suite:\n", stdout);

	MDString("", qflag, rflag);
	MDString("a", qflag, rflag);
	MDString("abc", qflag, rflag);
	MDString("message digest", qflag, rflag);
	MDString("abcdefghijklmnopqrstuvwxyz", qflag, rflag);
	MDString(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
		"0123456789",
		qflag,
		rflag);
	MDString(
		"1234567890123456789012345678901234567890"
		"1234567890123456789012345678901234567890",
		qflag,
		rflag);
}


/*
 * Digest the standard input and print the result.
 *
 */

static VOID MDFilter(BOOL pipe)
{	MD5_CTX context;
	UINT len;
	UCHAR buffer[BUFSIZ];
	UCHAR buf[33];

	MD5Init(&context);
	(VOID) freopen("", "rb", stdin);	/* Need binary mode */
	(VOID) freopen("", "wb", stdout);	/* Need binary mode */
	while((len = fread(buffer, 1, BUFSIZ, stdin)) != 0) {
		if((pipe == TRUE) && (len != fwrite(buffer, 1, len, stdout))) {
			error("error writing to standard output");
			exit(EXIT_FAILURE);
	}
		MD5Update(&context, buffer, len);
	}
	(VOID) freopen("", "w", stdout);
	fprintf(stdout, "%s\n", MD5End(&context,buf));
}


/*
 * Print message on standard error in printf style,
 * accompanied by program name.
 *
 */

static VOID error(PUCHAR mes, ...)
{	va_list ap;

	fprintf(stderr, "%s: ", progname);

	va_start(ap, mes);
	vfprintf(stderr, mes, ap);
	va_end(ap);

	fputc('\n', stderr);
}


/*
 * Output program usage information.
 *
 */

static VOID putusage(VOID)
{	PUCHAR *p = (PUCHAR *) helpinfo;
	PUCHAR q;

	for(;;) {
		q = *p++;
		if(*q == '\0') break;

		fprintf(stderr, q, progname);
		fputc('\n', stderr);
	}
	fprintf(stderr, "\nThis is version %d.%d\n", VERSION, EDIT);
}

/*
 * End of file: md5.c
 *
 */
