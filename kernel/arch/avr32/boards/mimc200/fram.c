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
 * FRAM driver for MIMC200 board
 *
 * Copyright 2008 Mark Jackson <mpfj@mimc.co.uk>
 *
 * This module adds *very* simply support for the system's FRAM device.
 * At the moment, this is hard-coded to the MIMC200 platform, and only
 * supports mmap().
 */

#define FRAM_VERSION	"1.0"

#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/io.h>

#define FRAM_BASE	0xac000000
#define FRAM_SIZE	0x20000

/*
 * The are the file operation function for user access to /dev/fram
 */

static int fram_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret;

	ret = remap_pfn_range(vma,
		vma->vm_start,
		virt_to_phys((void *)((unsigned long)FRAM_BASE)) >> PAGE_SHIFT,
		vma->vm_end-vma->vm_start,
		PAGE_SHARED);

	if (ret != 0)
		return -EAGAIN;

	return 0;
}

static const struct file_operations fram_fops = {
	.owner			= THIS_MODULE,
	.mmap			= fram_mmap,
};

#define FRAM_MINOR	0

static struct miscdevice fram_dev = {
	FRAM_MINOR,
	"fram",
	&fram_fops
};

static int __init
fram_init(void)
{
	int ret;

	ret = misc_register(&fram_dev);
	if (ret) {
		printk(KERN_ERR "fram: can't misc_register on minor=%d\n",
		    FRAM_MINOR);
		return ret;
	}
	printk(KERN_INFO "FRAM memory driver v" FRAM_VERSION "\n");
	return 0;
}

static void __exit
fram_cleanup_module(void)
{
	misc_deregister(&fram_dev);
}

module_init(fram_init);
module_exit(fram_cleanup_module);

MODULE_LICENSE("GPL");

MODULE_ALIAS_MISCDEV(FRAM_MINOR);
