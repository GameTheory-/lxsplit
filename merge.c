/*
 * $Id: merge.c,v 1.9 2008/02/02 08:03:55 sezero Exp $
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

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "config.h"
#include "func.h"
#include "merge.h"


int merge_files (const char *filename)
{
	FILE			*fp_split,
				*fp_merged;

	size_t			len, bytes_read;
	uoff_t			merged_size,
				split_size,
				split_pos;

	int			filenr,
				amount,
				startnr;

	char			*merged_fname,
				*split_fname,
				*orig_fname,
				*missing_fname,
				*c_bytes,
				*split_sfname;

	struct stat		buf;


	len = strlen(filename);
	if (len < 5 || filename[len - 4] != '.')
	{
		printf("Erroneous filename. Please supply the extension.\n"
			"Must end with .000 or .001\n");
		return 1;
	}

	switch (filename[len - 1])
	{
	case '0':
		startnr = 0;
		break;
	case '1':
		startnr = 1;
		break;
	default:
		printf("Erroneous filename. Please supply the extension.\n"
			"Must end with .000 or .001\n");
		return 1;
	}

	orig_fname = (char *)malloc((len - 4 + 1) * sizeof(char));
	split_fname = (char *)malloc((len + 1) * sizeof(char));

	strncpy(orig_fname, filename, len - 4);
	orig_fname[len - 4] = '\0';

	merged_fname = strip_path(orig_fname);

	amount = 0;
	merged_size = 0;
	missing_fname = NULL;

	for (filenr = startnr; filenr < 1000; filenr++)
	{
		if (filenr < 10)
			sprintf(split_fname, "%s.00%d", orig_fname, filenr);
		else if (filenr < 100)
			sprintf(split_fname, "%s.0%d", orig_fname, filenr);
		else /* if (filenr < 1000) 999 is hardcoded above already. */
			sprintf(split_fname, "%s.%d", orig_fname, filenr);

		if (stat(split_fname, &buf) == -1)
		{
			if (filenr == startnr)
			{
				printf("Can't find `%s'\n", split_fname);
				return 1;
			}
			else
			{
				if (missing_fname == NULL)
					missing_fname = strdup(split_fname);
			}
		}
		else
		{
			if (!S_ISREG(buf.st_mode))
			{
				printf("Can't find `%s'\n", split_fname);
				return 1;
			}
			else
			{
				if (missing_fname != NULL)
				{
					printf("Missing file `%s' !\n", missing_fname);
					return 1;
				}
				merged_size += buf.st_size;
				amount++;
			}
		}
	}

	printf("Creating merged file `%s'.\nComplete size: %" _PRI_ULL_ " in %d files.\n",
							merged_fname, merged_size, amount);

	if (stat(merged_fname, &buf) == 0)
	{
		printf("%s already exists! Aborting...\n", merged_fname);
		return 1;
	}

	fp_merged = fopen(merged_fname, "w");
	if (fp_merged == NULL)
	{
		perror("cannot create output file");
		return 1;
	}

	c_bytes = (char *)malloc(READ_BUFSIZE * sizeof(char));

	for (filenr = startnr; filenr <= amount + (startnr - 1); filenr++)
	{
		if (filenr < 10)
			sprintf(split_fname, "%s.00%d", orig_fname, filenr);
		else if (filenr < 100)
			sprintf(split_fname, "%s.0%d", orig_fname, filenr);
		else if (filenr < 1000)
			sprintf(split_fname, "%s.%d", orig_fname, filenr);

		split_sfname = shorten_path(split_fname);
		printf("Processing file `%s' ...\n", split_sfname);

		if (stat(split_fname, &buf) == -1)
		{
			perror("cannot stat input file");
			goto _err_in;
		}

		split_size = buf.st_size;

		fp_split = fopen(split_fname, "r");
		if (fp_split == NULL)
		{
			perror("cannot open input file");
			goto _err_in;
		}

		split_pos = 0;

		while (split_pos < split_size)
		{
			bytes_read = fread(c_bytes, 1, READ_BUFSIZE, fp_split);
			if (bytes_read != READ_BUFSIZE && !feof(fp_split))
			{
				perror("error while reading input file");
				goto _err_io;
			}
			len = fwrite(c_bytes, 1, bytes_read, fp_merged);
			if (len != bytes_read)
			{
				perror("error while writing to output file");
				goto _err_io;
			}
			split_pos += bytes_read;
		}

		fclose(fp_split);
		if (split_sfname != split_fname)
			free (split_sfname);
	}

	printf("Done!\n");
	fclose(fp_merged);
	return 0;

_err_io:
	fclose(fp_split);
_err_in:
	fclose(fp_merged);
	return 1;
}

