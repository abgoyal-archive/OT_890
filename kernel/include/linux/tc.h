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
 *	Interface to the TURBOchannel related routines.
 *
 *	Copyright (c) 1998  Harald Koerfgen
 *	Copyright (c) 2005  James Simmons
 *	Copyright (c) 2006  Maciej W. Rozycki
 *
 *	Based on:
 *
 *	"TURBOchannel Firmware Specification", EK-TCAAD-FS-004
 *
 *	from Digital Equipment Corporation.
 *
 *	This file is subject to the terms and conditions of the GNU
 *	General Public License.  See the file "COPYING" in the main
 *	directory of this archive for more details.
 */
#ifndef _LINUX_TC_H
#define _LINUX_TC_H

#include <linux/compiler.h>
#include <linux/device.h>
#include <linux/ioport.h>
#include <linux/types.h>

/*
 * Offsets for the ROM header locations for TURBOchannel cards.
 */
#define TC_OLDCARD	0x3c0000
#define TC_NEWCARD	0x000000

#define TC_ROM_WIDTH	0x3e0
#define TC_ROM_STRIDE	0x3e4
#define TC_ROM_SIZE	0x3e8
#define TC_SLOT_SIZE	0x3ec
#define TC_PATTERN0	0x3f0
#define TC_PATTERN1	0x3f4
#define TC_PATTERN2	0x3f8
#define TC_PATTERN3	0x3fc
#define TC_FIRM_VER	0x400
#define TC_VENDOR	0x420
#define TC_MODULE	0x440
#define TC_FIRM_TYPE	0x460
#define TC_FLAGS	0x470
#define TC_ROM_OBJECTS	0x480

/*
 * Information obtained through the get_tcinfo() PROM call.
 */
struct tcinfo {
	s32		revision;	/* Hardware revision level. */
	s32		clk_period;	/* Clock period in nanoseconds. */
	s32		slot_size;	/* Slot size in megabytes. */
	s32		io_timeout;	/* I/O timeout in cycles. */
	s32		dma_range;	/* DMA address range in megabytes. */
	s32		max_dma_burst;	/* Maximum DMA burst length. */
	s32		parity;		/* System module supports TC parity. */
	s32		reserved[4];
};

/*
 * TURBOchannel bus.
 */
struct tc_bus {
	struct list_head devices;	/* List of devices on this bus. */
	struct resource	resource[2];	/* Address space routed to this bus. */

	struct device	dev;
	char		name[13];
	resource_size_t	slot_base;
	resource_size_t	ext_slot_base;
	resource_size_t	ext_slot_size;
	int		num_tcslots;
	struct tcinfo	info;
};

/*
 * TURBOchannel device.
 */
struct tc_dev {
	struct list_head node;		/* Node in list of all TC devices. */
	struct tc_bus	*bus;		/* Bus this device is on. */
	struct tc_driver *driver;	/* Which driver has allocated this
					   device. */
	struct device	dev;		/* Generic device interface. */
	struct resource	resource;	/* Address space of this device. */
	char		vendor[9];
	char		name[9];
	char		firmware[9];
	int		interrupt;
	int		slot;
};

#define to_tc_dev(n) container_of(n, struct tc_dev, dev)

struct tc_device_id {
	char		vendor[9];
	char		name[9];
};

/*
 * TURBOchannel driver.
 */
struct tc_driver {
	struct list_head node;
	const struct tc_device_id *id_table;
	struct device_driver driver;
};

#define to_tc_driver(drv) container_of(drv, struct tc_driver, driver)

/*
 * Return TURBOchannel clock frequency in Hz.
 */
static inline unsigned long tc_get_speed(struct tc_bus *tbus)
{
	return 100000 * (10000 / (unsigned long)tbus->info.clk_period);
}

#ifdef CONFIG_TC

extern struct bus_type tc_bus_type;

extern int tc_register_driver(struct tc_driver *tdrv);
extern void tc_unregister_driver(struct tc_driver *tdrv);

#else /* !CONFIG_TC */

static inline int tc_register_driver(struct tc_driver *tdrv) { return 0; }
static inline void tc_unregister_driver(struct tc_driver *tdrv) { }

#endif /* CONFIG_TC */

/*
 * These have to be provided by the architecture.
 */
extern int tc_preadb(u8 *valp, void __iomem *addr);
extern int tc_bus_get_info(struct tc_bus *tbus);
extern void tc_device_get_irq(struct tc_dev *tdev);

#endif /* _LINUX_TC_H */
