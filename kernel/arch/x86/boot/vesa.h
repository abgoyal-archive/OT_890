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

/* ----------------------------------------------------------------------- *
 *
 *   Copyright 1999-2007 H. Peter Anvin - All Rights Reserved
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, Inc., 53 Temple Place Ste 330,
 *   Boston MA 02111-1307, USA; either version 2 of the License, or
 *   (at your option) any later version; incorporated herein by reference.
 *
 * ----------------------------------------------------------------------- */

#ifndef BOOT_VESA_H
#define BOOT_VESA_H

typedef struct {
	u16 off, seg;
} far_ptr;

/* VESA General Information table */
struct vesa_general_info {
	u32 signature;		/* 0 Magic number = "VESA" */
	u16 version;		/* 4 */
	far_ptr vendor_string;	/* 6 */
	u32 capabilities;	/* 10 */
	far_ptr video_mode_ptr;	/* 14 */
	u16 total_memory;	/* 18 */

	u8 reserved[236];	/* 20 */
} __attribute__ ((packed));

#define VESA_MAGIC ('V' + ('E' << 8) + ('S' << 16) + ('A' << 24))

struct vesa_mode_info {
	u16 mode_attr;		/* 0 */
	u8 win_attr[2];		/* 2 */
	u16 win_grain;		/* 4 */
	u16 win_size;		/* 6 */
	u16 win_seg[2];		/* 8 */
	far_ptr win_scheme;	/* 12 */
	u16 logical_scan;	/* 16 */

	u16 h_res;		/* 18 */
	u16 v_res;		/* 20 */
	u8 char_width;		/* 22 */
	u8 char_height;		/* 23 */
	u8 memory_planes;	/* 24 */
	u8 bpp;			/* 25 */
	u8 banks;		/* 26 */
	u8 memory_layout;	/* 27 */
	u8 bank_size;		/* 28 */
	u8 image_planes;	/* 29 */
	u8 page_function;	/* 30 */

	u8 rmask;		/* 31 */
	u8 rpos;		/* 32 */
	u8 gmask;		/* 33 */
	u8 gpos;		/* 34 */
	u8 bmask;		/* 35 */
	u8 bpos;		/* 36 */
	u8 resv_mask;		/* 37 */
	u8 resv_pos;		/* 38 */
	u8 dcm_info;		/* 39 */

	u32 lfb_ptr;		/* 40 Linear frame buffer address */
	u32 offscreen_ptr;	/* 44 Offscreen memory address */
	u16 offscreen_size;	/* 48 */

	u8 reserved[206];	/* 50 */
} __attribute__ ((packed));

#endif				/* LIB_SYS_VESA_H */
