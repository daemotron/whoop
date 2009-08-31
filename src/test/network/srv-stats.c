/**
 *  ____    __    ____  __    __    ______     ______   .______    __  
 *  \   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 *   \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
 *    \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
 *     \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
 *      \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)                           
 *
 * @file handle-client.c
 * @brief whoop test server client handler
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

#include <sys/times.h>

#include "msg.h"
#include "network.h"
#include "config.h"

#include "_testnw.h"

struct tms start_acct;

void
init_srv_stats(void)
{
	times(&start_acct);
	return;
}

void
print_srv_stats(void)
{
	struct tms end_acct;

	times(&end_acct);

	msg_log
	(
		LOG_INFO, "CPU-Info: %d\n", 
		(int)(end_acct.tms_utime - start_acct.tms_utime) +
		(int)(end_acct.tms_stime - start_acct.tms_stime) +
		(int)(end_acct.tms_cutime - start_acct.tms_cutime) +
		(int)(end_acct.tms_cstime - start_acct.tms_cstime)
	);

	return;
}

#if 0
____    __    ____  __    __    ______     ______   .______    __  
\   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
  \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
   \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
    \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)                           

#endif

