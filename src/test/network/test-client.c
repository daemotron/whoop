/**
 *  ____    __    ____  __    __    ______     ______   .______    __  
 *  \   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 *   \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
 *    \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
 *     \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
 *      \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)                           
 *
 * @file test-client.c
 * @brief whoop test client and load generator
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
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>

#include "barrier.h"
#include "msg.h"
#include "network.h"
#include "config.h"

#include "_testnw.h"


/* XXX
 * I know those globals are ugly, but as it's 
 * for testing purposes only...
 */
char *srv_host;
char *srv_port;
int num_reqs, num_work, num_bytes;
barrier_t barrier;

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
		"   -b <bytes>     expected data size\n"
		"   -h             show this help and exit\n"
		"   -l <logfile>   write test log to specified file\n"
		"   -p <port>      server-side tcp port\n"
		"   -r <reqs>      number of requests per thread\n"
		"   -s <server>    ip address or hostname of the server\n"
		"   -u <limit>     upper limit for server-side test calculation\n"
		"   -w <worker>    number of worker threads (client side)\n"
		"\n"
		"Please report any issues like bugs etc. to <jesco.freund@my-universe.com>\n",
		progname
	);
}

void *
worker(void *arg)
{
	int srv, i, status, tid = (int)arg;
	char req[MAXLINE], hello[MAXLINE], *buffer;
	struct tms acct;
	clock_t *rtime, t;
	void *rl = NULL;

	/* prepare request statement */
	snprintf(req, MAXLINE -1, "%d %d\n", num_work, num_bytes);

	/* prepare buffer for result */
	buffer = malloc(num_bytes);

	/* prepare time stamp */
	rtime = calloc(num_reqs, sizeof(clock_t));

	if ((NULL == rtime) || (NULL == buffer))
	{
		msg_log(LOG_ERR, "Thread %02d: malloc() failed.\n", tid);
		exit(EXIT_FAILURE);
	}

	/* wait until all threads are up and running */
	status = barrier_wait(&barrier);

	/* send requests to server */
	for (i = 0; i < num_reqs; i++)
	{
		t = times(&acct);

		/* create tcp connection */
		if (0 > (srv = network_tcp_connect(srv_host, srv_port)))
		{
			msg_log(LOG_ERR, "Thread %02d: tcp_connect failed.\n", tid);
			exit(EXIT_FAILURE);
		}

		/* read greeting from server */
		if (1 > network_readline(srv, hello, MAXLINE, &rl))
		{
			msg_log(LOG_ERR, "Thread %02d: No server greeting.\n", tid);
			exit(EXIT_FAILURE);
		}

		/* calculate time for establishing the connection */
		rtime[i] = times(&acct) - t;

		/* Send request to server */
		if (network_writen(srv, req, strlen(req)) != strlen(req))
		{
			msg_log(LOG_ERR, "Thread %02d: writen() failed.\n", tid);
			exit(EXIT_FAILURE);
		}

		/* read answer from server */
		if (network_readn(srv, buffer, num_bytes) != num_bytes)
		{
			msg_log(LOG_ERR, "Thread %02d: readn() failed.\n", tid);
			exit(EXIT_FAILURE);
		}

		/* do some housekeeping */
		free(rl);
		rl = NULL;
		close(srv);
	}

	/* record statistics */
	for (i = 0; i < num_reqs; i++)
		msg_log(LOG_INFO, "Thread %02d: %d clockticks\n", tid, rtime[i]);

	/* wait until all threads have finished */
	status = barrier_wait(&barrier);

	return(NULL);
}

int 
main(int argc, char **argv)
{
	int c, i, num_threads, status;
	char *logfile = NULL;
	char cwd[1024] = { '\0' };
	pthread_t id;

	/* some default settings */
	num_threads = 1;
	num_bytes = 0;
	num_reqs = 0;
	num_work = 0;
	srv_host = NULL;
	srv_port = NULL;

	while ((c = getopt(argc, argv, "b:hl:p:r:s:u:w:")) != -1)
	{
		switch (c)
		{
			case 'b':
				if (NULL != optarg)
					num_bytes = atoi(optarg);
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
			case 'r':
				if (NULL != optarg)
					num_reqs = atoi(optarg);
				break;
			case 's':
				if (NULL != optarg)
					srv_host = optarg;
				break;
			case 'u':
				if (NULL != optarg)
					num_work = atoi(optarg);
				break;
			case 'w':
				if (NULL != optarg)
					num_threads = atoi(optarg);
				break;
		}
	}

	if (NULL == logfile)
		msg_open(MSG_TERM, NULL, 0, 0);
	else
		msg_open(MSG_FILE, logfile, 0, 0);

	if (num_threads <= 0)
	{
		msg_log(LOG_ERR, "Invalid number of workers: %d\n", num_threads);
		exit(EXIT_FAILURE);
	}

	if (num_reqs <= 0)
	{
		msg_log(LOG_ERR, "Invalid number of requests per thread: %d\n", num_reqs);
		exit(EXIT_FAILURE);
	}

	if (num_bytes <= 0)
	{
		msg_log(LOG_ERR, "Invalid number of bytes: %d\n", num_bytes);
		exit(EXIT_FAILURE);
	}

	if (num_work <= 0)
	{
		msg_log(LOG_ERR, "Invalid upper limit: %d\n", num_work);
		exit(EXIT_FAILURE);
	}

	if (NULL == srv_host)
	{
		msg_log(LOG_ERR, "No server specified.\n");
		exit(EXIT_FAILURE);
	}

	if (NULL == srv_port)
	{
		msg_log(LOG_ERR, "Invalid server-side tcp port\n");
		exit(EXIT_FAILURE);
	}

	/* init barrier */
	status = barrier_init(&barrier, num_threads);
	if (0 != status)
	{
		msg_log(LOG_ERR, "Could not initialise barrier: %s\n", strerror(status));
		exit(EXIT_FAILURE);
	}

	/* start worker threads (one less than specified) */
	for (i = 1; i < num_threads; i++)
	{
		status = pthread_create(&id, NULL, worker, (void *)i);
		if (0 != status)
		{
			msg_log(LOG_ERR, "Failed to create thread: %s\n", strerror(status));
			exit(EXIT_FAILURE);
		}
		pthread_detach(id);
	}

	/* I (the main thread) make the last (or first) worker */
	worker((void *)0);

	/* do housekeeping stuff */
	barrier_destroy(&barrier);
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

