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
 * xtalk.h -- platform-independent crosstalk interface, derived from
 * IRIX <sys/PCI/bridge.h>, revision 1.38.
 *
 * Copyright (C) 1995 - 1997, 1999 Silcon Graphics, Inc.
 * Copyright (C) 1999 Ralf Baechle (ralf@gnu.org)
 */
#ifndef _ASM_XTALK_XTALK_H
#define _ASM_XTALK_XTALK_H

#ifndef __ASSEMBLY__
/*
 * User-level device driver visible types
 */
typedef char            xwidgetnum_t;	/* xtalk widget number  (0..15) */

#define XWIDGET_NONE		-1

typedef int xwidget_part_num_t;	/* xtalk widget part number */

#define XWIDGET_PART_NUM_NONE	-1

typedef int             xwidget_rev_num_t;	/* xtalk widget revision number */

#define XWIDGET_REV_NUM_NONE	-1

typedef int xwidget_mfg_num_t;	/* xtalk widget manufacturing ID */

#define XWIDGET_MFG_NUM_NONE	-1

typedef struct xtalk_piomap_s *xtalk_piomap_t;

/* It is often convenient to fold the XIO target port
 * number into the XIO address.
 */
#define	XIO_NOWHERE	(0xFFFFFFFFFFFFFFFFull)
#define	XIO_ADDR_BITS	(0x0000FFFFFFFFFFFFull)
#define	XIO_PORT_BITS	(0xF000000000000000ull)
#define	XIO_PORT_SHIFT	(60)

#define	XIO_PACKED(x)	(((x)&XIO_PORT_BITS) != 0)
#define	XIO_ADDR(x)	((x)&XIO_ADDR_BITS)
#define	XIO_PORT(x)	((xwidgetnum_t)(((x)&XIO_PORT_BITS) >> XIO_PORT_SHIFT))
#define	XIO_PACK(p, o)	((((uint64_t)(p))<<XIO_PORT_SHIFT) | ((o)&XIO_ADDR_BITS))

#endif /* !__ASSEMBLY__ */

#endif /* _ASM_XTALK_XTALK_H */
