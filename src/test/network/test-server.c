/**
 *  ____    __    ____  __    __    ______     ______   .______    __  
 *  \   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 *   \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
 *    \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
 *     \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
 *      \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)                           
 *
 * @file test-server.c
 * @brief whoop test server
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
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "barrier.h"
#include "daemon.h"
#include "msg.h"
#include "network.h"
#include "config.h"

#include "_testnw.h"



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
		"usage: %s [-h] [-f <pidfile] [-l <logfile>] [-p <port>] [-w <threads>]\n"
		"\n"
		"   -h             show this help and exit\n"
		"   -f <pidfile>   write server PID to specified file\n"
		"   -l <logfile>   write test log to specified file\n"
		"   -p <port>      listen on specified tcp port\n"
		"   -w <worker>    number of worker threads\n"
		"\n"
		"Please report any issues like bugs etc. to <jesco.freund@my-universe.com>\n",
		progname
	);
}

void
sigcatcher(void)
{
	int signal = 0;
	sigset_t sigset;

	sigemptyset(&sigset);
	sigaddset(&sigset, SIGTERM);

	for (;;)
	{
		sigwait(&sigset, &signal);
		if (SIGTERM == signal)
			break;
	}

	return;
}

void *
accept_handler(void *arg)
{
	int client = 0, sd = (int)arg;
	socklen_t slen;
	struct sockaddr_storage sa;

	for (;;)
	{
		slen = sizeof(sa);

		/* accept new connection */
		if (0 > (client = accept(sd, (struct sockaddr *)&sa, &slen)))
		{
			msg_log(LOG_ERR, "Failed to accept connection: %s\n", strerror(errno));
			continue;
		}

		/* handle client request */
		handle_client(client);

		close(client);
	}

	return(NULL);
}

int 
main(int argc, char **argv)
{
	int i = 0, c = 0, sd = 0, status = 0, num_threads = 1;
	char *logfile = NULL;
	char *pidfile = NULL;
	char *srv_port = NULL;
	char cwd[1024] = { '\0' };
	sigset_t sigset;
	pthread_t tid;

	while ((c = getopt(argc, argv, "f:hl:p:w:")) != -1)
	{
		switch (c)
		{
			case 'f':
				if ((NULL != optarg) && (strlen(optarg) >0) && (optarg[0] != '/'))
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
			case 'h':
				show_usage(argc, argv);
				return(EXIT_SUCCESS);
				break;
			case 'l':
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
			case 'p':
				if (NULL != optarg)
					srv_port = optarg;
				break;
			case 'w':
				if (NULL != optarg)
					num_threads = atoi(optarg);
				break;
		}
	}

	msg_open(MSG_TERM, NULL, 0, 0);

	if (num_threads <= 0)
	{
		msg_log(LOG_ERR, "Invalid number of workers: %d\n", num_threads);
		exit(EXIT_FAILURE);
	}

	if (NULL == srv_port)
	{
		msg_log(LOG_ERR, "No tcp port specified\n");
		exit(EXIT_FAILURE);
	}

	/* create listening socket (passive) */
	if (0 > (sd = network_tcp_listen(NULL, srv_port, BACKLOG)))
	{
		msg_log(LOG_ERR, "Failed to bind socket: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* initialise signal mask */
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGTERM);

	/* set signal mask for main thread */
	status = pthread_sigmask(SIG_BLOCK, &sigset, NULL);
	if (0 != status)
	{
		msg_log(LOG_ERR, "Failed to set signal mask: %s\n", strerror(status));
		close(sd);
		exit(EXIT_FAILURE);
	}

	/* daemonize */
	if (NULL == logfile)
		daemon_init(MSG_SYSLOG, basename(argv[0]), LOG_LOCAL0, pidfile);
	else
		daemon_init(MSG_FILE, logfile, 0, pidfile);

	init_srv_stats();

	/* create worker thread pool */
	for (i = 0; i < num_threads; i++)
	{
		status = pthread_create(&tid, NULL, accept_handler, (void *)sd);
		if (0 != status)
		{
			msg_log(LOG_ERR, "Failed to create thread: %s\n", strerror(status));
			close(sd);
			if (NULL != pidfile)
				unlink(pidfile);
			exit(EXIT_FAILURE);
		}
		pthread_detach(tid);
	}

	/* wait for SIGTERM */
	sigcatcher();

	print_srv_stats();

	if (NULL != pidfile)
		unlink(pidfile);

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

