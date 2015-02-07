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
 * arch/sh/boards/landisk/gio.c - driver for landisk
 *
 * This driver will also support the I-O DATA Device, Inc. LANDISK Board.
 * LANDISK and USL-5P Button, LED and GIO driver drive function.
 *
 *   Copylight (C) 2006 kogiidena
 *   Copylight (C) 2002 Atom Create Engineering Co., Ltd. *
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/smp_lock.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <mach-landisk/mach/gio.h>
#include <mach-landisk/mach/iodata_landisk.h>

#define DEVCOUNT                4
#define GIO_MINOR	        2	/* GIO minor no. */

static dev_t dev;
static struct cdev *cdev_p;
static int openCnt;

static int gio_open(struct inode *inode, struct file *filp)
{
	int minor;
	int ret = -ENOENT;

	lock_kernel();
	minor = MINOR(inode->i_rdev);
	if (minor < DEVCOUNT) {
		if (openCnt > 0) {
			ret = -EALREADY;
		} else {
			openCnt++;
			ret = 0;
		}
	}
	unlock_kernel();
	return ret;
}

static int gio_close(struct inode *inode, struct file *filp)
{
	int minor;

	minor = MINOR(inode->i_rdev);
	if (minor < DEVCOUNT) {
		openCnt--;
	}
	return 0;
}

static int gio_ioctl(struct inode *inode, struct file *filp,
			     unsigned int cmd, unsigned long arg)
{
	unsigned int data;
	static unsigned int addr = 0;

	if (cmd & 0x01) {	/* write */
		if (copy_from_user(&data, (int *)arg, sizeof(int))) {
			return -EFAULT;
		}
	}

	switch (cmd) {
	case GIODRV_IOCSGIOSETADDR:	/* address set */
		addr = data;
		break;

	case GIODRV_IOCSGIODATA1:	/* write byte */
		ctrl_outb((unsigned char)(0x0ff & data), addr);
		break;

	case GIODRV_IOCSGIODATA2:	/* write word */
		if (addr & 0x01) {
			return -EFAULT;
		}
		ctrl_outw((unsigned short int)(0x0ffff & data), addr);
		break;

	case GIODRV_IOCSGIODATA4:	/* write long */
		if (addr & 0x03) {
			return -EFAULT;
		}
		ctrl_outl(data, addr);
		break;

	case GIODRV_IOCGGIODATA1:	/* read byte */
		data = ctrl_inb(addr);
		break;

	case GIODRV_IOCGGIODATA2:	/* read word */
		if (addr & 0x01) {
			return -EFAULT;
		}
		data = ctrl_inw(addr);
		break;

	case GIODRV_IOCGGIODATA4:	/* read long */
		if (addr & 0x03) {
			return -EFAULT;
		}
		data = ctrl_inl(addr);
		break;
	default:
		return -EFAULT;
		break;
	}

	if ((cmd & 0x01) == 0) {	/* read */
		if (copy_to_user((int *)arg, &data, sizeof(int))) {
			return -EFAULT;
		}
	}
	return 0;
}

static const struct file_operations gio_fops = {
	.owner = THIS_MODULE,
	.open = gio_open,	/* open */
	.release = gio_close,	/* release */
	.ioctl = gio_ioctl,	/* ioctl */
};

static int __init gio_init(void)
{
	int error;

	printk(KERN_INFO "gio: driver initialized\n");

	openCnt = 0;

	if ((error = alloc_chrdev_region(&dev, 0, DEVCOUNT, "gio")) < 0) {
		printk(KERN_ERR
		       "gio: Couldn't alloc_chrdev_region, error=%d\n",
		       error);
		return 1;
	}

	cdev_p = cdev_alloc();
	cdev_p->ops = &gio_fops;
	error = cdev_add(cdev_p, dev, DEVCOUNT);
	if (error) {
		printk(KERN_ERR
		       "gio: Couldn't cdev_add, error=%d\n", error);
		return 1;
	}

	return 0;
}

static void __exit gio_exit(void)
{
	cdev_del(cdev_p);
	unregister_chrdev_region(dev, DEVCOUNT);
}

module_init(gio_init);
module_exit(gio_exit);

MODULE_LICENSE("GPL");
