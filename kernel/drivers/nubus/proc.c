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

/* drivers/nubus/proc.c: Proc FS interface for NuBus.

   By David Huggins-Daines <dhd@debian.org>

   Much code and many ideas from drivers/pci/proc.c:
   Copyright (c) 1997, 1998 Martin Mares <mj@atrey.karlin.mff.cuni.cz>

   This is initially based on the Zorro and PCI interfaces.  However,
   it works somewhat differently.  The intent is to provide a
   structure in /proc analogous to the structure of the NuBus ROM
   resources.

   Therefore each NuBus device is in fact a directory, which may in
   turn contain subdirectories.  The "files" correspond to NuBus
   resource records.  For those types of records which we know how to
   convert to formats that are meaningful to userspace (mostly just
   icons) these files will provide "cooked" data.  Otherwise they will
   simply provide raw access (read-only of course) to the ROM.  */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/nubus.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/init.h>
#include <linux/module.h>

#include <asm/uaccess.h>
#include <asm/byteorder.h>

static int
nubus_devices_proc_show(struct seq_file *m, void *v)
{
	struct nubus_dev *dev = nubus_devices;

	while (dev) {
		seq_printf(m, "%x\t%04x %04x %04x %04x",
			      dev->board->slot,
			      dev->category,
			      dev->type,
			      dev->dr_sw,
			      dev->dr_hw);
		seq_printf(m, "\t%08lx\n", dev->board->slot_addr);
		dev = dev->next;
	}
	return 0;
}

static int nubus_devices_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, nubus_devices_proc_show, NULL);
}

static const struct file_operations nubus_devices_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= nubus_devices_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static struct proc_dir_entry *proc_bus_nubus_dir;

static void nubus_proc_subdir(struct nubus_dev* dev,
			      struct proc_dir_entry* parent,
			      struct nubus_dir* dir)
{
	struct nubus_dirent ent;

	/* Some of these are directories, others aren't */
	while (nubus_readdir(dir, &ent) != -1) {
		char name[8];
		struct proc_dir_entry* e;
		
		sprintf(name, "%x", ent.type);
		e = create_proc_entry(name, S_IFREG | S_IRUGO |
				      S_IWUSR, parent);
		if (!e) return;
	}
}

/* Can't do this recursively since the root directory is structured
   somewhat differently from the subdirectories */
static void nubus_proc_populate(struct nubus_dev* dev,
				struct proc_dir_entry* parent,
				struct nubus_dir* root)
{
	struct nubus_dirent ent;

	/* We know these are all directories (board resource + one or
	   more functional resources) */
	while (nubus_readdir(root, &ent) != -1) {
		char name[8];
		struct proc_dir_entry* e;
		struct nubus_dir dir;
		
		sprintf(name, "%x", ent.type);
		e = proc_mkdir(name, parent);
		if (!e) return;

		/* And descend */
		if (nubus_get_subdir(&ent, &dir) == -1) {
			/* This shouldn't happen */
			printk(KERN_ERR "NuBus root directory node %x:%x has no subdir!\n",
			       dev->board->slot, ent.type);
			continue;
		} else {
			nubus_proc_subdir(dev, e, &dir);
		}
	}
}

int nubus_proc_attach_device(struct nubus_dev *dev)
{
	struct proc_dir_entry *e;
	struct nubus_dir root;
	char name[8];

	if (dev == NULL) {
		printk(KERN_ERR
		       "NULL pointer in nubus_proc_attach_device, shoot the programmer!\n");
		return -1;
	}
		
	if (dev->board == NULL) {
		printk(KERN_ERR
		       "NULL pointer in nubus_proc_attach_device, shoot the programmer!\n");
		printk("dev = %p, dev->board = %p\n", dev, dev->board);
		return -1;
	}
		
	/* Create a directory */
	sprintf(name, "%x", dev->board->slot);
	e = dev->procdir = proc_mkdir(name, proc_bus_nubus_dir);
	if (!e)
		return -ENOMEM;

	/* Now recursively populate it with files */
	nubus_get_root_dir(dev->board, &root);
	nubus_proc_populate(dev, e, &root);

	return 0;
}
EXPORT_SYMBOL(nubus_proc_attach_device);

/* FIXME: this is certainly broken! */
int nubus_proc_detach_device(struct nubus_dev *dev)
{
	struct proc_dir_entry *e;

	if ((e = dev->procdir)) {
		if (atomic_read(&e->count))
			return -EBUSY;
		remove_proc_entry(e->name, proc_bus_nubus_dir);
		dev->procdir = NULL;
	}
	return 0;
}
EXPORT_SYMBOL(nubus_proc_detach_device);

void __init proc_bus_nubus_add_devices(void)
{
	struct nubus_dev *dev;
	
	for(dev = nubus_devices; dev; dev = dev->next)
		nubus_proc_attach_device(dev);
}

void __init nubus_proc_init(void)
{
	if (!MACH_IS_MAC)
		return;
	proc_bus_nubus_dir = proc_mkdir("bus/nubus", NULL);
	proc_create("devices", 0, proc_bus_nubus_dir, &nubus_devices_proc_fops);
	proc_bus_nubus_add_devices();
}
