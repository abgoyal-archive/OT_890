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
 * MMC definitions for OMAP2
 *
 * Copyright (C) 2006 Nokia Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __OMAP2_MMC_H
#define __OMAP2_MMC_H

#include <linux/types.h>
#include <linux/device.h>
#include <linux/mmc/host.h>

#include <mach/board.h>

#define OMAP15XX_NR_MMC		1
#define OMAP16XX_NR_MMC		2
#define OMAP1_MMC_SIZE		0x080
#define OMAP1_MMC1_BASE		0xfffb7800
#define OMAP1_MMC2_BASE		0xfffb7c00	/* omap16xx only */

#define OMAP24XX_NR_MMC		2
#define OMAP34XX_NR_MMC		3
#define OMAP2420_MMC_SIZE	OMAP1_MMC_SIZE
#define HSMMC_SIZE		0x200
#define OMAP2_MMC1_BASE		0x4809c000
#define OMAP2_MMC2_BASE		0x480b4000
#define OMAP3_MMC3_BASE		0x480ad000
#define HSMMC3			(1 << 2)
#define HSMMC2			(1 << 1)
#define HSMMC1			(1 << 0)

#define OMAP_MMC_MAX_SLOTS	2

struct omap_mmc_platform_data {

	/* number of slots per controller */
	unsigned nr_slots:2;

	/* set if your board has components or wiring that limits the
	 * maximum frequency on the MMC bus */
	unsigned int max_freq;

	/* switch the bus to a new slot */
	int (* switch_slot)(struct device *dev, int slot);
	/* initialize board-specific MMC functionality, can be NULL if
	 * not supported */
	int (* init)(struct device *dev);
	void (* cleanup)(struct device *dev);
	void (* shutdown)(struct device *dev);

	/* To handle board related suspend/resume functionality for MMC */
	int (*suspend)(struct device *dev, int slot);
	int (*resume)(struct device *dev, int slot);

	u64 dma_mask;

	struct omap_mmc_slot_data {

		/* 4 wire signaling is optional, and is used for SD/SDIO/HSMMC;
		 * 8 wire signaling is also optional, and is used with HSMMC
		 */
		u8 wires;

		/*
		 * nomux means "standard" muxing is wrong on this board, and
		 * that board-specific code handled it before common init logic.
		 */
		unsigned nomux:1;

		/* switch pin can be for card detect (default) or card cover */
		unsigned cover:1;

		/* use the internal clock */
		unsigned internal_clock:1;
		s16 power_pin;

		int switch_pin;			/* gpio (card detect) */
		int gpio_wp;			/* gpio (write protect) */

		int (* set_bus_mode)(struct device *dev, int slot, int bus_mode);
		int (* set_power)(struct device *dev, int slot, int power_on, int vdd);
		int (* get_ro)(struct device *dev, int slot);

		/* return MMC cover switch state, can be NULL if not supported.
		 *
		 * possible return values:
		 *   0 - closed
		 *   1 - open
		 */
		int (* get_cover_state)(struct device *dev, int slot);

		const char *name;
		u32 ocr_mask;

		/* Card detection IRQs */
		int card_detect_irq;
		int (* card_detect)(int irq);

		unsigned int ban_openended:1;

	} slots[OMAP_MMC_MAX_SLOTS];
};

/* called from board-specific card detection service routine */
extern void omap_mmc_notify_cover_event(struct device *dev, int slot, int is_closed);

#if	defined(CONFIG_MMC_OMAP) || defined(CONFIG_MMC_OMAP_MODULE) || \
	defined(CONFIG_MMC_OMAP_HS) || defined(CONFIG_MMC_OMAP_HS_MODULE)
void omap1_init_mmc(struct omap_mmc_platform_data **mmc_data,
				int nr_controllers);
void omap2_init_mmc(struct omap_mmc_platform_data **mmc_data,
				int nr_controllers);
int omap_mmc_add(const char *name, int id, unsigned long base,
				unsigned long size, unsigned int irq,
				struct omap_mmc_platform_data *data);
#else
static inline void omap1_init_mmc(struct omap_mmc_platform_data **mmc_data,
				int nr_controllers)
{
}
static inline void omap2_init_mmc(struct omap_mmc_platform_data **mmc_data,
				int nr_controllers)
{
}
static inline int omap_mmc_add(const char *name, int id, unsigned long base,
				unsigned long size, unsigned int irq,
				struct omap_mmc_platform_data *data)
{
	return 0;
}

#endif
#endif
