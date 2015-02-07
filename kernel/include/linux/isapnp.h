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
 *  ISA Plug & Play support
 *  Copyright (c) by Jaroslav Kysela <perex@suse.cz>
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef LINUX_ISAPNP_H
#define LINUX_ISAPNP_H

#include <linux/errno.h>
#include <linux/pnp.h>

/*
 *
 */

#define ISAPNP_VENDOR(a,b,c)	(((((a)-'A'+1)&0x3f)<<2)|\
				((((b)-'A'+1)&0x18)>>3)|((((b)-'A'+1)&7)<<13)|\
				((((c)-'A'+1)&0x1f)<<8))
#define ISAPNP_DEVICE(x)	((((x)&0xf000)>>8)|\
				 (((x)&0x0f00)>>8)|\
				 (((x)&0x00f0)<<8)|\
				 (((x)&0x000f)<<8))
#define ISAPNP_FUNCTION(x)	ISAPNP_DEVICE(x)

/*
 *
 */

#ifdef __KERNEL__

#define DEVICE_COUNT_COMPATIBLE 4

#define ISAPNP_ANY_ID		0xffff
#define ISAPNP_CARD_DEVS	8

#define ISAPNP_CARD_ID(_va, _vb, _vc, _device) \
		.card_vendor = ISAPNP_VENDOR(_va, _vb, _vc), .card_device = ISAPNP_DEVICE(_device)
#define ISAPNP_CARD_END \
		.card_vendor = 0, .card_device = 0
#define ISAPNP_DEVICE_ID(_va, _vb, _vc, _function) \
		{ .vendor = ISAPNP_VENDOR(_va, _vb, _vc), .function = ISAPNP_FUNCTION(_function) }

/* export used IDs outside module */
#define ISAPNP_CARD_TABLE(name) \
		MODULE_GENERIC_TABLE(isapnp_card, name)

struct isapnp_card_id {
	unsigned long driver_data;	/* data private to the driver */
	unsigned short card_vendor, card_device;
	struct {
		unsigned short vendor, function;
	} devs[ISAPNP_CARD_DEVS];	/* logical devices */
};

#define ISAPNP_DEVICE_SINGLE(_cva, _cvb, _cvc, _cdevice, _dva, _dvb, _dvc, _dfunction) \
		.card_vendor = ISAPNP_VENDOR(_cva, _cvb, _cvc), .card_device =  ISAPNP_DEVICE(_cdevice), \
		.vendor = ISAPNP_VENDOR(_dva, _dvb, _dvc), .function = ISAPNP_FUNCTION(_dfunction)
#define ISAPNP_DEVICE_SINGLE_END \
		.card_vendor = 0, .card_device = 0

struct isapnp_device_id {
	unsigned short card_vendor, card_device;
	unsigned short vendor, function;
	unsigned long driver_data;	/* data private to the driver */
};

#if defined(CONFIG_ISAPNP) || (defined(CONFIG_ISAPNP_MODULE) && defined(MODULE))

#define __ISAPNP__

/* lowlevel configuration */
int isapnp_present(void);
int isapnp_cfg_begin(int csn, int device);
int isapnp_cfg_end(void);
unsigned char isapnp_read_byte(unsigned char idx);
void isapnp_write_byte(unsigned char idx, unsigned char val);

#ifdef CONFIG_PROC_FS
int isapnp_proc_init(void);
int isapnp_proc_done(void);
#else
static inline int isapnp_proc_init(void) { return 0; }
static inline int isapnp_proc_done(void) { return 0; }
#endif

/* compat */
struct pnp_card *pnp_find_card(unsigned short vendor,
			       unsigned short device,
			       struct pnp_card *from);
struct pnp_dev *pnp_find_dev(struct pnp_card *card,
			     unsigned short vendor,
			     unsigned short function,
			     struct pnp_dev *from);

#else /* !CONFIG_ISAPNP */

/* lowlevel configuration */
static inline int isapnp_present(void) { return 0; }
static inline int isapnp_cfg_begin(int csn, int device) { return -ENODEV; }
static inline int isapnp_cfg_end(void) { return -ENODEV; }
static inline unsigned char isapnp_read_byte(unsigned char idx) { return 0xff; }
static inline void isapnp_write_byte(unsigned char idx, unsigned char val) { ; }

static inline struct pnp_card *pnp_find_card(unsigned short vendor,
					     unsigned short device,
					     struct pnp_card *from) { return NULL; }
static inline struct pnp_dev *pnp_find_dev(struct pnp_card *card,
					   unsigned short vendor,
					   unsigned short function,
					   struct pnp_dev *from) { return NULL; }

#endif /* CONFIG_ISAPNP */

#endif /* __KERNEL__ */
#endif /* LINUX_ISAPNP_H */
