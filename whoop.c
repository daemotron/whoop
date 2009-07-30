/**
 *  ____    __    ____  __    __    ______     ______   .______    __  
 *  \   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 *   \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
 *    \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
 *     \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
 *      \__/  \__/     |__|  |__|  \______/   \______/  | _|      (__)                           
 *
 * @file whoop.c
 * @brief whoop CLI tool
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

#include "whoop.h"

void
show_usage(const char *progname)
{
    printf("usage: %s <command> [command-specific arguments]\n"
           "\n"
           "\thelp\tprint this help\n"
           "\t\tif followed  by a command, print help for this command\n"
           "\tnew\tcreate a new project\n"
           "\tversion\tshow version information\n", 
           progname);
}

void
show_version(const char *progname)
{

}

cmd_t
get_cmd(const char *cmd)
{
    if (strncmp(cmd, "new", ((strlen(cmd) > strlen("new")) ? strlen(cmd) : strlen("new")) + 1) == 0)
        return CMD_NEW;
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

int 
main(int argc, char **argv)
{
    cmd_t command = CMD_ILLEGAL;
    
    if (argc >= 2)
        command = get_cmd(argv[1]);
    
    if (command)
    {
        switch(command)
        {
            case CMD_WHOOP:
                printf(WHOOP "\nI can do it louder than you...\n");
                return EXIT_SUCCESS;
                break;
            default:
                printf("I don't know how to do %s\n", argv[1]);
        }
    }
    else
    {
        show_usage(basename(argv[0]));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

#if 0
____    __    ____  __    __    ______     ______   .______    __  
\   \  /  \  /   / |  |  |  |  /  __  \   /  __  \  |   _  \  |  | 
 \   \/    \/   /  |  |__|  | |  |  |  | |  |  |  | |  |_)  | |  | 
  \            /   |   __   | |  |  |  | |  |  |  | |   ___/  |  | 
   \    /\    /    |  |  |  | |  `--'  | |  `--'  | |  |      |__| 
    \__/  \__/     |__|  |__|  \______/   \______/  | _|      (__)                           

#endif
