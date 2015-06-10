/*
 * $Id: func.h,v 1.6 2008/07/02 17:15:56 sezero Exp $
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

#ifndef __FUNC_H
#define __FUNC_H

#if 0
uintmax_t available_diskspace (const char *path);
#endif
char *strip_path (const char *filename);
char *shorten_path (const char *cfilename);

#endif	/* __FUNC_H */

