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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1992 - 1997, 2000-2004 Silicon Graphics, Inc. All rights reserved.
 */
#ifndef _ASM_IA64_SN_MODULE_H
#define _ASM_IA64_SN_MODULE_H

/* parameter for format_module_id() */
#define MODULE_FORMAT_BRIEF	1
#define MODULE_FORMAT_LONG	2
#define MODULE_FORMAT_LCD	3

/*
 *	Module id format
 *
 *	31-16	Rack ID (encoded class, group, number - 16-bit unsigned int)
 *	 15-8	Brick type (8-bit ascii character)
 *	  7-0	Bay (brick position in rack (0-63) - 8-bit unsigned int)
 *
 */

/*
 * Macros for getting the brick type
 */
#define MODULE_BTYPE_MASK	0xff00
#define MODULE_BTYPE_SHFT	8
#define MODULE_GET_BTYPE(_m)	(((_m) & MODULE_BTYPE_MASK) >> MODULE_BTYPE_SHFT)
#define MODULE_BT_TO_CHAR(_b)	((char)(_b))
#define MODULE_GET_BTCHAR(_m)	(MODULE_BT_TO_CHAR(MODULE_GET_BTYPE(_m)))

/*
 * Macros for getting the rack ID.
 */
#define MODULE_RACK_MASK	0xffff0000
#define MODULE_RACK_SHFT	16
#define MODULE_GET_RACK(_m)	(((_m) & MODULE_RACK_MASK) >> MODULE_RACK_SHFT)

/*
 * Macros for getting the brick position
 */
#define MODULE_BPOS_MASK	0x00ff
#define MODULE_BPOS_SHFT	0
#define MODULE_GET_BPOS(_m)	(((_m) & MODULE_BPOS_MASK) >> MODULE_BPOS_SHFT)

/*
 * Macros for encoding and decoding rack IDs
 * A rack number consists of three parts:
 *   class (0==CPU/mixed, 1==I/O), group, number
 *
 * Rack number is stored just as it is displayed on the screen:
 * a 3-decimal-digit number.
 */
#define RACK_CLASS_DVDR         100
#define RACK_GROUP_DVDR         10
#define RACK_NUM_DVDR           1

#define RACK_CREATE_RACKID(_c, _g, _n)  ((_c) * RACK_CLASS_DVDR +       \
        (_g) * RACK_GROUP_DVDR + (_n) * RACK_NUM_DVDR)

#define RACK_GET_CLASS(_r)              ((_r) / RACK_CLASS_DVDR)
#define RACK_GET_GROUP(_r)              (((_r) - RACK_GET_CLASS(_r) *   \
            RACK_CLASS_DVDR) / RACK_GROUP_DVDR)
#define RACK_GET_NUM(_r)                (((_r) - RACK_GET_CLASS(_r) *   \
            RACK_CLASS_DVDR - RACK_GET_GROUP(_r) *      \
            RACK_GROUP_DVDR) / RACK_NUM_DVDR)

/*
 * Macros for encoding and decoding rack IDs
 * A rack number consists of three parts:
 *   class      1 bit, 0==CPU/mixed, 1==I/O
 *   group      2 bits for CPU/mixed, 3 bits for I/O
 *   number     3 bits for CPU/mixed, 2 bits for I/O (1 based)
 */
#define RACK_GROUP_BITS(_r)     (RACK_GET_CLASS(_r) ? 3 : 2)
#define RACK_NUM_BITS(_r)       (RACK_GET_CLASS(_r) ? 2 : 3)

#define RACK_CLASS_MASK(_r)     0x20
#define RACK_CLASS_SHFT(_r)     5
#define RACK_ADD_CLASS(_r, _c)  \
        ((_r) |= (_c) << RACK_CLASS_SHFT(_r) & RACK_CLASS_MASK(_r))

#define RACK_GROUP_SHFT(_r)     RACK_NUM_BITS(_r)
#define RACK_GROUP_MASK(_r)     \
        ( (((unsigned)1<<RACK_GROUP_BITS(_r)) - 1) << RACK_GROUP_SHFT(_r) )
#define RACK_ADD_GROUP(_r, _g)  \
        ((_r) |= (_g) << RACK_GROUP_SHFT(_r) & RACK_GROUP_MASK(_r))

#define RACK_NUM_SHFT(_r)       0
#define RACK_NUM_MASK(_r)       \
        ( (((unsigned)1<<RACK_NUM_BITS(_r)) - 1) << RACK_NUM_SHFT(_r) )
#define RACK_ADD_NUM(_r, _n)    \
        ((_r) |= ((_n) - 1) << RACK_NUM_SHFT(_r) & RACK_NUM_MASK(_r))


/*
 * Brick type definitions
 */
#define MAX_BRICK_TYPES         256 /* brick type is stored as uchar */

extern char brick_types[];

#define MODULE_CBRICK           0
#define MODULE_RBRICK           1
#define MODULE_IBRICK           2
#define MODULE_KBRICK           3
#define MODULE_XBRICK           4
#define MODULE_DBRICK           5
#define MODULE_PBRICK           6
#define MODULE_NBRICK           7
#define MODULE_PEBRICK          8
#define MODULE_PXBRICK          9
#define MODULE_IXBRICK          10
#define MODULE_CGBRICK		11
#define MODULE_OPUSBRICK        12
#define MODULE_SABRICK		13	/* TIO BringUp Brick */
#define MODULE_IABRICK		14
#define MODULE_PABRICK		15
#define MODULE_GABRICK		16
#define MODULE_OPUS_TIO		17	/* OPUS TIO Riser */

extern char brick_types[];
extern void format_module_id(char *, moduleid_t, int);

#endif /* _ASM_IA64_SN_MODULE_H */
