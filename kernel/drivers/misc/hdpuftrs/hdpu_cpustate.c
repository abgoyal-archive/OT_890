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
 *	Sky CPU State Driver
 *
 *	Copyright (C) 2002 Brian Waite
 *
 *	This driver allows use of the CPU state bits
 *	It exports the /dev/sky_cpustate and also
 *	/proc/sky_cpustate pseudo-file for status information.
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/smp_lock.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/hdpu_features.h>
#include <linux/platform_device.h>
#include <asm/uaccess.h>
#include <linux/seq_file.h>
#include <asm/io.h>

#define SKY_CPUSTATE_VERSION		"1.1"

static int hdpu_cpustate_probe(struct platform_device *pdev);
static int hdpu_cpustate_remove(struct platform_device *pdev);

static unsigned char cpustate_get_state(void);
static int cpustate_proc_open(struct inode *inode, struct file *file);
static int cpustate_proc_read(struct seq_file *seq, void *offset);

static struct cpustate_t cpustate;

static const struct file_operations proc_cpustate = {
	.open = cpustate_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
	.owner = THIS_MODULE,
};

static int cpustate_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, cpustate_proc_read, NULL);
}

static int cpustate_proc_read(struct seq_file *seq, void *offset)
{
	seq_printf(seq, "CPU State: %04x\n", cpustate_get_state());
	return 0;
}

static int cpustate_get_ref(int excl)
{

	int retval = -EBUSY;

	spin_lock(&cpustate.lock);

	if (cpustate.excl)
		goto out_busy;

	if (excl) {
		if (cpustate.open_count)
			goto out_busy;
		cpustate.excl = 1;
	}

	cpustate.open_count++;
	retval = 0;

      out_busy:
	spin_unlock(&cpustate.lock);
	return retval;
}

static int cpustate_free_ref(void)
{

	spin_lock(&cpustate.lock);

	cpustate.excl = 0;
	cpustate.open_count--;

	spin_unlock(&cpustate.lock);
	return 0;
}

static unsigned char cpustate_get_state(void)
{

	return cpustate.cached_val;
}

static void cpustate_set_state(unsigned char new_state)
{
	unsigned int state = (new_state << 21);

#ifdef DEBUG_CPUSTATE
	printk("CPUSTATE -> 0x%x\n", new_state);
#endif
	spin_lock(&cpustate.lock);
	cpustate.cached_val = new_state;
	writel((0xff << 21), cpustate.clr_addr);
	writel(state, cpustate.set_addr);
	spin_unlock(&cpustate.lock);
}

/*
 *	Now all the various file operations that we export.
 */

static ssize_t cpustate_read(struct file *file, char *buf,
			     size_t count, loff_t * ppos)
{
	unsigned char data;

	if (count < 0)
		return -EFAULT;
	if (count == 0)
		return 0;

	data = cpustate_get_state();
	if (copy_to_user(buf, &data, sizeof(unsigned char)))
		return -EFAULT;
	return sizeof(unsigned char);
}

static ssize_t cpustate_write(struct file *file, const char *buf,
			      size_t count, loff_t * ppos)
{
	unsigned char data;

	if (count < 0)
		return -EFAULT;

	if (count == 0)
		return 0;

	if (copy_from_user((unsigned char *)&data, buf, sizeof(unsigned char)))
		return -EFAULT;

	cpustate_set_state(data);
	return sizeof(unsigned char);
}

static int cpustate_open(struct inode *inode, struct file *file)
{
	int ret;

	lock_kernel();
	ret = cpustate_get_ref((file->f_flags & O_EXCL));
	unlock_kernel();

	return ret;
}

static int cpustate_release(struct inode *inode, struct file *file)
{
	return cpustate_free_ref();
}

static struct platform_driver hdpu_cpustate_driver = {
	.probe = hdpu_cpustate_probe,
	.remove = hdpu_cpustate_remove,
	.driver = {
		.name = HDPU_CPUSTATE_NAME,
		.owner = THIS_MODULE,
	},
};

/*
 *	The various file operations we support.
 */
static const struct file_operations cpustate_fops = {
      .owner	= THIS_MODULE,
      .open	= cpustate_open,
      .release	= cpustate_release,
      .read	= cpustate_read,
      .write	= cpustate_write,
      .llseek	= no_llseek,
};

static struct miscdevice cpustate_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "sky_cpustate",
	.fops	= &cpustate_fops,
};

static int hdpu_cpustate_probe(struct platform_device *pdev)
{
	struct resource *res;
	struct proc_dir_entry *proc_de;
	int ret;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		printk(KERN_ERR "sky_cpustate: "
		       "Invalid memory resource.\n");
		return -EINVAL;
	}
	cpustate.set_addr = (unsigned long *)res->start;
	cpustate.clr_addr = (unsigned long *)res->end - 1;

	ret = misc_register(&cpustate_dev);
	if (ret) {
		printk(KERN_WARNING "sky_cpustate: "
		       "Unable to register misc device.\n");
		cpustate.set_addr = NULL;
		cpustate.clr_addr = NULL;
		return ret;
	}

	proc_de = proc_create("sky_cpustate", 0666, NULL, &proc_cpustate);
	if (!proc_de) {
		printk(KERN_WARNING "sky_cpustate: "
		       "Unable to create proc entry\n");
	}

	printk(KERN_INFO "Sky CPU State Driver v" SKY_CPUSTATE_VERSION "\n");
	return 0;
}

static int hdpu_cpustate_remove(struct platform_device *pdev)
{
	cpustate.set_addr = NULL;
	cpustate.clr_addr = NULL;

	remove_proc_entry("sky_cpustate", NULL);
	misc_deregister(&cpustate_dev);

	return 0;
}

static int __init cpustate_init(void)
{
	return platform_driver_register(&hdpu_cpustate_driver);
}

static void __exit cpustate_exit(void)
{
	platform_driver_unregister(&hdpu_cpustate_driver);
}

module_init(cpustate_init);
module_exit(cpustate_exit);

MODULE_AUTHOR("Brian Waite");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" HDPU_CPUSTATE_NAME);
