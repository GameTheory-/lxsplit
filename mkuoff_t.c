/*
 * $Id: mkuoff_t.c,v 1.1 2008/02/02 08:03:55 sezero Exp $
 * Generates uoff_t.h which defines the unsigned version of off_t
 * as uoff_t, defines SIZEOF_<type> macros for the types we use.
 *
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
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

int main (void)
{
	FILE	*f;
	int	ret = 0;

	f = fopen ("uoff_t.h", "w");
	if (f == NULL)
	{
		ret = 1;
		fprintf (stderr, "Error: cannot create uoff_t.h\n");
	}
	else
	{
		fprintf (f, "/* Generated by mkuoff_t.c */\n\n");
		fprintf (f, "#ifndef _UOFF_T_H\n");
		fprintf (f, "#define _UOFF_T_H\n\n");

		fprintf (f, "#ifndef SIZEOF_CHAR\n");
		fprintf (f, "#define SIZEOF_CHAR %d\n", (int)sizeof(char));
		fprintf (f, "#endif\n\n");

		fprintf (f, "#ifndef SIZEOF_SHORT\n");
		fprintf (f, "#define SIZEOF_SHORT %d\n", (int)sizeof(short));
		fprintf (f, "#endif\n\n");

		fprintf (f, "#ifndef SIZEOF_INT\n");
		fprintf (f, "#define SIZEOF_INT %d\n", (int)sizeof(int));
		fprintf (f, "#endif\n\n");

		fprintf (f, "#ifndef SIZEOF_LONG\n");
		fprintf (f, "#define SIZEOF_LONG %d\n", (int)sizeof(long));
		fprintf (f, "#endif\n\n");

		fprintf (f, "#ifndef SIZEOF_LONG_LONG\n");
		fprintf (f, "#define SIZEOF_LONG_LONG %d\n", (int)sizeof(long long));
		fprintf (f, "#endif\n\n");

		fprintf (f, "#ifndef SIZEOF_VOID_P\n");
		fprintf (f, "#define SIZEOF_VOID_P %d\n", (int)sizeof(void *));
		fprintf (f, "#endif\n\n");

		fprintf (f, "#ifndef SIZEOF_SIZE_T\n");
		fprintf (f, "#define SIZEOF_SIZE_T %d\n", (int)sizeof(size_t));
		fprintf (f, "#endif\n\n");

		fprintf (f, "#ifndef SIZEOF_OFF_T\n");
		fprintf (f, "#define SIZEOF_OFF_T %d\n", (int)sizeof(off_t));
		fprintf (f, "#endif\n\n");

		if (sizeof(off_t) == sizeof(uint32_t))
		{
			fprintf (f, "#define SIZEOF_UOFF_T %d\n", (int)sizeof(uint32_t));
			fprintf (f, "typedef uint32_t uoff_t;\n\n");
		}
		else if (sizeof(off_t) == sizeof(uint64_t))
		{
			fprintf (f, "#define SIZEOF_UOFF_T %d\n", (int)sizeof(uint64_t));
			fprintf (f, "typedef uint64_t uoff_t;\n\n");
		}
		else if (sizeof(off_t) == sizeof(uintmax_t))
		{
			fprintf (f, "#define SIZEOF_UOFF_T %d\n", (int)sizeof(uintmax_t));
			fprintf (f, "typedef uintmax_t uoff_t;\n\n");
		}
		else
		{
			fprintf (f, "#error Cannot define uoff_t\n\n");
			fprintf (stderr, "Error: cannot define uoff_t\n");
			ret = 1;
		}

		fprintf (f, "#endif\t/* _UOFF_T_H */\n\n");
		fclose (f);
	}

	exit (ret);
}

