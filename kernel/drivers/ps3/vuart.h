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
 *  PS3 virtual uart
 *
 *  Copyright (C) 2006 Sony Computer Entertainment Inc.
 *  Copyright 2006 Sony Corp.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#if !defined(_PS3_VUART_H)
#define _PS3_VUART_H

#include <asm/ps3.h>

struct ps3_vuart_stats {
	unsigned long bytes_written;
	unsigned long bytes_read;
	unsigned long tx_interrupts;
	unsigned long rx_interrupts;
	unsigned long disconnect_interrupts;
};

struct ps3_vuart_work {
	struct work_struct work;
	unsigned long trigger;
	struct ps3_system_bus_device *dev; /* to convert work to device */
};

/**
 * struct ps3_vuart_port_driver - a driver for a device on a vuart port
 */

struct ps3_vuart_port_driver {
	struct ps3_system_bus_driver core;
	int (*probe)(struct ps3_system_bus_device *);
	int (*remove)(struct ps3_system_bus_device *);
	void (*shutdown)(struct ps3_system_bus_device *);
	void (*work)(struct ps3_system_bus_device *);
	/* int (*tx_event)(struct ps3_system_bus_device *dev); */
	/* int (*rx_event)(struct ps3_system_bus_device *dev); */
	/* int (*disconnect_event)(struct ps3_system_bus_device *dev); */
	/* int (*suspend)(struct ps3_system_bus_device *, pm_message_t); */
	/* int (*resume)(struct ps3_system_bus_device *); */
};

int ps3_vuart_port_driver_register(struct ps3_vuart_port_driver *drv);
void ps3_vuart_port_driver_unregister(struct ps3_vuart_port_driver *drv);

static inline struct ps3_vuart_port_driver *
	ps3_system_bus_dev_to_vuart_drv(struct ps3_system_bus_device *_dev)
{
	struct ps3_system_bus_driver *sbd =
		ps3_system_bus_dev_to_system_bus_drv(_dev);
	BUG_ON(!sbd);
	return container_of(sbd, struct ps3_vuart_port_driver, core);
}
static inline struct ps3_system_bus_device *ps3_vuart_work_to_system_bus_dev(
	struct work_struct *_work)
{
	struct ps3_vuart_work *vw = container_of(_work, struct ps3_vuart_work,
		work);
	return vw->dev;
}

int ps3_vuart_write(struct ps3_system_bus_device *dev, const void *buf,
	unsigned int bytes);
int ps3_vuart_read(struct ps3_system_bus_device *dev, void *buf,
	unsigned int bytes);
int ps3_vuart_read_async(struct ps3_system_bus_device *dev, unsigned int bytes);
void ps3_vuart_cancel_async(struct ps3_system_bus_device *dev);
void ps3_vuart_clear_rx_bytes(struct ps3_system_bus_device *dev,
	unsigned int bytes);

#endif
