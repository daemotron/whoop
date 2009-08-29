/**
 *  ____    __    ____  __    __    ______     ______   .______    __  
 *  \   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 *   \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
 *    \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
 *     \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
 *      \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)                           
 *
 * @file barrier_wait.c
 * @brief whoop barrier library barrier_wait implementation
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

#include "_barrier.h"
#include "config.h"

extern int
barrier_wait(barrier_t *barrier)
{
	int status, cycle;

	status = pthread_mutex_lock(&barrier->mutex);
	if (0 != status)
		return status;

	cycle = barrier->cycle;

	if (0 == --barrier->counter)
	{
		barrier->cycle++;
		barrier->counter = barrier->threshold;

		status = pthread_cond_broadcast(&barrier->cv);
		if (0 == status)
			status = BARRIER_SERIAL_THREAD;
	}
	else
	{
		while (cycle == barrier->cycle)
		{
			status = pthread_cond_wait(&barrier->cv, &barrier->mutex);
			if (0 != status)
				break;
		}
	}

	pthread_mutex_unlock(&barrier->mutex);
	return(status);
}

