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
 * dma.h - Blackfin DMA defines/structures/etc...
 *
 * Copyright 2004-2008 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#ifndef _BLACKFIN_DMA_H_
#define _BLACKFIN_DMA_H_

#include <linux/interrupt.h>
#include <mach/dma.h>
#include <asm/blackfin.h>
#include <asm/page.h>

#define MAX_DMA_ADDRESS PAGE_OFFSET

/*****************************************************************************
*        Generic DMA  Declarations
*
****************************************************************************/
enum dma_chan_status {
	DMA_CHANNEL_FREE,
	DMA_CHANNEL_REQUESTED,
	DMA_CHANNEL_ENABLED,
};

/*-------------------------
 * config reg bits value
 *-------------------------*/
#define DATA_SIZE_8 		0
#define DATA_SIZE_16 		1
#define DATA_SIZE_32 		2

#define DMA_FLOW_STOP 		0
#define DMA_FLOW_AUTO 		1
#define DMA_FLOW_ARRAY 		4
#define DMA_FLOW_SMALL 		6
#define DMA_FLOW_LARGE 		7

#define DIMENSION_LINEAR    0
#define DIMENSION_2D           1

#define DIR_READ     0
#define DIR_WRITE    1

#define INTR_DISABLE   0
#define INTR_ON_BUF    2
#define INTR_ON_ROW    3

#define DMA_NOSYNC_KEEP_DMA_BUF	0
#define DMA_SYNC_RESTART	1

struct dmasg {
	void *next_desc_addr;
	unsigned long start_addr;
	unsigned short cfg;
	unsigned short x_count;
	short x_modify;
	unsigned short y_count;
	short y_modify;
} __attribute__((packed));

struct dma_register {
	void *next_desc_ptr;	/* DMA Next Descriptor Pointer register */
	unsigned long start_addr;	/* DMA Start address  register */

	unsigned short cfg;	/* DMA Configuration register */
	unsigned short dummy1;	/* DMA Configuration register */

	unsigned long reserved;

	unsigned short x_count;	/* DMA x_count register */
	unsigned short dummy2;

	short x_modify;	/* DMA x_modify register */
	unsigned short dummy3;

	unsigned short y_count;	/* DMA y_count register */
	unsigned short dummy4;

	short y_modify;	/* DMA y_modify register */
	unsigned short dummy5;

	void *curr_desc_ptr;	/* DMA Current Descriptor Pointer
					   register */
	unsigned long curr_addr_ptr;	/* DMA Current Address Pointer
						   register */
	unsigned short irq_status;	/* DMA irq status register */
	unsigned short dummy6;

	unsigned short peripheral_map;	/* DMA peripheral map register */
	unsigned short dummy7;

	unsigned short curr_x_count;	/* DMA Current x-count register */
	unsigned short dummy8;

	unsigned long reserved2;

	unsigned short curr_y_count;	/* DMA Current y-count register */
	unsigned short dummy9;

	unsigned long reserved3;

};

struct mutex;
struct dma_channel {
	struct mutex dmalock;
	const char *device_id;
	enum dma_chan_status chan_status;
	volatile struct dma_register *regs;
	struct dmasg *sg;		/* large mode descriptor */
	unsigned int irq;
	void *data;
#ifdef CONFIG_PM
	unsigned short saved_peripheral_map;
#endif
};

#ifdef CONFIG_PM
int blackfin_dma_suspend(void);
void blackfin_dma_resume(void);
#endif

/*******************************************************************************
*	DMA API's
*******************************************************************************/
extern struct dma_channel dma_ch[MAX_DMA_CHANNELS];
extern struct dma_register *dma_io_base_addr[MAX_DMA_CHANNELS];
extern int channel2irq(unsigned int channel);

