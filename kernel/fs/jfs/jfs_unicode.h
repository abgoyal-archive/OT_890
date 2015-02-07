/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/*
 *   Copyright (C) International Business Machines Corp., 2000-2002
 *   Portions Copyright (C) Christoph Hellwig, 2001-2002
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#ifndef _H_JFS_UNICODE
#define _H_JFS_UNICODE

#include <asm/byteorder.h>
#include "jfs_types.h"

typedef struct {
	wchar_t start;
	wchar_t end;
	signed char *table;
} UNICASERANGE;

extern signed char UniUpperTable[512];
extern UNICASERANGE UniUpperRange[];
extern int get_UCSname(struct component_name *, struct dentry *);
extern int jfs_strfromUCS_le(char *, const __le16 *, int, struct nls_table *);

#define free_UCSname(COMP) kfree((COMP)->name)

/*
 * UniStrcpy:  Copy a string
 */
static inline wchar_t *UniStrcpy(wchar_t * ucs1, const wchar_t * ucs2)
{
	wchar_t *anchor = ucs1;	/* save the start of result string */

	while ((*ucs1++ = *ucs2++));
	return anchor;
}



/*
 * UniStrncpy:  Copy length limited string with pad
 */
static inline __le16 *UniStrncpy_le(__le16 * ucs1, const __le16 * ucs2,
				  size_t n)
{
	__le16 *anchor = ucs1;

	while (n-- && *ucs2)	/* Copy the strings */
		*ucs1++ = *ucs2++;

	n++;
	while (n--)		/* Pad with nulls */
		*ucs1++ = 0;
	return anchor;
}

/*
 * UniStrncmp_le:  Compare length limited string - native to little-endian
 */
static inline int UniStrncmp_le(const wchar_t * ucs1, const __le16 * ucs2,
				size_t n)
{
	if (!n)
		return 0;	/* Null strings are equal */
	while ((*ucs1 == __le16_to_cpu(*ucs2)) && *ucs1 && --n) {
		ucs1++;
		ucs2++;
	}
	return (int) *ucs1 - (int) __le16_to_cpu(*ucs2);
}

/*
 * UniStrncpy_to_le:  Copy length limited string with pad to little-endian
 */
static inline __le16 *UniStrncpy_to_le(__le16 * ucs1, const wchar_t * ucs2,
				       size_t n)
{
	__le16 *anchor = ucs1;

	while (n-- && *ucs2)	/* Copy the strings */
		*ucs1++ = cpu_to_le16(*ucs2++);

	n++;
	while (n--)		/* Pad with nulls */
		*ucs1++ = 0;
	return anchor;
}

/*
 * UniStrncpy_from_le:  Copy length limited string with pad from little-endian
 */
static inline wchar_t *UniStrncpy_from_le(wchar_t * ucs1, const __le16 * ucs2,
					  size_t n)
{
	wchar_t *anchor = ucs1;

	while (n-- && *ucs2)	/* Copy the strings */
		*ucs1++ = __le16_to_cpu(*ucs2++);

	n++;
	while (n--)		/* Pad with nulls */
		*ucs1++ = 0;
	return anchor;
}

/*
 * UniToupper:  Convert a unicode character to upper case
 */
static inline wchar_t UniToupper(wchar_t uc)
{
	UNICASERANGE *rp;

	if (uc < sizeof(UniUpperTable)) {	/* Latin characters */
		return uc + UniUpperTable[uc];	/* Use base tables */
	} else {
		rp = UniUpperRange;	/* Use range tables */
		while (rp->start) {
			if (uc < rp->start)	/* Before start of range */
				return uc;	/* Uppercase = input */
			if (uc <= rp->end)	/* In range */
				return uc + rp->table[uc - rp->start];
			rp++;	/* Try next range */
		}
	}
	return uc;		/* Past last range */
}


/*
 * UniStrupr:  Upper case a unicode string
 */
static inline wchar_t *UniStrupr(wchar_t * upin)
{
	wchar_t *up;

	up = upin;
	while (*up) {		/* For all characters */
		*up = UniToupper(*up);
		up++;
	}
	return upin;		/* Return input pointer */
}

#endif				/* !_H_JFS_UNICODE */
