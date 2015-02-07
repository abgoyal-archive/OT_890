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

/* p80211meta.h
*
* Macros, constants, types, and funcs for p80211 metadata
*
* Copyright (C) 1999 AbsoluteValue Systems, Inc.  All Rights Reserved.
* --------------------------------------------------------------------
*
* linux-wlan
*
*   The contents of this file are subject to the Mozilla Public
*   License Version 1.1 (the "License"); you may not use this file
*   except in compliance with the License. You may obtain a copy of
*   the License at http://www.mozilla.org/MPL/
*
*   Software distributed under the License is distributed on an "AS
*   IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
*   implied. See the License for the specific language governing
*   rights and limitations under the License.
*
*   Alternatively, the contents of this file may be used under the
*   terms of the GNU Public License version 2 (the "GPL"), in which
*   case the provisions of the GPL are applicable instead of the
*   above.  If you wish to allow the use of your version of this file
*   only under the terms of the GPL and not to allow others to use
*   your version of this file under the MPL, indicate your decision
*   by deleting the provisions above and replace them with the notice
*   and other provisions required by the GPL.  If you do not delete
*   the provisions above, a recipient may use your version of this
*   file under either the MPL or the GPL.
*
* --------------------------------------------------------------------
*
* Inquiries regarding the linux-wlan Open Source project can be
* made directly to:
*
* AbsoluteValue Systems Inc.
* info@linux-wlan.com
* http://www.linux-wlan.com
*
* --------------------------------------------------------------------
*
* Portions of the development of this software were funded by
* Intersil Corporation as part of PRISM(R) chipset product development.
*
* --------------------------------------------------------------------
*
* This file declares some of the constants and types used in various
* parts of the linux-wlan system.
*
* Notes:
*   - Constant values are always in HOST byte order.
*
* All functions and statics declared here are implemented in p80211types.c
*   --------------------------------------------------------------------
*/

#ifndef _P80211META_H
#define _P80211META_H

/*================================================================*/
/* System Includes */

/*================================================================*/
/* Project Includes */

#ifndef _WLAN_COMPAT_H
#include "wlan_compat.h"
#endif

/*================================================================*/
/* Constants */

/*----------------------------------------------------------------*/
/* */

/*================================================================*/
/* Macros */

/*----------------------------------------------------------------*/
/* The following macros are used to ensure consistent naming */
/*  conventions for all the different metadata lists. */

#define MKREQMETANAME(name)		p80211meta_ ## req ## _ ## name
#define MKINDMETANAME(name)		p80211meta_ ## ind ## _ ## name
#define MKMIBMETANAME(name)		p80211meta_ ## mib ## _ ## name
#define MKGRPMETANAME(name)		p80211meta_ ## grp ## _ ## name

#define MKREQMETASIZE(name)		p80211meta_ ## req ## _ ## name ## _ ## size
#define MKINDMETASIZE(name)		p80211meta_ ## ind ## _ ## name ## _ ## size
#define MKMIBMETASIZE(name)		p80211meta_ ## mib ## _ ## name ## _ ## size
#define MKGRPMETASIZE(name)		p80211meta_ ## grp ## _ ## name ## _ ## size

#define GETMETASIZE(aptr)		(**((u32**)(aptr)))

/*----------------------------------------------------------------*/
/* The following ifdef depends on the following defines: */
/*  P80211_NOINCLUDESTRINGS - if defined, all metadata name fields */
/*                               are empty strings */

#ifdef P80211_NOINCLUDESTRINGS
	#define	MKITEMNAME(s)	("")
#else
	#define	MKITEMNAME(s)	(s)
#endif

/*================================================================*/
/* Types */

/*----------------------------------------------------------------*/
/* The following structure types are used for the metadata */
/* representation of category list metadata, group list metadata, */
/* and data item metadata for both Mib and Messages. */

typedef struct p80211meta
{
	char			*name;		/* data item name */
	u32			did;		/* partial did */
	u32			flags;		/* set of various flag bits */
	u32			min;		/* min value of a BOUNDEDint */
	u32			max;		/* max value of a BOUNDEDint */

	u32			maxlen;		/* maxlen of a OCTETSTR or DISPLAYSTR */
	u32			minlen;		/* minlen of a OCTETSTR or DISPLAYSTR */
	p80211enum_t		*enumptr;	/* ptr to the enum type for ENUMint */
	p80211_totext_t		totextptr;	/* ptr to totext conversion function */
	p80211_fromtext_t	fromtextptr;	/* ptr to totext conversion function */
	p80211_valid_t		validfunptr;	/* ptr to totext conversion function */
} p80211meta_t;

typedef struct grplistitem
{
	char		*name;
	p80211meta_t	*itemlist;
} grplistitem_t;

typedef struct catlistitem
{
	char		*name;
	grplistitem_t	*grplist;
} catlistitem_t;

/*================================================================*/
/* Extern Declarations */

/*----------------------------------------------------------------*/
/* */

/*================================================================*/
/* Function Declarations */

/*----------------------------------------------------------------*/
/* */
u32 p80211_text2did(catlistitem_t *catlist, char *catname, char *grpname, char *itemname);
u32 p80211_text2catdid(catlistitem_t *list, char *name );
u32 p80211_text2grpdid(grplistitem_t *list, char *name );
u32 p80211_text2itemdid(p80211meta_t *list, char *name );
u32 p80211_isvalid_did( catlistitem_t *catlist, u32 did );
u32 p80211_isvalid_catdid( catlistitem_t *catlist, u32 did );
u32 p80211_isvalid_grpdid( catlistitem_t *catlist, u32 did );
u32 p80211_isvalid_itemdid( catlistitem_t *catlist, u32 did );
catlistitem_t *p80211_did2cat( catlistitem_t *catlist, u32 did );
grplistitem_t *p80211_did2grp( catlistitem_t *catlist, u32 did );
p80211meta_t *p80211_did2item( catlistitem_t *catlist, u32 did );
u32 p80211item_maxdatalen( struct catlistitem *metalist, u32 did );
u32 p80211_metaname2did(struct catlistitem *metalist, char *itemname);
u32 p80211item_getoffset( struct catlistitem *metalist, u32 did );
int p80211item_gettype(p80211meta_t *meta);

#endif /* _P80211META_H */
