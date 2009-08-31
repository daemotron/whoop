/**
 *  ____    __    ____  __    __    ______     ______   .______    __  
 *  \   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 *   \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
 *    \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
 *     \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
 *      \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)                           
 *
 * @file msg.c
 * @brief whoop msg library functions
 * 
 * @copyright
 * ====================================================================
 *
 * Copyright (c) 2009
 *   Jesco Freund <jesco.freund@my-universe.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * ====================================================================
 * @endcopyright
 *
 * @version $Id$
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "_msg.h"
#include "config.h"

FILE *__msg_fptr;
msg_dest_t __msg_dest;

char *
__msg_loglevel(int loglevel)
{
	msg_code_t *n = msg_prioritynames;
	while ((n->c_name) && (n->c_val != loglevel))
		n++;

	return (char *)n->c_name;
}

char *
__msg_strmcap(const char *string)
{
	char *result = NULL;
	int i;

	result = (char *)malloc(strlen(string) + 1);
	if (NULL == result)
	{
		fprintf(stderr, "Could not allocate memory: %s\n", strerror(errno));
		return NULL;
	}

	memset(result, '\0', strlen(string) + 1);

	for (i = 0; i < (int)strlen(string); i++)
	{
		if ((string[i] >= 97) && (string[i] <= 122))
			result[i] = string[i] - 32;
		else
			result[i] = string[i];
	}
	return result;
}

extern int
msg_open(msg_dest_t dest, const char *target, int flag, int facility)
{
	switch (dest)
	{
#ifdef HAVE_SYSLOG_H
		case MSG_SYSLOG:
			openlog(target, flag, facility);
			__msg_dest = dest;
			break;
#endif
		case MSG_FILE:
			if (__msg_fptr && (__msg_fptr != stderr) && (__msg_fptr != stdout))
				fclose(__msg_fptr);
			__msg_fptr = fopen(target, "a");
			if (NULL == __msg_fptr)
			{
				fprintf(stderr, "Failed to open logfile: %s\n", strerror(errno));
				__msg_dest = MSG_NONE;
				return 0;
			}
			setvbuf(__msg_fptr, NULL, _IONBF, 0);
			__msg_dest = dest;
			break;
		case MSG_TERM:
			__msg_dest = dest;
			break;
		default:
			return 0;
			break;
	}
	return 1;
}

extern void
msg_close(void)
{
	switch(__msg_dest)
	{
#ifdef HAVE_SYSLOG_H
		case MSG_SYSLOG:
			closelog();
			__msg_dest = MSG_NONE;
			break;
#endif
		case MSG_FILE:
			if (__msg_fptr && (__msg_fptr != stderr) && (__msg_fptr != stdout))
				fclose(__msg_fptr);
			__msg_dest = MSG_NONE;
			break;
		case MSG_TERM:
			__msg_dest = MSG_NONE;
			break;
		default:
			break;
	}
}

extern void
msg_log(int loglevel, const char *format, ...)
{
	va_list vargptr;
	char level[16] = { '\0' };
	char *message = NULL;
	time_t tstmp;
	struct tm *nicetime;
	char tbuf[21] = { '\0' };

	va_start(vargptr, format);
	switch (__msg_dest)
	{
#ifdef HAVE_SYSLOG_H
		case MSG_SYSLOG:
			vsyslog(loglevel, format, vargptr);
			break;
#endif
		case MSG_FILE:
			time(&tstmp);
			nicetime = localtime(&tstmp);
			strftime(tbuf, 21, "%Y-%m-%d %H:%M:%S ", nicetime);
			snprintf(level, 15, "[%s] ", __msg_strmcap(__msg_loglevel(loglevel)));
			message = (char *)malloc(strlen(tbuf) + strlen(level) + strlen(format) + 1);
			memset(message, '\0', strlen(tbuf) + strlen(level) + strlen(format) + 1);
			strncpy(message, tbuf, strlen(tbuf) + 1);
			strncat(message, level, strlen(level) + 1);
			strncat(message, format, strlen(format) + 1);
			vfprintf(__msg_fptr, message, vargptr);
			fflush(__msg_fptr);
			break;
		case MSG_TERM:
			snprintf(level, 15, "[%s] ", __msg_strmcap(__msg_loglevel(loglevel)));
			message = (char *)malloc(strlen(level) + strlen(format) + 1);
			strncpy(message, level, strlen(level)+1);
			strncat(message, format, strlen(format) + 1);
			if (loglevel > 3)
				vfprintf(stdout, message, vargptr);
			else
				vfprintf(stderr, message, vargptr);
			break;
		default:
			break;
	}
	va_end(vargptr);
}

