/**
 *  ____    __    ____  __    __    ______     ______   .______    __  
 *  \   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 *   \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
 *    \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
 *     \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
 *      \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)                           
 *
 * @file writen.c
 * @brief whoop network library writen implementation
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

#include "_network.h"
#include "config.h"

extern ssize_t
network_writen(int filedesc, const void *buf, size_t nbyte)
{
	size_t n;
	ssize_t bw;
	char *ptr = (char *)buf;

	for (n = nbyte; n > 0; n -= bw, ptr += bw)
	{
		if (0 > (bw = write(filedesc, ptr, n)))
		{
			if (EINTR == errno)
				bw = 0;
			else
				return(-1);
		}
		else if (0 == bw)
			return(nbyte - n);
	}
	
	return nbyte;
}

