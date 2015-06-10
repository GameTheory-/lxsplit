/*
 * $Id: split.c,v 1.11 2008/10/03 12:04:23 sezero Exp $
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
#include "split.h"


int split_file (const char *filename, uoff_t _s)
{
	FILE		*fp_orig,
			*fp_split;

	uoff_t		orig_pos, orig_size,
			split_pos, split_size,
			bytes_left, piece_bytes;
	size_t		bytes_read, allow_read,
			len;

	int		filenr,
			pieces;

	struct stat	buf;
	char		*c_byte,
			*split_fname,
			*orig_fname,
			*split_sfname;


	orig_pos = orig_size = 0;
	split_pos = split_size = 0;
	filenr = 1;

	if (stat(filename, &buf) == -1)
	{
		perror("cannot stat input file");
		return 1;
	}
	if (!S_ISREG(buf.st_mode))
	{
		printf("%s is not a regular file\n", filename);
		return 1;
	}
	orig_size = buf.st_size;

	if (_s >= orig_size)
	{
		printf("Splitted filesize is same as size of the filename!\n"
			"Try using cp(1) instead...\n");
		return 1;
	}

	fp_orig = fopen(filename, "r");
	if (fp_orig == NULL)
	{
		perror("error opening input file");
		return -1;
	}

	split_size = _s;
	bytes_left = orig_size;

	pieces = orig_size / split_size;
	if ((orig_size % split_size) > 0)
		pieces++;

	if (pieces > 999)
	{
		printf("You will be ending up with over %d files, while I"
			"rather wouldn't go over the 999... Perhaps you"
			"could choose a bigger output size?\n", pieces);
		return 1;
	}

	orig_fname = strip_path(filename);
	len = strlen(orig_fname) + 5;	/* ".001" + null terminator */
	split_fname = (char *) malloc(len * sizeof(char));
	c_byte = (char *) malloc(READ_BUFSIZE * sizeof(char));

	printf("Splitting %s into %d pieces.\n", orig_fname, pieces);

	while (orig_pos < orig_size)
	{
		if (filenr < 10)
			sprintf(split_fname, "%s.00%d", orig_fname, filenr);
		else if (filenr < 100)
			sprintf(split_fname, "%s.0%d", orig_fname, filenr);
		else /* if (filenr < 1000) 999 is hardcoded above already. */
			sprintf(split_fname, "%s.%d", orig_fname, filenr);

		fp_split = fopen(split_fname, "w");
		if (fp_split == NULL)
		{
			perror("cannot create output file");
			goto _err_of;
		}

		split_pos = 0;
		split_sfname = shorten_path(split_fname);

		printf("%s\t\t\t%" _PRI_ULL_ " bytes\n", split_sfname, split_size);
		bytes_read = 0;
		allow_read = READ_BUFSIZE;

		while (split_pos < split_size)
		{
			piece_bytes = split_size - split_pos;			/* how much to read */
			if (piece_bytes < (uoff_t)allow_read)
				allow_read = (size_t)piece_bytes;

			bytes_read = fread(c_byte, 1, allow_read, fp_orig);	/* how much is read */
			if (bytes_read != allow_read)
			{
				if (feof(fp_orig))
					printf("unexpected EOF!...\n");
				perror("error while reading input file");
				goto _err_io;
			}
			len = fwrite(c_byte, 1, bytes_read, fp_split);
			if (len != bytes_read)
			{
				perror("error while writing to output file");
				goto _err_io;
			}

			orig_pos += bytes_read;
			split_pos += bytes_read;
#ifdef DEBUG
			printf("split pos :\t%" _PRI_ULL_ "\n", split_pos);
			printf("split size:\t%" _PRI_ULL_ "\n", split_size);
			printf("allow_read:\t%lu\n", (unsigned long)allow_read);
			sleep (1);
#endif
		}

		bytes_left -= split_size;
		if (bytes_left < split_size)
			split_size = bytes_left;

		filenr++;

		fclose(fp_split);
		if (split_sfname != split_fname)
			free (split_sfname);
	}

	printf("Done!\n");
	fclose(fp_orig);
	return 0;

_err_io:
	fclose(fp_split);
_err_of:
	fclose(fp_orig);
	return 1;
}

