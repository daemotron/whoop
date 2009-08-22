/**
 *  ____    __    ____  __    __    ______     ______   .______    __  
 *  \   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 *   \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
 *    \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
 *     \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
 *      \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)                           
 *
 * @file daemon.c
 * @brief whoop daemon library functions
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
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <string.h>
#include <sys/stat.h>

#include "_daemon.h"
#include "config.h"

extern void
daemon_init(const char *program, int facility, const char *pidfile)
{
	pid_t pid;
	int i;
	FILE *pid_fp;
	int sigs[] = { SIGHUP, SIGINT, SIGQUIT, SIGTSTP, SIGTTIN, SIGTTOU };

	/* make sure all those nice signals are going to be ignored */
	for (i = 0; i < sizeof(sigs) / sizeof(int); i++)
	{
		if (!_daemon_handle_signal(sigs[i], SIG_IGN))
		{
			fprintf(stderr, "%s: Fatal error in daemon_init(): %s\n", program, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	/* stalking off to background... */
	switch (pid = fork())
	{
		case -1:
			fprintf(stderr, "%s: Fatal error in fork(): %s\n", program, strerror(errno));
			exit(EXIT_FAILURE);
			break;
		case 0:
			openlog(program, LOG_PID, facility);
			break;
		default:
			exit(EXIT_SUCCESS);
			break;
	}

	/* get rid of the associated terminal */
	if (setsid() < 0)
	{
		syslog(LOG_ERR, "Fatal error in setsid(): %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* make sure we cannot be prosecuted by a not yet controlling terminal */
	switch (pid = fork())
	{
		case -1:
			syslog(LOG_ERR, "Fatal error in fork(): %s\n", strerror(errno));
			exit(EXIT_FAILURE);
			break;
		case 0:
			break;
		default:
			exit(EXIT_SUCCESS);
			break;
	}

	/* close unnecessary file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	/* change working directory */
	chdir("/");

	/* set default file mode creation mask */
	umask(0);

	/* write PID to pidfile */
	if (NULL != pidfile)
	{
		pid_fp = fopen(pidfile, "w");
		if (NULL == pid_fp)
		{
			syslog(LOG_ERR, "Fatal error in fopen(): %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		fprintf(pid_fp, "%d", (int)getpid());
		fclose(pid_fp);
	}
}

