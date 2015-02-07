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
 *  ISA Plug & Play support
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/isapnp.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/smp_lock.h>
#include <asm/uaccess.h>

extern struct pnp_protocol isapnp_protocol;

static struct proc_dir_entry *isapnp_proc_bus_dir = NULL;

static loff_t isapnp_proc_bus_lseek(struct file *file, loff_t off, int whence)
{
	loff_t new = -1;

	lock_kernel();
	switch (whence) {
	case 0:
		new = off;
		break;
	case 1:
		new = file->f_pos + off;
		break;
	case 2:
		new = 256 + off;
		break;
	}
	if (new < 0 || new > 256) {
		unlock_kernel();
		return -EINVAL;
	}
	unlock_kernel();
	return (file->f_pos = new);
}

static ssize_t isapnp_proc_bus_read(struct file *file, char __user * buf,
				    size_t nbytes, loff_t * ppos)
{
	struct inode *ino = file->f_path.dentry->d_inode;
	struct proc_dir_entry *dp = PDE(ino);
	struct pnp_dev *dev = dp->data;
	int pos = *ppos;
	int cnt, size = 256;

	if (pos >= size)
		return 0;
	if (nbytes >= size)
		nbytes = size;
	if (pos + nbytes > size)
		nbytes = size - pos;
	cnt = nbytes;

	if (!access_ok(VERIFY_WRITE, buf, cnt))
		return -EINVAL;

	isapnp_cfg_begin(dev->card->number, dev->number);
	for (; pos < 256 && cnt > 0; pos++, buf++, cnt--) {
		unsigned char val;
		val = isapnp_read_byte(pos);
		__put_user(val, buf);
	}
	isapnp_cfg_end();

	*ppos = pos;
	return nbytes;
}

static const struct file_operations isapnp_proc_bus_file_operations = {
	.owner	= THIS_MODULE,
	.llseek = isapnp_proc_bus_lseek,
	.read = isapnp_proc_bus_read,
};

static int isapnp_proc_attach_device(struct pnp_dev *dev)
{
	struct pnp_card *bus = dev->card;
	struct proc_dir_entry *de, *e;
	char name[16];

	if (!(de = bus->procdir)) {
		sprintf(name, "%02x", bus->number);
		de = bus->procdir = proc_mkdir(name, isapnp_proc_bus_dir);
		if (!de)
			return -ENOMEM;
	}
	sprintf(name, "%02x", dev->number);
	e = dev->procent = proc_create_data(name, S_IFREG | S_IRUGO, de,
			&isapnp_proc_bus_file_operations, dev);
	if (!e)
		return -ENOMEM;
	e->size = 256;
	return 0;
}

int __init isapnp_proc_init(void)
{
	struct pnp_dev *dev;

	isapnp_proc_bus_dir = proc_mkdir("bus/isapnp", NULL);
	protocol_for_each_dev(&isapnp_protocol, dev) {
		isapnp_proc_attach_device(dev);
	}
	return 0;
}
