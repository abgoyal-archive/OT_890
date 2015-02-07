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

#ifndef __ASM_SH_MOBILE_LCDC_H__
#define __ASM_SH_MOBILE_LCDC_H__

#include <linux/fb.h>

enum { RGB8,   /* 24bpp, 8:8:8 */
       RGB9,   /* 18bpp, 9:9 */
       RGB12A, /* 24bpp, 12:12 */
       RGB12B, /* 12bpp */
       RGB16,  /* 16bpp */
       RGB18,  /* 18bpp */
       RGB24,  /* 24bpp */
       SYS8A,  /* 24bpp, 8:8:8 */
       SYS8B,  /* 18bpp, 8:8:2 */
       SYS8C,  /* 18bpp, 2:8:8 */
       SYS8D,  /* 16bpp, 8:8 */
       SYS9,   /* 18bpp, 9:9 */
       SYS12,  /* 24bpp, 12:12 */
       SYS16A, /* 16bpp */
       SYS16B, /* 18bpp, 16:2 */
       SYS16C, /* 18bpp, 2:16 */
       SYS18,  /* 18bpp */
       SYS24 };/* 24bpp */

enum { LCDC_CHAN_DISABLED = 0,
       LCDC_CHAN_MAINLCD,
       LCDC_CHAN_SUBLCD };

enum { LCDC_CLK_BUS, LCDC_CLK_PERIPHERAL, LCDC_CLK_EXTERNAL };

#define LCDC_FLAGS_DWPOL (1 << 0) /* Rising edge dot clock data latch */
#define LCDC_FLAGS_DIPOL (1 << 1) /* Active low display enable polarity */
#define LCDC_FLAGS_DAPOL (1 << 2) /* Active low display data polarity */
#define LCDC_FLAGS_HSCNT (1 << 3) /* Disable HSYNC during VBLANK */
#define LCDC_FLAGS_DWCNT (1 << 4) /* Disable dotclock during blanking */

struct sh_mobile_lcdc_sys_bus_cfg {
	unsigned long ldmt2r;
	unsigned long ldmt3r;
	unsigned long deferred_io_msec;
};

struct sh_mobile_lcdc_sys_bus_ops {
	void (*write_index)(void *handle, unsigned long data);
	void (*write_data)(void *handle, unsigned long data);
	unsigned long (*read_data)(void *handle);
};

struct sh_mobile_lcdc_board_cfg {
	void *board_data;
	int (*setup_sys)(void *board_data, void *sys_ops_handle,
			 struct sh_mobile_lcdc_sys_bus_ops *sys_ops);
	void (*display_on)(void *board_data);
	void (*display_off)(void *board_data);
};

struct sh_mobile_lcdc_lcd_size_cfg { /* width and height of panel in mm */
	unsigned long width;
	unsigned long height;
};

struct sh_mobile_lcdc_chan_cfg {
	int chan;
	int bpp;
	int interface_type; /* selects RGBn or SYSn I/F, see above */
	int clock_divider;
	unsigned long flags; /* LCDC_FLAGS_... */
	struct fb_videomode lcd_cfg;
	struct sh_mobile_lcdc_lcd_size_cfg lcd_size_cfg;
	struct sh_mobile_lcdc_board_cfg board_cfg;
	struct sh_mobile_lcdc_sys_bus_cfg sys_bus_cfg; /* only for SYSn I/F */
};

struct sh_mobile_lcdc_info {
	int clock_source;
	struct sh_mobile_lcdc_chan_cfg ch[2];
};

#endif /* __ASM_SH_MOBILE_LCDC_H__ */
