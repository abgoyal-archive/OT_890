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
 * Navman Serial USB driver
 *
 * Copyright (C) 2006 Greg Kroah-Hartman <gregkh@suse.de>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	version 2 as published by the Free Software Foundation.
 *
 * TODO:
 *	Add termios method that uses copy_hw but also kills all echo
 *	flags as the navman is rx only so cannot echo.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>

static int debug;

static struct usb_device_id id_table [] = {
	{ USB_DEVICE(0x0a99, 0x0001) },	/* Talon Technology device */
	{ },
};
MODULE_DEVICE_TABLE(usb, id_table);

static struct usb_driver navman_driver = {
	.name =		"navman",
	.probe =	usb_serial_probe,
	.disconnect =	usb_serial_disconnect,
	.id_table =	id_table,
	.no_dynamic_id = 	1,
};

static void navman_read_int_callback(struct urb *urb)
{
	struct usb_serial_port *port = urb->context;
	unsigned char *data = urb->transfer_buffer;
	struct tty_struct *tty;
	int status = urb->status;
	int result;

	switch (status) {
	case 0:
		/* success */
		break;
	case -ECONNRESET:
	case -ENOENT:
	case -ESHUTDOWN:
		/* this urb is terminated, clean up */
		dbg("%s - urb shutting down with status: %d",
		    __func__, status);
		return;
	default:
		dbg("%s - nonzero urb status received: %d",
		    __func__, status);
		goto exit;
	}

	usb_serial_debug_data(debug, &port->dev, __func__,
			      urb->actual_length, data);

	tty = tty_port_tty_get(&port->port);
	if (tty && urb->actual_length) {
		tty_buffer_request_room(tty, urb->actual_length);
		tty_insert_flip_string(tty, data, urb->actual_length);
		tty_flip_buffer_push(tty);
	}
	tty_kref_put(tty);

exit:
	result = usb_submit_urb(urb, GFP_ATOMIC);
	if (result)
		dev_err(&urb->dev->dev,
			"%s - Error %d submitting interrupt urb\n",
			__func__, result);
}

static int navman_open(struct tty_struct *tty,
			struct usb_serial_port *port, struct file *filp)
{
	int result = 0;

	dbg("%s - port %d", __func__, port->number);

	if (port->interrupt_in_urb) {
		dbg("%s - adding interrupt input for treo", __func__);
		result = usb_submit_urb(port->interrupt_in_urb, GFP_KERNEL);
		if (result)
			dev_err(&port->dev,
				"%s - failed submitting interrupt urb, error %d\n",
				__func__, result);
	}
	return result;
}

static void navman_close(struct tty_struct *tty,
			struct usb_serial_port *port, struct file *filp)
{
	dbg("%s - port %d", __func__, port->number);

	usb_kill_urb(port->interrupt_in_urb);
}

static int navman_write(struct tty_struct *tty, struct usb_serial_port *port,
			const unsigned char *buf, int count)
{
	dbg("%s - port %d", __func__, port->number);

	/*
	 * This device can't write any data, only read from the device
	 */
	return -EOPNOTSUPP;
}

static struct usb_serial_driver navman_device = {
	.driver = {
		.owner =	THIS_MODULE,
		.name =		"navman",
	},
	.id_table =		id_table,
	.usb_driver =		&navman_driver,
	.num_ports =		1,
	.open =			navman_open,
	.close = 		navman_close,
	.write = 		navman_write,
	.read_int_callback =	navman_read_int_callback,
};

static int __init navman_init(void)
{
	int retval;

	retval = usb_serial_register(&navman_device);
	if (retval)
		return retval;
	retval = usb_register(&navman_driver);
	if (retval)
		usb_serial_deregister(&navman_device);
	return retval;
}

static void __exit navman_exit(void)
{
	usb_deregister(&navman_driver);
	usb_serial_deregister(&navman_device);
}

module_init(navman_init);
module_exit(navman_exit);
MODULE_LICENSE("GPL");

module_param(debug, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug, "Debug enabled or not");
