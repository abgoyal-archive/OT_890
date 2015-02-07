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
 * Intel Wireless UWB Link 1480
 * Event Size tables for Wired Adaptors
 *
 * Copyright (C) 2005-2006 Intel Corporation
 * Inaky Perez-Gonzalez <inaky.perez-gonzalez@intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 *
 * FIXME: docs
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/uwb.h>
#include "dfu/i1480-dfu.h"


/** Event size table for wEvents 0x00XX */
static struct uwb_est_entry i1480_est_fd00[] = {
	/* Anybody expecting this response has to use
	 * neh->extra_size to specify the real size that will
	 * come back. */
	[i1480_EVT_CONFIRM] = { .size = sizeof(struct i1480_evt_confirm) },
	[i1480_CMD_SET_IP_MAS] = { .size = sizeof(struct i1480_evt_confirm) },
#ifdef i1480_RCEB_EXTENDED
	[0x09] = {
		.size = sizeof(struct i1480_rceb),
		.offset = 1 + offsetof(struct i1480_rceb, wParamLength),
	},
#endif
};

/** Event size table for wEvents 0x01XX */
static struct uwb_est_entry i1480_est_fd01[] = {
	[0xff & i1480_EVT_RM_INIT_DONE] = { .size = sizeof(struct i1480_rceb) },
	[0xff & i1480_EVT_DEV_ADD] = { .size = sizeof(struct i1480_rceb) + 9 },
	[0xff & i1480_EVT_DEV_RM] = { .size = sizeof(struct i1480_rceb) + 9 },
	[0xff & i1480_EVT_DEV_ID_CHANGE] = {
		.size = sizeof(struct i1480_rceb) + 2 },
};

static int i1480_est_init(void)
{
	int result = uwb_est_register(i1480_CET_VS1, 0x00, 0x8086, 0x0c3b,
				      i1480_est_fd00,
				      ARRAY_SIZE(i1480_est_fd00));
	if (result < 0) {
		printk(KERN_ERR "Can't register EST table fd00: %d\n", result);
		return result;
	}
	result = uwb_est_register(i1480_CET_VS1, 0x01, 0x8086, 0x0c3b,
				  i1480_est_fd01, ARRAY_SIZE(i1480_est_fd01));
	if (result < 0) {
		printk(KERN_ERR "Can't register EST table fd01: %d\n", result);
		return result;
	}
	return 0;
}
module_init(i1480_est_init);

static void i1480_est_exit(void)
{
	uwb_est_unregister(i1480_CET_VS1, 0x00, 0x8086, 0x0c3b,
			   i1480_est_fd00, ARRAY_SIZE(i1480_est_fd00));
	uwb_est_unregister(i1480_CET_VS1, 0x01, 0x8086, 0x0c3b,
			   i1480_est_fd01, ARRAY_SIZE(i1480_est_fd01));
}
module_exit(i1480_est_exit);

MODULE_AUTHOR("Inaky Perez-Gonzalez <inaky.perez-gonzalez@intel.com>");
MODULE_DESCRIPTION("i1480's Vendor Specific Event Size Tables");
MODULE_LICENSE("GPL");

/**
 * USB device ID's that we handle
 *
 * [so we are loaded when this kind device is connected]
 */
static struct usb_device_id i1480_est_id_table[] = {
	{ USB_DEVICE(0x8086, 0xdf3b), },
	{ USB_DEVICE(0x8086, 0x0c3b), },
	{ },
};
MODULE_DEVICE_TABLE(usb, i1480_est_id_table);
