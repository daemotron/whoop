/**
 *  ____    __    ____  __    __    ______     ______   .______    __  
 *  \   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 *   \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
 *    \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
 *     \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
 *      \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)                           
 *
 * @file readline.c
 * @brief whoop network library readline implementation
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
#include <stdlib.h>

#include "_network.h"
#include "config.h"

extern ssize_t
network_readline(int filedesc, void *buf, size_t nbyte, void **help)
{
	size_t n;
	ssize_t br;
	char c = 0;
	char *ptr = buf;
	readline_t *rl = *help;

	if (NULL == rl)
	{
		if (NULL == (rl = malloc(sizeof(readline_t))))
			return(-1);
		rl->count = 0;
		rl->current = rl->buf;
		*help = rl;
	}

	for (n = 1; n < nbyte; n++)
	{
		if (0 > (br = __network_readcbuf(filedesc, &c, rl)))
			return(-1);

		*ptr++ = c;

		if ((0 == br) || ('\n' == c))
			break;
	}

	if ((0 == br) && (1 == n))
		return(0);

	*ptr = 0;
	return(n);
}


ssize_t
__network_readcbuf(int filedesc, char *buf, readline_t *rl)
{
	while (rl->count < 1)
	{
		if (0 > (rl->count = read(filedesc, rl->buf, sizeof(rl->buf))))
		{
			if (EINTR == errno)
				rl->count = 0;
			else
				return(-1);
		}
		else if (0 == rl->count)
			return(0);

		rl->current = rl->buf;
	}

	*buf = *rl->current++;
	rl->count--;

	return(1);
}

