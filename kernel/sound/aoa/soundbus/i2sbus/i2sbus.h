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
 * i2sbus driver -- private definitions
 *
 * Copyright 2006 Johannes Berg <johannes@sipsolutions.net>
 *
 * GPL v2, can be found in COPYING.
 */
#ifndef __I2SBUS_H
#define __I2SBUS_H
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <linux/completion.h>

#include <sound/pcm.h>

#include <asm/prom.h>
#include <asm/pmac_feature.h>
#include <asm/dbdma.h>

#include "interface.h"
#include "../soundbus.h"

struct i2sbus_control {
	struct list_head list;
	struct macio_chip *macio;
};

#define MAX_DBDMA_COMMANDS	32

struct dbdma_command_mem {
	dma_addr_t bus_addr;
	dma_addr_t bus_cmd_start;
	struct dbdma_cmd *cmds;
	void *space;
	int size;
	u32 running:1;
	u32 stopping:1;
};

struct pcm_info {
	u32 created:1, /* has this direction been created with alsa? */
	    active:1;  /* is this stream active? */
	/* runtime information */
	struct snd_pcm_substream *substream;
	int current_period;
	u32 frame_count;
	struct dbdma_command_mem dbdma_ring;
	volatile struct dbdma_regs __iomem *dbdma;
	struct completion *stop_completion;
};

enum {
	aoa_resource_i2smmio = 0,
	aoa_resource_txdbdma,
	aoa_resource_rxdbdma,
};

struct i2sbus_dev {
	struct soundbus_dev sound;
	struct macio_dev *macio;
	struct i2sbus_control *control;
	volatile struct i2s_interface_regs __iomem *intfregs;

	struct resource resources[3];
	struct resource *allocated_resource[3];
	int interrupts[3];
	char rnames[3][32];

	/* info about currently active substreams */
	struct pcm_info out, in;
	snd_pcm_format_t format;
	unsigned int rate;

	/* list for a single controller */
	struct list_head item;
	/* number of bus on controller */
	int bus_number;
	/* for use by control layer */
	struct pmf_function *enable,
			    *cell_enable,
			    *cell_disable,
			    *clock_enable,
			    *clock_disable;

	/* locks */
	/* spinlock for low-level interrupt locking */
	spinlock_t low_lock;
	/* mutex for high-level consistency */
	struct mutex lock;
};

#define soundbus_dev_to_i2sbus_dev(sdev) \
		container_of(sdev, struct i2sbus_dev, sound)

/* pcm specific functions */
extern int
i2sbus_attach_codec(struct soundbus_dev *dev, struct snd_card *card,
		    struct codec_info *ci, void *data);
extern void
i2sbus_detach_codec(struct soundbus_dev *dev, void *data);
extern irqreturn_t
i2sbus_tx_intr(int irq, void *devid);
extern irqreturn_t
i2sbus_rx_intr(int irq, void *devid);

extern void i2sbus_wait_for_stop_both(struct i2sbus_dev *i2sdev);
extern void i2sbus_pcm_prepare_both(struct i2sbus_dev *i2sdev);

/* control specific functions */
extern int i2sbus_control_init(struct macio_dev* dev,
			       struct i2sbus_control **c);
extern void i2sbus_control_destroy(struct i2sbus_control *c);
extern int i2sbus_control_add_dev(struct i2sbus_control *c,
				  struct i2sbus_dev *i2sdev);
extern void i2sbus_control_remove_dev(struct i2sbus_control *c,
				      struct i2sbus_dev *i2sdev);
extern int i2sbus_control_enable(struct i2sbus_control *c,
				 struct i2sbus_dev *i2sdev);
extern int i2sbus_control_cell(struct i2sbus_control *c,
			       struct i2sbus_dev *i2sdev,
			       int enable);
extern int i2sbus_control_clock(struct i2sbus_control *c,
				struct i2sbus_dev *i2sdev,
				int enable);
#endif /* __I2SBUS_H */
