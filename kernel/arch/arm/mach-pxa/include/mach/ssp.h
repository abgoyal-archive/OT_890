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
 *  ssp.h
 *
 *  Copyright (C) 2003 Russell King, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This driver supports the following PXA CPU/SSP ports:-
 *
 *       PXA250     SSP
 *       PXA255     SSP, NSSP
 *       PXA26x     SSP, NSSP, ASSP
 *       PXA27x     SSP1, SSP2, SSP3
 *       PXA3xx     SSP1, SSP2, SSP3, SSP4
 */

#ifndef __ASM_ARCH_SSP_H
#define __ASM_ARCH_SSP_H

#include <linux/list.h>
#include <linux/io.h>

enum pxa_ssp_type {
	SSP_UNDEFINED = 0,
	PXA25x_SSP,  /* pxa 210, 250, 255, 26x */
	PXA25x_NSSP, /* pxa 255, 26x (including ASSP) */
	PXA27x_SSP,
};

struct ssp_device {
	struct platform_device *pdev;
	struct list_head	node;

	struct clk	*clk;
	void __iomem	*mmio_base;
	unsigned long	phys_base;

	const char	*label;
	int		port_id;
	int		type;
	int		use_count;
	int		irq;
	int		drcmr_rx;
	int		drcmr_tx;
};

/*
 * SSP initialisation flags
 */
#define SSP_NO_IRQ	0x1		/* don't register an irq handler in SSP driver */

struct ssp_state {
	u32	cr0;
	u32 cr1;
	u32 to;
	u32 psp;
};

struct ssp_dev {
	struct ssp_device *ssp;
	u32 port;
	u32 mode;
	u32 flags;
	u32 psp_flags;
	u32 speed;
	int irq;
};

int ssp_write_word(struct ssp_dev *dev, u32 data);
int ssp_read_word(struct ssp_dev *dev, u32 *data);
int ssp_flush(struct ssp_dev *dev);
void ssp_enable(struct ssp_dev *dev);
void ssp_disable(struct ssp_dev *dev);
void ssp_save_state(struct ssp_dev *dev, struct ssp_state *ssp);
void ssp_restore_state(struct ssp_dev *dev, struct ssp_state *ssp);
int ssp_init(struct ssp_dev *dev, u32 port, u32 init_flags);
int ssp_config(struct ssp_dev *dev, u32 mode, u32 flags, u32 psp_flags, u32 speed);
void ssp_exit(struct ssp_dev *dev);

/**
 * ssp_write_reg - Write to a SSP register
 *
 * @dev: SSP device to access
 * @reg: Register to write to
 * @val: Value to be written.
 */
static inline void ssp_write_reg(struct ssp_device *dev, u32 reg, u32 val)
{
	__raw_writel(val, dev->mmio_base + reg);
}

/**
 * ssp_read_reg - Read from a SSP register
 *
 * @dev: SSP device to access
 * @reg: Register to read from
 */
static inline u32 ssp_read_reg(struct ssp_device *dev, u32 reg)
{
	return __raw_readl(dev->mmio_base + reg);
}

struct ssp_device *ssp_request(int port, const char *label);
void ssp_free(struct ssp_device *);
#endif /* __ASM_ARCH_SSP_H */
