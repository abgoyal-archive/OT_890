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
 *  Copyright (C) 2005 by Basler Vision Technologies AG
 *  Author: Thomas Koeller <thomas.koeller@baslerweb.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
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

#include <linux/compiler.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/smp_lock.h>

#include "excite_iodev.h"



static const struct resource *iodev_get_resource(struct platform_device *, const char *, unsigned int);
static int __init iodev_probe(struct device *);
static int __exit iodev_remove(struct device *);
static int iodev_open(struct inode *, struct file *);
static int iodev_release(struct inode *, struct file *);
static ssize_t iodev_read(struct file *, char __user *, size_t s, loff_t *);
static unsigned int iodev_poll(struct file *, struct poll_table_struct *);
static irqreturn_t iodev_irqhdl(int, void *);



static const char iodev_name[] = "iodev";
static unsigned int iodev_irq;
static DECLARE_WAIT_QUEUE_HEAD(wq);



static const struct file_operations fops =
{
	.owner		= THIS_MODULE,
	.open		= iodev_open,
	.release	= iodev_release,
	.read		= iodev_read,
	.poll		= iodev_poll
};

static struct miscdevice miscdev =
{
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= iodev_name,
	.fops		= &fops
};

static struct device_driver iodev_driver =
{
	.name		= (char *) iodev_name,
	.bus		= &platform_bus_type,
	.owner		= THIS_MODULE,
	.probe		= iodev_probe,
	.remove		= __exit_p(iodev_remove)
};



static const struct resource *
iodev_get_resource(struct platform_device *pdv, const char *name,
		     unsigned int type)
{
	char buf[80];
	if (snprintf(buf, sizeof buf, "%s_0", name) >= sizeof buf)
		return NULL;
	return platform_get_resource_byname(pdv, type, buf);
}



/* No hotplugging on the platform bus - use __init */
static int __init iodev_probe(struct device *dev)
{
	struct platform_device * const pdv = to_platform_device(dev);
	const struct resource * const ri =
		iodev_get_resource(pdv, IODEV_RESOURCE_IRQ, IORESOURCE_IRQ);

	if (unlikely(!ri))
		return -ENXIO;

	iodev_irq = ri->start;
	return misc_register(&miscdev);
}



static int __exit iodev_remove(struct device *dev)
{
	return misc_deregister(&miscdev);
}

static int iodev_open(struct inode *i, struct file *f)
{
	int ret;

	lock_kernel();
	ret = request_irq(iodev_irq, iodev_irqhdl, IRQF_DISABLED,
			   iodev_name, &miscdev);
	unlock_kernel();

	return ret;
}

static int iodev_release(struct inode *i, struct file *f)
{
	free_irq(iodev_irq, &miscdev);
	return 0;
}




static ssize_t
iodev_read(struct file *f, char __user *d, size_t s, loff_t *o)
{
	ssize_t ret;
	DEFINE_WAIT(w);

	prepare_to_wait(&wq, &w, TASK_INTERRUPTIBLE);
	if (!signal_pending(current))
		schedule();
	ret = signal_pending(current) ? -ERESTARTSYS : 0;
	finish_wait(&wq, &w);
	return ret;
}


static unsigned int iodev_poll(struct file *f, struct poll_table_struct *p)
{
	poll_wait(f, &wq, p);
	return POLLOUT | POLLWRNORM;
}

static irqreturn_t iodev_irqhdl(int irq, void *ctxt)
{
	wake_up(&wq);

	return IRQ_HANDLED;
}

static int __init iodev_init_module(void)
{
	return driver_register(&iodev_driver);
}



static void __exit iodev_cleanup_module(void)
{
	driver_unregister(&iodev_driver);
}

module_init(iodev_init_module);
module_exit(iodev_cleanup_module);



MODULE_AUTHOR("Thomas Koeller <thomas.koeller@baslerweb.com>");
MODULE_DESCRIPTION("Basler eXcite i/o interrupt handler");
MODULE_VERSION("0.0");
MODULE_LICENSE("GPL");
