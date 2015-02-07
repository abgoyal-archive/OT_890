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
 * Copyright (C) 1992-1997,2000-2003 Silicon Graphics, Inc. All Rights Reserved.
 */
#ifndef _ASM_IA64_SN_XTALK_XWIDGET_H
#define _ASM_IA64_SN_XTALK_XWIDGET_H

/* WIDGET_ID */
#define WIDGET_REV_NUM                  0xf0000000
#define WIDGET_PART_NUM                 0x0ffff000
#define WIDGET_MFG_NUM                  0x00000ffe
#define WIDGET_REV_NUM_SHFT             28
#define WIDGET_PART_NUM_SHFT            12
#define WIDGET_MFG_NUM_SHFT             1

#define XWIDGET_PART_NUM(widgetid) (((widgetid) & WIDGET_PART_NUM) >> WIDGET_PART_NUM_SHFT)
#define XWIDGET_REV_NUM(widgetid) (((widgetid) & WIDGET_REV_NUM) >> WIDGET_REV_NUM_SHFT)
#define XWIDGET_MFG_NUM(widgetid) (((widgetid) & WIDGET_MFG_NUM) >> WIDGET_MFG_NUM_SHFT)
#define XWIDGET_PART_REV_NUM(widgetid) ((XWIDGET_PART_NUM(widgetid) << 4) | \
                                        XWIDGET_REV_NUM(widgetid))
#define XWIDGET_PART_REV_NUM_REV(partrev) (partrev & 0xf)

/* widget configuration registers */
struct widget_cfg{
	u32	w_id;	/* 0x04 */
	u32	w_pad_0;	/* 0x00 */
	u32	w_status;	/* 0x0c */
	u32	w_pad_1;	/* 0x08 */
	u32	w_err_upper_addr;	/* 0x14 */
	u32	w_pad_2;	/* 0x10 */
	u32	w_err_lower_addr;	/* 0x1c */
	u32	w_pad_3;	/* 0x18 */
	u32	w_control;	/* 0x24 */
	u32	w_pad_4;	/* 0x20 */
	u32	w_req_timeout;	/* 0x2c */
	u32	w_pad_5;	/* 0x28 */
	u32	w_intdest_upper_addr;	/* 0x34 */
	u32	w_pad_6;	/* 0x30 */
	u32	w_intdest_lower_addr;	/* 0x3c */
	u32	w_pad_7;	/* 0x38 */
	u32	w_err_cmd_word;	/* 0x44 */
	u32	w_pad_8;	/* 0x40 */
	u32	w_llp_cfg;	/* 0x4c */
	u32	w_pad_9;	/* 0x48 */
	u32	w_tflush;	/* 0x54 */
	u32	w_pad_10;	/* 0x50 */
};

/*
 * Crosstalk Widget Hardware Identification, as defined in the Crosstalk spec.
 */
struct xwidget_hwid{
	int		mfg_num;
	int		rev_num;
	int		part_num;
};

struct xwidget_info{

	struct xwidget_hwid	xwi_hwid;	/* Widget Identification */
	char			xwi_masterxid;	/* Hub's Widget Port Number */
	void			*xwi_hubinfo;     /* Hub's provider private info */
	u64			*xwi_hub_provider; /* prom provider functions */
	void			*xwi_vertex;
};

#endif                          /* _ASM_IA64_SN_XTALK_XWIDGET_H */
