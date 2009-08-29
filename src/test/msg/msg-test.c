/**
 *  ____    __    ____  __    __    ______     ______   .______    __  
 *  \   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 *   \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
 *    \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
 *     \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
 *      \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)                           
 *
 * @file msg-test.c
 * @brief whoop msg library test tool
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

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <unistd.h>

#include "msg.h"
#include "config.h"


void
show_usage(int argc, char * const *argv)
{
	char *progname;

	if (argc >= 1)
		progname = basename(argv[0]);
	else
		progname = "msg-test";

	printf
	(
		"usage: %s [-h] [-f <logfile>]\n"
		"\n"
		"   -h             show this help and exit\n"
		"   -f <logfile>   write test log to specified file\n"
		"\n"
		"Please report any issues like bugs etc. to <jesco.freund@my-universe.com>\n",
		progname
	);
}

int 
main(int argc, char **argv)
{
	int i = 1;
	int c;
	char *logfile = NULL;
	char cwd[1024] = { '\0' };

	while ((c = getopt(argc, argv, "hf:")) != -1)
	{
		switch (c)
		{
			case 'h':
				show_usage(argc, argv);
				return(EXIT_SUCCESS);
				break;
			case 'f':
				if ((NULL != optarg) && (strlen(optarg) >0) && (optarg[0] != '/'))
				{
					getcwd(cwd, 1024);
					logfile = (char *)malloc(strlen(cwd) + strlen(optarg) + 2);
					strncpy(logfile, cwd, strlen(cwd) + 1);
					strncat(logfile, "/", 2);
					strncat(logfile, optarg, strlen(optarg) + 1);
				}
				else
				{
					logfile = optarg;
				}
				break;
		}
	}

	/* TEST 01: Terminal */
	msg_open(MSG_TERM, NULL, 0, 0);
	msg_log(LOG_DEBUG, "Test no. %d: Terminal (debug)\n", i);
	msg_log(LOG_ERR, "Test no. %d: Terminal (error)\n", i);
	msg_close();

	i++;

	/* TEST 02: File */
	msg_open(MSG_FILE, logfile, 0, 0);
	msg_log(LOG_DEBUG, "Test no. %d: File (debug)\n", i);
	msg_log(LOG_ERR, "Test no. %d: File (error)\n", i);
	msg_close();

	i++;

	/* TEST 03: Syslog */
	msg_open(MSG_SYSLOG, "logtest", LOG_PID, LOG_LOCAL0);
	msg_log(LOG_DEBUG, "Test no. %d: Syslog (debug)\n", i);
	msg_log(LOG_ERR, "Test no. %d: Syslog (error)\n", i);
	msg_close();

	return(EXIT_SUCCESS);
}

#if 0
____    __    ____  __    __    ______     ______   .______    __  
\   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
  \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
   \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
    \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)                           

#endif

