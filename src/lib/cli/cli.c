/**
 *  ____    __    ____  __    __    ______     ______   .______    __  
 *  \   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 *   \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
 *    \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
 *     \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
 *      \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)                           
 *
 * @file cli.c
 * @brief whoop CLI library functions
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

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <unistd.h>

#include "_cli.h"
#include "config.h"

extern cmd_t
_cli_get_cmd(const char *cmd)
{
	if (strncmp(cmd, "new", ((strlen(cmd) > strlen("new")) ? strlen(cmd) : strlen("new")) + 1) == 0)
		return CMD_NEW;
	else if (strncmp(cmd, "init", ((strlen(cmd) > strlen("init")) ? strlen(cmd) : strlen("init")) + 1) == 0)
		return CMD_INIT;
	else if (strncmp(cmd, "in", ((strlen(cmd) > strlen("in")) ? strlen(cmd) : strlen("in")) + 1) == 0)
		return CMD_INIT;
	else if (strncmp(cmd, "help", ((strlen(cmd) > strlen("help")) ? strlen(cmd) : strlen("help")) + 1) == 0)
		return CMD_HELP;
	else if (strncmp(cmd, "--help", ((strlen(cmd) > strlen("--help")) ? strlen(cmd) : strlen("--help")) + 1) == 0)
		return CMD_HELP;
	else if (strncmp(cmd, "-h", ((strlen(cmd) > strlen("-h")) ? strlen(cmd) : strlen("-h")) + 1) == 0)
		return CMD_HELP;
	else if (strncmp(cmd, "version", ((strlen(cmd) > strlen("version")) ? strlen(cmd) : strlen("version")) + 1) == 0)
		return CMD_VERSION;
	else if (strncmp(cmd, "--version", ((strlen(cmd) > strlen("--version")) ? strlen(cmd) : strlen("--version")) + 1) == 0)
		return CMD_VERSION;
	else if (strncmp(cmd, "-v", ((strlen(cmd) > strlen("-v")) ? strlen(cmd) : strlen("-v")) + 1) == 0)
		return CMD_VERSION;
	else if (strncmp(cmd, "whoop", ((strlen(cmd) > strlen("whoop")) ? strlen(cmd) : strlen("whoop")) + 1) == 0)
		return CMD_WHOOP;
	else
		return CMD_ILLEGAL;
}

extern int
cli_run_command(int argc, char * const *argv)
{
	cmd_t command = CMD_ILLEGAL;

	if (argc >= 2)
		command = _cli_get_cmd(argv[1]);

	if (command)
	{
		switch(command)
		{
			case CMD_NEW:
				if (_cli_cmd_new(argc, argv))
					return 1;
				else
					return 0;
				break;
			case CMD_VERSION:
				if (_cli_cmd_version(argc, argv))
					return 1;
				else
					return 0;
				break;
			case CMD_HELP:
				if (_cli_cmd_help(argc, argv))
					return 1;
				else
					return 0;
				break;
			case CMD_WHOOP:
				printf(WHOOP "\nI can do it louder than you...\n");
				return 1;
				break;
			default:
				(void)_cli_cmd_help(argc, argv);
				return 0;
		}
	}
	else
	{
		(void)_cli_cmd_help(argc, argv);
		return 0;
	}
	return 1;
}

