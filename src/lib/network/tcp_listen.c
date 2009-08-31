/**
 *  ____    __    ____  __    __    ______     ______   .______    __  
 *  \   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 *   \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
 *    \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
 *     \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
 *      \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)                           
 *
 * @file tcp_listen.c
 * @brief whoop network library tcp_listen implementation
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
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "_network.h"
#include "config.h"

extern int
network_tcp_listen(const char *nodename, const char *servname, int backlog)
{
	int sd = 0;
	int reuseaddr = 0;
	int status = 0;
	struct addrinfo hints, *ai, *aptr;

	/* init hints address structure */
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;		/* works with both, IPv4 and IPv6 */
	hints.ai_socktype = SOCK_STREAM;	/* the function's called _tcp_... - ain't it? */

	/* determine address structure for passive socket */
	if (0 == (status = getaddrinfo(nodename, servname, &hints, &ai)))
	{
		for (aptr = ai; NULL != aptr; aptr = aptr->ai_next)
		{
			if (0 > (sd = socket(aptr->ai_family, aptr->ai_socktype, aptr->ai_protocol)))
				continue;	/* in case of error try next address structure */

			/* try to avoid "address already in use" errors */
			reuseaddr = 1;
			setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int));

			/* finally, bind socket */
			if (0 == (bind(sd, aptr->ai_addr, aptr->ai_addrlen)))
				/* convert socket from active to passive */
				if (0 <= (listen(sd, backlog)))
					break;	/* if everything's fine, get out of the loop */

			/* in case of error, close the socket */
			close(sd);
		}

		freeaddrinfo(ai);

		/* check for errors */
		if (NULL == aptr)
		{
			msg_log(LOG_ERR, "Can't listen on port %s: %s\n", servname, strerror(errno));
			return(-1);
		}
	}
	else
	{
		msg_log(LOG_ERR, "getaddrinfo() failed: %s\n", gai_strerror(status));
		return(-1);
	}

	return(sd);
}

