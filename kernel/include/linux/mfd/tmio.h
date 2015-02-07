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

#ifndef MFD_TMIO_H
#define MFD_TMIO_H

#include <linux/fb.h>

#define tmio_ioread8(addr) readb(addr)
#define tmio_ioread16(addr) readw(addr)
#define tmio_ioread16_rep(r, b, l) readsw(r, b, l)
#define tmio_ioread32(addr) \
	(((u32) readw((addr))) | (((u32) readw((addr) + 2)) << 16))

#define tmio_iowrite8(val, addr) writeb((val), (addr))
#define tmio_iowrite16(val, addr) writew((val), (addr))
#define tmio_iowrite16_rep(r, b, l) writesw(r, b, l)
#define tmio_iowrite32(val, addr) \
	do { \
	writew((val),       (addr)); \
	writew((val) >> 16, (addr) + 2); \
	} while (0)

/*
 * data for the NAND controller
 */
struct tmio_nand_data {
	struct nand_bbt_descr	*badblock_pattern;
	struct mtd_partition	*partition;
	unsigned int		num_partitions;
};

#define FBIO_TMIO_ACC_WRITE	0x7C639300
#define FBIO_TMIO_ACC_SYNC	0x7C639301

struct tmio_fb_data {
	int			(*lcd_set_power)(struct platform_device *fb_dev,
								bool on);
	int			(*lcd_mode)(struct platform_device *fb_dev,
					const struct fb_videomode *mode);
	int			num_modes;
	struct fb_videomode	*modes;

	/* in mm: size of screen */
	int			height;
	int			width;
};


#endif
