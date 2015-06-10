/*
 * $Id: config.h,v 1.10 2008/10/03 12:05:24 sezero Exp $
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

#ifndef __CONFIG_H
#define __CONFIG_H

/* version macros:	*/
#define	LXSPLIT_VER_MAJ		0
#define	LXSPLIT_VER_MID		2
#define	LXSPLIT_VER_MIN		4


#include <limits.h>

#if defined(__cplusplus)
#define __STDC_CONSTANT_MACROS	1
#define __STDC_FORMAT_MACROS	1
#endif	/* __cplusplus */

#include <stdint.h>
#include <inttypes.h>

#undef	USE_LFS_SUPPORT

/* type sizes and 'unsigned off_t': */
#include "uoff_t.h"

#if !defined(PRIuLEAST32)
#   define PRIuLEAST32	"u"
#endif

#if !defined(PRIuLEAST64)
# if (SIZEOF_LONG < 8)
#   define PRIuLEAST64	"llu"
# else
#   define PRIuLEAST64	"lu"
# endif
#endif

#if !defined(UINT64_C)
# if (SIZEOF_LONG < 8)
#   define UINT64_C(c)	c ## ULL
# else 
#   define UINT64_C(c)	c ## UL
# endif
#endif

#if (SIZEOF_UOFF_T < 8)	/* No LFS : */
#   define _STRTOULL		strtoul
#   define _PRI_ULL_		PRIuLEAST32
    typedef unsigned long int	strtou_t;
#else	/* Use LFS : */
#   define USE_LFS_SUPPORT	1
#   define _PRI_ULL_		PRIuLEAST64
# if (SIZEOF_LONG < 8)
#   define _STRTOULL		strtoumax
    typedef uoff_t		strtou_t;
# else
#   define _STRTOULL		strtoul
    typedef unsigned long int	strtou_t;
# endif
#endif

/* maximum size for split files: */
#if !defined(USE_LFS_SUPPORT)
#define	MAX_SPLITSIZE		(2147483647U)		/* 2 GB - 1  ==  (1U << 31) - 1 */
#else
#define	MAX_SPLITSIZE		(UINT64_C(1) << 33)	/* 8 GB == 8589934592 bytes. shall I even need this much? */
#endif

/* read buffer size	*/
#define	READ_BUFSIZE		4096U

#endif	/* __CONFIG_H */

