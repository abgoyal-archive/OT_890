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
 *    EISA "eeprom" support routines
 *
 *    Copyright (C) 2001 Thomas Bogendoerfer <tsbogend at parisc-linux.org>
 *
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/smp_lock.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/eisa_eeprom.h>

#define 	EISA_EEPROM_MINOR 241

static loff_t eisa_eeprom_llseek(struct file *file, loff_t offset, int origin )
{
	switch (origin) {
	  case 0:
		/* nothing to do */
		break;
	  case 1:
		offset += file->f_pos;
		break;
	  case 2:
		offset += HPEE_MAX_LENGTH;
		break;
	}
	return (offset >= 0 && offset < HPEE_MAX_LENGTH) ? (file->f_pos = offset) : -EINVAL;
}

static ssize_t eisa_eeprom_read(struct file * file,
			      char __user *buf, size_t count, loff_t *ppos )
{
	unsigned char *tmp;
	ssize_t ret;
	int i;
	
	if (*ppos >= HPEE_MAX_LENGTH)
		return 0;
	
	count = *ppos + count < HPEE_MAX_LENGTH ? count : HPEE_MAX_LENGTH - *ppos;
	tmp = kmalloc(count, GFP_KERNEL);
	if (tmp) {
		for (i = 0; i < count; i++)
			tmp[i] = readb(eisa_eeprom_addr+(*ppos)++);

		if (copy_to_user (buf, tmp, count))
			ret = -EFAULT;
		else
			ret = count;
		kfree (tmp);
	} else
		ret = -ENOMEM;
	
	return ret;
}

static int eisa_eeprom_ioctl(struct inode *inode, struct file *file, 
			   unsigned int cmd,
			   unsigned long arg)
{
	return -ENOTTY;
}

static int eisa_eeprom_open(struct inode *inode, struct file *file)
{
	cycle_kernel_lock();

	if (file->f_mode & FMODE_WRITE)
		return -EINVAL;
   
	return 0;
}

static int eisa_eeprom_release(struct inode *inode, struct file *file)
{
	return 0;
}

/*
 *	The various file operations we support.
 */
static const struct file_operations eisa_eeprom_fops = {
	.owner =	THIS_MODULE,
	.llseek =	eisa_eeprom_llseek,
	.read =		eisa_eeprom_read,
	.ioctl =	eisa_eeprom_ioctl,
	.open =		eisa_eeprom_open,
	.release =	eisa_eeprom_release,
};

static struct miscdevice eisa_eeprom_dev = {
	EISA_EEPROM_MINOR,
	"eisa_eeprom",
	&eisa_eeprom_fops
};

static int __init eisa_eeprom_init(void)
{
	int retval;

	if (!eisa_eeprom_addr)
		return -ENODEV;

	retval = misc_register(&eisa_eeprom_dev);
	if (retval < 0) {
		printk(KERN_ERR "EISA EEPROM: cannot register misc device.\n");
		return retval;
	}

	printk(KERN_INFO "EISA EEPROM at 0x%p\n", eisa_eeprom_addr);
	return 0;
}

MODULE_LICENSE("GPL");

module_init(eisa_eeprom_init);
