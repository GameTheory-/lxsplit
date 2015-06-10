/*
 * $Id: func.c,v 1.7 2008/07/02 17:15:56 sezero Exp $
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
#include <stdio.h>

#include <sys/stat.h>
#include <limits.h>
#if 0
/* statfs icludes: */
#if (defined(__APPLE__) && defined(__MACH__))	/* Mac OS X */	||	\
    defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
#include <sys/param.h>
#include <sys/mount.h>
#else	/* BSD */
#include <sys/vfs.h>
#endif	/* Linux, etc. */
#endif

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "config.h"
#include "func.h"


#if 0
uintmax_t available_diskspace (const char *path)
{
	struct statfs	buf;
	uintmax_t	space;

	if (statfs(path, &buf) < 0)
		return 0;
	space = buf.f_bsize;
	space *= buf.f_bavail;
	return space;
}
#endif

char *strip_path (const char *cfilename)
{
	size_t			lpos,
				i, n, len;
	char			*filename;


	lpos = 0;
	len = strlen(cfilename);
	for (i = 0; i < len; i++)
		if (cfilename[i] == '/')
			lpos = i;

	if (lpos != 0)
		lpos++;

	n = 0;

	filename = (char *)malloc((len + 2) * sizeof(char));

	for (i = lpos; i < len; i++)
	{
		filename[n] = cfilename[i];
		n++;
	}

	filename[n] = '\0';

	return filename;
}

char *shorten_path (const char *cfilename)
{
	char		*filename;
	size_t		i, n, len;

	len = strlen(cfilename);
	if (len < 30)
		return (char *)cfilename;

	filename = (char *)malloc((30 + 1) * sizeof(char));

	n = 0;

	for (i = 0; i < 10; i++)
	{
		filename[n] = cfilename[i];
		n++;
	}

	for (i = 0; i < 3; i++)
	{
		filename[n] = '.';
		n++;
	}

	for (i = len - 17; i < len; i++)
	{
		filename[n] = cfilename[i];
		n++;
	}

	filename[30] = '\0';

	return filename;
}

