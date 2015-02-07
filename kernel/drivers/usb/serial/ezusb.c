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
 * EZ-USB specific functions used by some of the USB to Serial drivers.
 *
 * Copyright (C) 1999 - 2002 Greg Kroah-Hartman (greg@kroah.com)
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License version
 *	2 as published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/tty.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>

/* EZ-USB Control and Status Register.  Bit 0 controls 8051 reset */
#define CPUCS_REG    0x7F92

int ezusb_writememory(struct usb_serial *serial, int address,
				unsigned char *data, int length, __u8 request)
{
	int result;
	unsigned char *transfer_buffer;

	/* dbg("ezusb_writememory %x, %d", address, length); */
	if (!serial->dev) {
		printk(KERN_ERR "ezusb: %s - no physical device present, "
		       "failing.\n", __func__);
		return -ENODEV;
	}

	transfer_buffer = kmemdup(data, length, GFP_KERNEL);
	if (!transfer_buffer) {
		dev_err(&serial->dev->dev, "%s - kmalloc(%d) failed.\n",
							__func__, length);
		return -ENOMEM;
	}
	result = usb_control_msg(serial->dev, usb_sndctrlpipe(serial->dev, 0),
		     request, 0x40, address, 0, transfer_buffer, length, 3000);
	kfree(transfer_buffer);
	return result;
}
EXPORT_SYMBOL_GPL(ezusb_writememory);

int ezusb_set_reset(struct usb_serial *serial, unsigned char reset_bit)
{
	int response;

	/* dbg("%s - %d", __func__, reset_bit); */
	response = ezusb_writememory(serial, CPUCS_REG, &reset_bit, 1, 0xa0);
	if (response < 0)
		dev_err(&serial->dev->dev, "%s- %d failed\n",
						__func__, reset_bit);
	return response;
}
EXPORT_SYMBOL_GPL(ezusb_set_reset);

