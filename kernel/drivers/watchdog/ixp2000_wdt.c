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
 * drivers/char/watchdog/ixp2000_wdt.c
 *
 * Watchdog driver for Intel IXP2000 network processors
 *
 * Adapted from the IXP4xx watchdog driver by Lennert Buytenhek.
 * The original version carries these notices:
 *
 * Author: Deepak Saxena <dsaxena@plexity.net>
 *
 * Copyright 2004 (c) MontaVista, Software, Inc.
 * Based on sa1100 driver, Copyright (C) 2000 Oleg Drokin <green@crimea.edu>
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/uaccess.h>
#include <mach/hardware.h>

static int nowayout = WATCHDOG_NOWAYOUT;
static unsigned int heartbeat = 60;	/* (secs) Default is 1 minute */
static unsigned long wdt_status;
static spinlock_t wdt_lock;

#define	WDT_IN_USE		0
#define	WDT_OK_TO_CLOSE		1

static unsigned long wdt_tick_rate;

static void wdt_enable(void)
{
	spin_lock(&wdt_lock);
	ixp2000_reg_write(IXP2000_RESET0, *(IXP2000_RESET0) | WDT_RESET_ENABLE);
	ixp2000_reg_write(IXP2000_TWDE, WDT_ENABLE);
	ixp2000_reg_write(IXP2000_T4_CLD, heartbeat * wdt_tick_rate);
	ixp2000_reg_write(IXP2000_T4_CTL, TIMER_DIVIDER_256 | TIMER_ENABLE);
	spin_unlock(&wdt_lock);
}

static void wdt_disable(void)
{
	spin_lock(&wdt_lock);
	ixp2000_reg_write(IXP2000_T4_CTL, 0);
	spin_unlock(&wdt_lock);
}

static void wdt_keepalive(void)
{
	spin_lock(&wdt_lock);
	ixp2000_reg_write(IXP2000_T4_CLD, heartbeat * wdt_tick_rate);
	spin_unlock(&wdt_lock);
}

static int ixp2000_wdt_open(struct inode *inode, struct file *file)
{
	if (test_and_set_bit(WDT_IN_USE, &wdt_status))
		return -EBUSY;

	clear_bit(WDT_OK_TO_CLOSE, &wdt_status);

	wdt_enable();

	return nonseekable_open(inode, file);
}

static ssize_t ixp2000_wdt_write(struct file *file, const char *data,
						size_t len, loff_t *ppos)
{
	if (len) {
		if (!nowayout) {
			size_t i;

			clear_bit(WDT_OK_TO_CLOSE, &wdt_status);

			for (i = 0; i != len; i++) {
				char c;

				if (get_user(c, data + i))
					return -EFAULT;
				if (c == 'V')
					set_bit(WDT_OK_TO_CLOSE, &wdt_status);
			}
		}
		wdt_keepalive();
	}

	return len;
}


static struct watchdog_info ident = {
	.options	= WDIOF_MAGICCLOSE | WDIOF_SETTIMEOUT |
				WDIOF_KEEPALIVEPING,
	.identity	= "IXP2000 Watchdog",
};

static long ixp2000_wdt_ioctl(struct file *file, unsigned int cmd,
							unsigned long arg)
{
	int ret = -ENOTTY;
	int time;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		ret = copy_to_user((struct watchdog_info *)arg, &ident,
				   sizeof(ident)) ? -EFAULT : 0;
		break;

	case WDIOC_GETSTATUS:
		ret = put_user(0, (int *)arg);
		break;

	case WDIOC_GETBOOTSTATUS:
		ret = put_user(0, (int *)arg);
		break;

	case WDIOC_KEEPALIVE:
		wdt_enable();
		ret = 0;
		break;

	case WDIOC_SETTIMEOUT:
		ret = get_user(time, (int *)arg);
		if (ret)
			break;

		if (time <= 0 || time > 60) {
			ret = -EINVAL;
			break;
		}

		heartbeat = time;
		wdt_keepalive();
		/* Fall through */

	case WDIOC_GETTIMEOUT:
		ret = put_user(heartbeat, (int *)arg);
		break;
	}

	return ret;
}

static int ixp2000_wdt_release(struct inode *inode, struct file *file)
{
	if (test_bit(WDT_OK_TO_CLOSE, &wdt_status))
		wdt_disable();
	else
		printk(KERN_CRIT "WATCHDOG: Device closed unexpectedly - "
					"timer will not stop\n");
	clear_bit(WDT_IN_USE, &wdt_status);
	clear_bit(WDT_OK_TO_CLOSE, &wdt_status);

	return 0;
}


static const struct file_operations ixp2000_wdt_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.write		= ixp2000_wdt_write,
	.unlocked_ioctl	= ixp2000_wdt_ioctl,
	.open		= ixp2000_wdt_open,
	.release	= ixp2000_wdt_release,
};

static struct miscdevice ixp2000_wdt_miscdev = {
	.minor		= WATCHDOG_MINOR,
	.name		= "watchdog",
	.fops		= &ixp2000_wdt_fops,
};

static int __init ixp2000_wdt_init(void)
{
	if ((*IXP2000_PRODUCT_ID & 0x001ffef0) == 0x00000000) {
		printk(KERN_INFO "Unable to use IXP2000 watchdog due to IXP2800 erratum #25.\n");
		return -EIO;
	}
	wdt_tick_rate = (*IXP2000_T1_CLD * HZ) / 256;
	spin_lock_init(&wdt_lock);
	return misc_register(&ixp2000_wdt_miscdev);
}

static void __exit ixp2000_wdt_exit(void)
{
	misc_deregister(&ixp2000_wdt_miscdev);
}

module_init(ixp2000_wdt_init);
module_exit(ixp2000_wdt_exit);

MODULE_AUTHOR("Deepak Saxena <dsaxena@plexity.net>");
MODULE_DESCRIPTION("IXP2000 Network Processor Watchdog");

module_param(heartbeat, int, 0);
MODULE_PARM_DESC(heartbeat, "Watchdog heartbeat in seconds (default 60s)");

module_param(nowayout, int, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started");

MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);

