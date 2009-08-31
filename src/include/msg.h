/**
 *  ____    __    ____  __    __    ______     ______   .______    __  
 *  \   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 *   \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
 *    \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
 *     \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
 *      \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)
 *
 * @file msg.h
 * @brief whoop msg header file
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

#ifndef MSG_H_
#define MSG_H_

#include "config.h"

#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#else
#define LOG_EMERG       0       /* system is unusable */
#define LOG_ALERT       1       /* action must be taken immediately */
#define LOG_CRIT        2       /* critical conditions */
#define LOG_ERR         3       /* error conditions */
#define LOG_WARNING     4       /* warning conditions */
#define LOG_NOTICE      5       /* normal but significant condition */
#define LOG_INFO        6       /* informational */
#define LOG_DEBUG       7       /* debug-level messages */
#endif /* HAVE_SYSLOG_H */

#ifdef MSG_NAMES

typedef struct
{
	char *c_name;
	int c_val;
} msg_code_t;

msg_code_t msg_prioritynames[] =
{
#ifdef LOG_EMERG
	{ "emerg", LOG_EMERG,      },
#endif
#ifdef LOG_ALERT
	{ "alert", LOG_ALERT,      },
#endif
#ifdef LOG_CRIT
	{ "crit ", LOG_CRIT,       },
#endif
#ifdef LOG_ERR
	{ "error", LOG_ERR,        },
#endif
#ifdef LOG_WARNING
	{ "warn ", LOG_WARNING,    },
#endif
#ifdef LOG_NOTICE
	{ "note ", LOG_NOTICE,     },
#endif
#ifdef LOG_INFO
	{ "info ", LOG_INFO,       },
#endif
#ifdef LOG_DEBUG
	{ "debug", LOG_DEBUG,      },
#endif
	{ NULL, -1,                }
};

#endif /* MSG_NAMES */

typedef enum
{
	MSG_NONE,
#ifdef HAVE_SYSLOG_H
	MSG_SYSLOG,
#endif
	MSG_FILE,
	MSG_TERM,
} msg_dest_t;

extern int msg_open(msg_dest_t dest, const char *target, int flag, int facility);
extern void msg_close(void);
extern void msg_log(int loglevel, const char *format, ...);

#endif /* MSG_H_ */

