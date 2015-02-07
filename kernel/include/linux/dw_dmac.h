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
 * Driver for the Synopsys DesignWare DMA Controller (aka DMACA on
 * AVR32 systems.)
 *
 * Copyright (C) 2007 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef DW_DMAC_H
#define DW_DMAC_H

#include <linux/dmaengine.h>

/**
 * struct dw_dma_platform_data - Controller configuration parameters
 * @nr_channels: Number of channels supported by hardware (max 8)
 */
struct dw_dma_platform_data {
	unsigned int	nr_channels;
};

/**
 * enum dw_dma_slave_width - DMA slave register access width.
 * @DMA_SLAVE_WIDTH_8BIT: Do 8-bit slave register accesses
 * @DMA_SLAVE_WIDTH_16BIT: Do 16-bit slave register accesses
 * @DMA_SLAVE_WIDTH_32BIT: Do 32-bit slave register accesses
 */
enum dw_dma_slave_width {
	DW_DMA_SLAVE_WIDTH_8BIT,
	DW_DMA_SLAVE_WIDTH_16BIT,
	DW_DMA_SLAVE_WIDTH_32BIT,
};

/**
 * struct dw_dma_slave - Controller-specific information about a slave
 *
 * @dma_dev: required DMA master device
 * @tx_reg: physical address of data register used for
 *	memory-to-peripheral transfers
 * @rx_reg: physical address of data register used for
 *	peripheral-to-memory transfers
 * @reg_width: peripheral register width
 * @cfg_hi: Platform-specific initializer for the CFG_HI register
 * @cfg_lo: Platform-specific initializer for the CFG_LO register
 */
struct dw_dma_slave {
	struct device		*dma_dev;
	dma_addr_t		tx_reg;
	dma_addr_t		rx_reg;
	enum dw_dma_slave_width	reg_width;
	u32			cfg_hi;
	u32			cfg_lo;
};

/* Platform-configurable bits in CFG_HI */
#define DWC_CFGH_FCMODE		(1 << 0)
#define DWC_CFGH_FIFO_MODE	(1 << 1)
#define DWC_CFGH_PROTCTL(x)	((x) << 2)
#define DWC_CFGH_SRC_PER(x)	((x) << 7)
#define DWC_CFGH_DST_PER(x)	((x) << 11)

/* Platform-configurable bits in CFG_LO */
#define DWC_CFGL_PRIO(x)	((x) << 5)	/* priority */
#define DWC_CFGL_LOCK_CH_XFER	(0 << 12)	/* scope of LOCK_CH */
#define DWC_CFGL_LOCK_CH_BLOCK	(1 << 12)
#define DWC_CFGL_LOCK_CH_XACT	(2 << 12)
#define DWC_CFGL_LOCK_BUS_XFER	(0 << 14)	/* scope of LOCK_BUS */
#define DWC_CFGL_LOCK_BUS_BLOCK	(1 << 14)
#define DWC_CFGL_LOCK_BUS_XACT	(2 << 14)
#define DWC_CFGL_LOCK_CH	(1 << 15)	/* channel lockout */
#define DWC_CFGL_LOCK_BUS	(1 << 16)	/* busmaster lockout */
#define DWC_CFGL_HS_DST_POL	(1 << 18)	/* dst handshake active low */
#define DWC_CFGL_HS_SRC_POL	(1 << 19)	/* src handshake active low */

#endif /* DW_DMAC_H */
