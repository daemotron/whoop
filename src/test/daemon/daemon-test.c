/**
 *  ____    __    ____  __    __    ______     ______   .______    __  
 *  \   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 *   \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
 *    \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
 *     \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
 *      \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)                           
 *
 * @file daemon-test.c
 * @brief whoop daemon library test tool
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
#include <syslog.h>

#include "daemon.h"
#include "config.h"

void
show_usage(int argc, char * const *argv)
{
	char *progname;

	if (argc >= 1)
		progname = basename(argv[0]);
	else
		progname = "daemon-test";

	printf
	(
		"usage: %s [-h] [-s <delay>] [-p <pidfile>]\n"
		"\n"
		"   -h             show this help and exit\n"
		"   -p <pidfile>   write daemon PID to the specified file\n"
		"   -s <delay>     let the test daemon run for <delay> seconds\n"
		"\n"
		"Please report any issues like bugs etc. to <jesco.freund@my-universe.com>\n",
		progname
	);
}


int 
main(int argc, char **argv)
{
	int c;
	char *pidfile = NULL;
	char cwd[1024] = { '\0' };
	int delay = 0;

	while ((c = getopt(argc, argv, "hp:s:")) != -1)
	{
		switch(c)
		{
			case 'h':
				show_usage(argc, argv);
				return(EXIT_SUCCESS);
				break;
			case 'p':
				if ((NULL != optarg) && (strlen(optarg) > 0) && (optarg[0] != '/'))
				{
					getcwd(cwd, 1024);
					pidfile = (char *)malloc(strlen(cwd) + strlen(optarg) + 2);
					strncpy(pidfile, cwd, strlen(cwd) + 1);
					strncat(pidfile, "/", 2);
					strncat(pidfile, optarg, strlen(optarg) + 1);
				}
				else
				{
					pidfile = optarg;
				}
				break;
			case 's':
				if (NULL != optarg)
					delay = atoi(optarg);
		}
	}


	openlog(basename(argv[0]), LOG_PID, LOG_LOCAL0);
	syslog(LOG_DEBUG, "Started with original PID %d.", (int)getpid());

	if (NULL != pidfile)
		syslog(LOG_DEBUG, "Daemonizing with PID file %s", pidfile);
	else
		syslog(LOG_DEBUG, "Daemonizing without PID file.");

	daemon_init(basename(argv[0]), LOG_LOCAL0, pidfile);

	syslog(LOG_DEBUG, "Daemonized with daemon PID %d.", (int)getpid());

	sleep(delay);

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
