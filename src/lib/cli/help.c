/**
 *  ____    __    ____  __    __    ______     ______   .______    __  
 *  \   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 *   \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
 *    \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
 *     \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
 *      \__/  \__/     |__|  |__|  \______/   \______/  |__|      (__)                           
 *
 * @file help.c
 * @brief whoop CLI help command
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

extern int
_cli_cmd_help(int argc, char * const *argv)
{
	cmd_t help_cmd;
	char *progname;

	if (argc >= 1)
		progname = basename(argv[0]);
	else
		progname = "whoop";
	
	if (argc > 2)
	{
		help_cmd = _cli_get_cmd(argv[2]);
		switch (help_cmd)
		{
			case CMD_NEW:
				printf
				(
					"new: creates a new whoop project.\n"
					"usage: new PROJECTNAME\n\n"
					"  This will create a new directory within the current working\n"
					"  directory named PROJECTNAME (therefore you should only use\n"
					"  characters in PROJECTNAME that are allowed with your file system).\n\n"
					"  Please note that chosing a PROJECTNAME identical to an already existing\n"
					"  directory name within the current working directory will inevitably\n"
					"  provoke a fatal error.\n\n"
					"  Inside the freshly created directory you will find the basic framework\n"
					"  of a Whoop application readily waiting for you to complete its code.\n\n"
				);
				break;
			case CMD_INIT:
				printf
				(
					"init (in): initialise a project inside the current work directory.\n"
					"usage: init [PROJECTNAME]\n\n"
					"  Other than the 'new' command, the init command will not create a new\n"
					"  directory for the project, but create the project structure within\n"
					"  the current working directory.\n\n"
					"  If no PROJECTNAME is specified, the base name of the current working\n"
					"  directory will be used as project name.\n\n"
				);
				break;
			default:
				printf("'%s': unknown command\n", argv[2]);
				return 0;
				break;
		}
	}
	else
	{
		printf
		(
			"usage: %s [--version] [--help] COMMAND [ARGS]\n"
			"\n"
			"Type '%s help COMMAND' for help on a specific command.\n"
			"\n"
			"The most commonly used %s commands are:\n"
			"   init       initialise a project in current directory\n"
			"   new        create a new project\n"
			"   version    show version information\n"
			"\n"
			"Please report any issues like bugs etc. to <jesco.freund@my-universe.com>\n",
			progname, progname, progname
		);
	}
	return 1;
}