static inline void set_dma_start_addr(unsigned int channel, unsigned long addr)
{
	dma_ch[channel].regs->start_addr = addr;
}
static inline void set_dma_next_desc_addr(unsigned int channel, void *addr)
{
	dma_ch[channel].regs->next_desc_ptr = addr;
}
static inline void set_dma_curr_desc_addr(unsigned int channel, void *addr)
{
	dma_ch[channel].regs->curr_desc_ptr = addr;
}
static inline void set_dma_x_count(unsigned int channel, unsigned short x_count)
{
	dma_ch[channel].regs->x_count = x_count;
}
static inline void set_dma_y_count(unsigned int channel, unsigned short y_count)
{
	dma_ch[channel].regs->y_count = y_count;
}
static inline void set_dma_x_modify(unsigned int channel, short x_modify)
{
	dma_ch[channel].regs->x_modify = x_modify;
}
static inline void set_dma_y_modify(unsigned int channel, short y_modify)
{
	dma_ch[channel].regs->y_modify = y_modify;
}
static inline void set_dma_config(unsigned int channel, unsigned short config)
{
	dma_ch[channel].regs->cfg = config;
}
static inline void set_dma_curr_addr(unsigned int channel, unsigned long addr)
{
	dma_ch[channel].regs->curr_addr_ptr = addr;
}

static inline unsigned short
set_bfin_dma_config(char direction, char flow_mode,
		    char intr_mode, char dma_mode, char width, char syncmode)
{
	return (direction << 1) | (width << 2) | (dma_mode << 4) |
		(intr_mode << 6) | (flow_mode << 12) | (syncmode << 5);
}

static inline unsigned short get_dma_curr_irqstat(unsigned int channel)
{
	return dma_ch[channel].regs->irq_status;
}
static inline unsigned short get_dma_curr_xcount(unsigned int channel)
{
	return dma_ch[channel].regs->curr_x_count;
}
static inline unsigned short get_dma_curr_ycount(unsigned int channel)
{
	return dma_ch[channel].regs->curr_y_count;
}
static inline void *get_dma_next_desc_ptr(unsigned int channel)
{
	return dma_ch[channel].regs->next_desc_ptr;
}
static inline void *get_dma_curr_desc_ptr(unsigned int channel)
{
	return dma_ch[channel].regs->curr_desc_ptr;
}
static inline unsigned short get_dma_config(unsigned int channel)
{
	return dma_ch[channel].regs->cfg;
}
static inline unsigned long get_dma_curr_addr(unsigned int channel)
{
	return dma_ch[channel].regs->curr_addr_ptr;
}

static inline void set_dma_sg(unsigned int channel, struct dmasg *sg, int ndsize)
{
	dma_ch[channel].regs->cfg =
		(dma_ch[channel].regs->cfg & ~(0xf << 8)) |
		((ndsize & 0xf) << 8);
	dma_ch[channel].regs->next_desc_ptr = sg;
}

static inline int dma_channel_active(unsigned int channel)
{
	if (dma_ch[channel].chan_status == DMA_CHANNEL_FREE)
		return 0;
	else
		return 1;
}

static inline void disable_dma(unsigned int channel)
{
	dma_ch[channel].regs->cfg &= ~DMAEN;
	SSYNC();
	dma_ch[channel].chan_status = DMA_CHANNEL_REQUESTED;
}
static inline void enable_dma(unsigned int channel)
{
	dma_ch[channel].regs->curr_x_count = 0;
	dma_ch[channel].regs->curr_y_count = 0;
	dma_ch[channel].regs->cfg |= DMAEN;
	dma_ch[channel].chan_status = DMA_CHANNEL_ENABLED;
}
void free_dma(unsigned int channel);
int request_dma(unsigned int channel, const char *device_id);
int set_dma_callback(unsigned int channel, irq_handler_t callback, void *data);

static inline void dma_disable_irq(unsigned int channel)
{
	disable_irq(dma_ch[channel].irq);
}
static inline void dma_enable_irq(unsigned int channel)
{
	enable_irq(dma_ch[channel].irq);
}
static inline void clear_dma_irqstat(unsigned int channel)
{
	dma_ch[channel].regs->irq_status = DMA_DONE | DMA_ERR;
}

void *dma_memcpy(void *dest, const void *src, size_t count);
void *safe_dma_memcpy(void *dest, const void *src, size_t count);
void blackfin_dma_early_init(void);

#endif
