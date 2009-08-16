/**
 *  ____    __    ____  __    __    ______     ______   .______    __  
 *  \   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 *   \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
 *    \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
 *     \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
 *      \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)
 *
 * @file _cli.h
 * @brief local CLI library header file
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

#ifndef _CLI_H_
#define _CLI_H_

#include "cli.h"

#define WHOOP "____    __    ____  __    __    ______     ______   .______    __ \n"\
              "\\   \\  /  \\  /   / |  |  |  |  /  __  \\   /  __  \\  |   _  \\  |  |\n"\
              " \\   \\/    \\/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  |\n"\
              "  \\            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  |\n"\
              "   \\    /\\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__|\n"\
              "    \\__/  \\__/     |__|  |__|  \\______/   \\______/  |__|      (__)\n"\


typedef enum
{
	CMD_ILLEGAL,    /* must remain in the first place so it keeps assigned to zero */
	CMD_NEW,
	CMD_INIT,
	CMD_HELP,
	CMD_VERSION,
	CMD_WHOOP,
} cmd_t;

extern int _cli_cmd_help(int argc, char * const *argv);
extern int _cli_cmd_new(int argc, char * const *argv);
extern int _cli_cmd_init(int argc, char * const *argv);
extern int _cli_cmd_version(int argc, char * const *argv);
extern cmd_t _cli_get_command(const char *cmd);

#endif /* _CLI_H_ */
