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
 *  include/asm-ppc/pmac_low_i2c.h
 *
 *  Copyright (C) 2003 Ben. Herrenschmidt (benh@kernel.crashing.org)
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 *
 */
#ifndef __PMAC_LOW_I2C_H__
#define __PMAC_LOW_I2C_H__
#ifdef __KERNEL__

/* i2c mode (based on the platform functions format) */
enum {
	pmac_i2c_mode_dumb	= 1,
	pmac_i2c_mode_std	= 2,
	pmac_i2c_mode_stdsub	= 3,
	pmac_i2c_mode_combined	= 4,
};

/* RW bit in address */
enum {
	pmac_i2c_read		= 0x01,
	pmac_i2c_write		= 0x00
};

/* i2c bus type */
enum {
	pmac_i2c_bus_keywest	= 0,
	pmac_i2c_bus_pmu	= 1,
	pmac_i2c_bus_smu	= 2,
};

/* i2c bus features */
enum {
	/* can_largesub : supports >1 byte subaddresses (SMU only) */
	pmac_i2c_can_largesub	= 0x00000001u,

	/* multibus : device node holds multiple busses, bus number is
	 * encoded in bits 0xff00 of "reg" of a given device
	 */
	pmac_i2c_multibus	= 0x00000002u,
};

/* i2c busses in the system */
struct pmac_i2c_bus;
struct i2c_adapter;

/* Init, called early during boot */
extern int pmac_i2c_init(void);

/* Lookup an i2c bus for a device-node. The node can be either the bus
 * node itself or a device below it. In the case of a multibus, the bus
 * node itself is the controller node, else, it's a child of the controller
 * node
 */
extern struct pmac_i2c_bus *pmac_i2c_find_bus(struct device_node *node);

/* Get the address for an i2c device. This strips the bus number if
 * necessary. The 7 bits address is returned 1 bit right shifted so that the
 * direction can be directly ored in
 */
extern u8 pmac_i2c_get_dev_addr(struct device_node *device);

/* Get infos about a bus */
extern struct device_node *pmac_i2c_get_controller(struct pmac_i2c_bus *bus);
extern struct device_node *pmac_i2c_get_bus_node(struct pmac_i2c_bus *bus);
extern int pmac_i2c_get_type(struct pmac_i2c_bus *bus);
extern int pmac_i2c_get_flags(struct pmac_i2c_bus *bus);
extern int pmac_i2c_get_channel(struct pmac_i2c_bus *bus);

/* i2c layer adapter attach/detach */
extern void pmac_i2c_attach_adapter(struct pmac_i2c_bus *bus,
				    struct i2c_adapter *adapter);
extern void pmac_i2c_detach_adapter(struct pmac_i2c_bus *bus,
				    struct i2c_adapter *adapter);
extern struct i2c_adapter *pmac_i2c_get_adapter(struct pmac_i2c_bus *bus);
extern struct pmac_i2c_bus *pmac_i2c_adapter_to_bus(struct i2c_adapter *adapter);

/* March a device or bus with an i2c adapter structure, to be used by drivers
 * to match device-tree nodes with i2c adapters during adapter discovery
 * callbacks
 */
extern int pmac_i2c_match_adapter(struct device_node *dev,
				  struct i2c_adapter *adapter);


/* (legacy) Locking functions exposed to i2c-keywest */
extern int pmac_low_i2c_lock(struct device_node *np);
extern int pmac_low_i2c_unlock(struct device_node *np);

/* Access functions for platform code */
extern int pmac_i2c_open(struct pmac_i2c_bus *bus, int polled);
extern void pmac_i2c_close(struct pmac_i2c_bus *bus);
extern int pmac_i2c_setmode(struct pmac_i2c_bus *bus, int mode);
extern int pmac_i2c_xfer(struct pmac_i2c_bus *bus, u8 addrdir, int subsize,
			 u32 subaddr, u8 *data,  int len);

/* Suspend/resume code called by via-pmu directly for now */
extern void pmac_pfunc_i2c_suspend(void);
extern void pmac_pfunc_i2c_resume(void);

#endif /* __KERNEL__ */
#endif /* __PMAC_LOW_I2C_H__ */
