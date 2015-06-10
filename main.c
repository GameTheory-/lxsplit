/*
 * $Id: main.c,v 1.5 2008/10/03 12:03:13 sezero Exp $
 * This file is a part of the lxSplit tool sources.
 *
 * Copyright (c) 2001 Richard Stellingwerff <remenic@gmail.com>
 * Copyright (c) 2008 O. Sezer <sezero@users.sourceforge.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to:
 *
 * Free Software Foundation, Inc.
 * 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "config.h"
#include "split.h"
#include "merge.h"


static const char authors[] = "Richard Stellingwerff, O. Sezer";
static const char website[] = "http://lxsplit.sourceforge.net/";

static void print_help (const char *cmd)
{
	printf ("LXSplit v%d.%d.%d by %s.\nHome page: %s\n\n",
		LXSPLIT_VER_MAJ, LXSPLIT_VER_MID, LXSPLIT_VER_MIN,
		authors, website);

	printf ("Usage: %s [OPTION] [FILE] [SPLITSIZE]\n\n", cmd);

	printf ("Available options:\n"
		" -j : join the files beginning with the given name\n"
		" -s : split the given file.  requires a valid size\n"
		"Splitsize examples: 15M, 100m, 5000k, 30000000b\n\n");

	printf ("Examples:\n"
		"\t%s -s hugefile.bin 15M\n"
		"\t%s -j hugefile.bin.001\n\n",
		cmd, cmd);
}

int main (int argc, char **argv)
{
	char			*filename,
				*usersize;
	int			ret;
	strtou_t		size;
	size_t			len;


	if (argc < 3)
	{
		print_help(argv[0]);
		exit (1);
	}

	if (!strcmp(argv[1], "-s"))
	{
		if (argc != 4)
		{
			print_help(argv[0]);
			exit (1);
		}

		if (argv[3][0] == '-')
		{
			printf("Split size cannot be a negative number!\n\n");
			exit (1);
		}

		len = strlen(argv[3]);
		usersize = (char *)malloc((len + 1) * sizeof(char));
		strncpy(usersize, argv[3], len);
		if ((argv[3][len - 1] == 'M') || (argv[3][len - 1] == 'm'))
		{
			usersize[len - 1] = '\0';
			size = _STRTOULL(usersize, NULL, 10);
			size *= 1024 * 1024;
		}
		else if ((argv[3][len - 1] == 'K') || (argv[3][len - 1] == 'k'))
		{
			usersize[len - 1] = '\0';
			size = _STRTOULL(usersize, NULL, 10);
			size *= 1024;
		}
		else if ((argv[3][len - 1] == 'B' || argv[3][len - 1] == 'b'))
		{
			usersize[len - 1] = '\0';
			size = _STRTOULL(usersize, NULL, 10);
		}
		else if (isdigit(argv[3][len - 1]))
		{
			size = _STRTOULL(usersize, NULL, 10);
		}
		else
		{
			printf("Please specify the size correctly!\n\n");
			print_help(argv[0]);
			exit (1);
		}
		/*
		printf ("size == %" _PRI_ULL_ "\n", size);
		*/
		if (size == 0)
		{
			printf("Invalid split size (0). Did you enter a very large number?\n\n");
			exit (1);
		}
		if (size > MAX_SPLITSIZE)
		{
			printf("Split size can be %" _PRI_ULL_ " bytes at maximum!\n\n",
								(uoff_t)MAX_SPLITSIZE);
			exit (1);
		}

		filename = strdup(argv[2]);
		ret = split_file(filename, size);
		exit (ret);
	}
	else if (!strcmp(argv[1], "-j"))
	{
		if (argc != 3)
		{
			print_help(argv[0]);
			exit (1);
		}

		filename = strdup(argv[2]);
		ret = merge_files(filename);
		exit (ret);
	}
	else
	{
		printf("Whatcha wanna do?\n");
		print_help(argv[0]);
		exit (1);
	}

	exit (0);
}

