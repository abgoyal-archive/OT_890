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
 *  ecard.h
 *
 *  Copyright 2007 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/* Definitions internal to ecard.c - for it's use only!!
 *
 * External expansion card header as read from the card
 */
struct ex_ecid {
	unsigned char	r_irq:1;
	unsigned char	r_zero:1;
	unsigned char	r_fiq:1;
	unsigned char	r_id:4;
	unsigned char	r_a:1;

	unsigned char	r_cd:1;
	unsigned char	r_is:1;
	unsigned char	r_w:2;
	unsigned char	r_r1:4;

	unsigned char	r_r2:8;

	unsigned char	r_prod[2];

	unsigned char	r_manu[2];

	unsigned char	r_country;

	unsigned char	r_fiqmask;
	unsigned char	r_fiqoff[3];

	unsigned char	r_irqmask;
	unsigned char	r_irqoff[3];
};

/*
 * Chunk directory entry as read from the card
 */
struct ex_chunk_dir {
	unsigned char r_id;
	unsigned char r_len[3];
	unsigned long r_start;
	union {
		char string[256];
		char data[1];
	} d;
#define c_id(x)		((x)->r_id)
#define c_len(x)	((x)->r_len[0]|((x)->r_len[1]<<8)|((x)->r_len[2]<<16))
#define c_start(x)	((x)->r_start)
};

typedef enum ecard_type {		/* Cards address space		*/
	ECARD_IOC,
	ECARD_MEMC,
	ECARD_EASI
} card_type_t;

typedef enum {				/* Speed for ECARD_IOC space	*/
	ECARD_SLOW	 = 0,
	ECARD_MEDIUM	 = 1,
	ECARD_FAST	 = 2,
	ECARD_SYNC	 = 3
} card_speed_t;
