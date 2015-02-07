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
 *  include/linux/mmc/sdio_func.h
 *
 *  Copyright 2007-2008 Pierre Ossman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */

#ifndef MMC_SDIO_FUNC_H
#define MMC_SDIO_FUNC_H

#include <linux/device.h>
#include <linux/mod_devicetable.h>

struct mmc_card;
struct sdio_func;

typedef void (sdio_irq_handler_t)(struct sdio_func *);

/*
 * Structure used to hold embedded SDIO device data from platform layer
 */
struct sdio_embedded_func {
	uint8_t f_class;
	uint32_t f_maxblksize;
};

/*
 * SDIO function CIS tuple (unknown to the core)
 */
struct sdio_func_tuple {
	struct sdio_func_tuple *next;
	unsigned char code;
	unsigned char size;
	unsigned char data[0];
};

/*
 * SDIO function devices
 */
struct sdio_func {
	struct mmc_card		*card;		/* the card this device belongs to */
	struct device		dev;		/* the device */
	sdio_irq_handler_t	*irq_handler;	/* IRQ callback */
	unsigned int		num;		/* function number */

	unsigned char		class;		/* standard interface class */
	unsigned short		vendor;		/* vendor id */
	unsigned short		device;		/* device id */

	unsigned		max_blksize;	/* maximum block size */
	unsigned		cur_blksize;	/* current block size */

	unsigned		enable_timeout;	/* max enable timeout in msec */

	unsigned int		state;		/* function state */
#define SDIO_STATE_PRESENT	(1<<0)		/* present in sysfs */

	u8			tmpbuf[4];	/* DMA:able scratch buffer */

	unsigned		num_info;	/* number of info strings */
	const char		**info;		/* info strings */

	struct sdio_func_tuple *tuples;
};

#define sdio_func_present(f)	((f)->state & SDIO_STATE_PRESENT)

#define sdio_func_set_present(f) ((f)->state |= SDIO_STATE_PRESENT)

#define sdio_func_id(f)		(dev_name(&(f)->dev))

#define sdio_get_drvdata(f)	dev_get_drvdata(&(f)->dev)
#define sdio_set_drvdata(f,d)	dev_set_drvdata(&(f)->dev, d)

/*
 * SDIO function device driver
 */
struct sdio_driver {
	char *name;
	const struct sdio_device_id *id_table;

	int (*probe)(struct sdio_func *, const struct sdio_device_id *);
	void (*remove)(struct sdio_func *);

	struct device_driver drv;
};

/**
 * SDIO_DEVICE - macro used to describe a specific SDIO device
 * @vend: the 16 bit manufacturer code
 * @dev: the 16 bit function id
 *
 * This macro is used to create a struct sdio_device_id that matches a
 * specific device. The class field will be set to SDIO_ANY_ID.
 */
#define SDIO_DEVICE(vend,dev) \
	.class = SDIO_ANY_ID, \
	.vendor = (vend), .device = (dev)

/**
 * SDIO_DEVICE_CLASS - macro used to describe a specific SDIO device class
 * @dev_class: the 8 bit standard interface code
 *
 * This macro is used to create a struct sdio_device_id that matches a
 * specific standard SDIO function type.  The vendor and device fields will
 * be set to SDIO_ANY_ID.
 */
#define SDIO_DEVICE_CLASS(dev_class) \
	.class = (dev_class), \
	.vendor = SDIO_ANY_ID, .device = SDIO_ANY_ID

extern int sdio_register_driver(struct sdio_driver *);
extern void sdio_unregister_driver(struct sdio_driver *);

/*
 * SDIO I/O operations
 */
extern void sdio_claim_host(struct sdio_func *func);
extern void sdio_release_host(struct sdio_func *func);

extern int sdio_enable_func(struct sdio_func *func);
extern int sdio_disable_func(struct sdio_func *func);

extern int sdio_set_block_size(struct sdio_func *func, unsigned blksz);

extern int sdio_claim_irq(struct sdio_func *func, sdio_irq_handler_t *handler);
extern int sdio_release_irq(struct sdio_func *func);

extern unsigned int sdio_align_size(struct sdio_func *func, unsigned int sz);

extern u8 sdio_readb(struct sdio_func *func, unsigned int addr, int *err_ret);
extern u8 sdio_readb_ext(struct sdio_func *func, unsigned int addr, int *err_ret,
	unsigned in);
extern u16 sdio_readw(struct sdio_func *func, unsigned int addr, int *err_ret);
extern u32 sdio_readl(struct sdio_func *func, unsigned int addr, int *err_ret);

extern int sdio_memcpy_fromio(struct sdio_func *func, void *dst,
	unsigned int addr, int count);
extern int sdio_readsb(struct sdio_func *func, void *dst,
	unsigned int addr, int count);

extern void sdio_writeb(struct sdio_func *func, u8 b,
	unsigned int addr, int *err_ret);
extern void sdio_writew(struct sdio_func *func, u16 b,
	unsigned int addr, int *err_ret);
extern void sdio_writel(struct sdio_func *func, u32 b,
	unsigned int addr, int *err_ret);

extern int sdio_memcpy_toio(struct sdio_func *func, unsigned int addr,
	void *src, int count);
extern int sdio_writesb(struct sdio_func *func, unsigned int addr,
	void *src, int count);

extern unsigned char sdio_f0_readb(struct sdio_func *func,
	unsigned int addr, int *err_ret);
extern void sdio_f0_writeb(struct sdio_func *func, unsigned char b,
	unsigned int addr, int *err_ret);

#endif

