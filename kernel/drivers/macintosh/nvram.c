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
 * /dev/nvram driver for Power Macintosh.
 */

#define NVRAM_VERSION "1.0"

#include <linux/module.h>

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/nvram.h>
#include <linux/init.h>
#include <linux/smp_lock.h>
#include <asm/uaccess.h>
#include <asm/nvram.h>

#define NVRAM_SIZE	8192

static loff_t nvram_llseek(struct file *file, loff_t offset, int origin)
{
	lock_kernel();
	switch (origin) {
	case 1:
		offset += file->f_pos;
		break;
	case 2:
		offset += NVRAM_SIZE;
		break;
	}
	if (offset < 0) {
		unlock_kernel();
		return -EINVAL;
	}
	file->f_pos = offset;
	unlock_kernel();
	return file->f_pos;
}

static ssize_t read_nvram(struct file *file, char __user *buf,
			  size_t count, loff_t *ppos)
{
	unsigned int i;
	char __user *p = buf;

	if (!access_ok(VERIFY_WRITE, buf, count))
		return -EFAULT;
	if (*ppos >= NVRAM_SIZE)
		return 0;
	for (i = *ppos; count > 0 && i < NVRAM_SIZE; ++i, ++p, --count)
		if (__put_user(nvram_read_byte(i), p))
			return -EFAULT;
	*ppos = i;
	return p - buf;
}

static ssize_t write_nvram(struct file *file, const char __user *buf,
			   size_t count, loff_t *ppos)
{
	unsigned int i;
	const char __user *p = buf;
	char c;

	if (!access_ok(VERIFY_READ, buf, count))
		return -EFAULT;
	if (*ppos >= NVRAM_SIZE)
		return 0;
	for (i = *ppos; count > 0 && i < NVRAM_SIZE; ++i, ++p, --count) {
		if (__get_user(c, p))
			return -EFAULT;
		nvram_write_byte(c, i);
	}
	*ppos = i;
	return p - buf;
}

static int nvram_ioctl(struct inode *inode, struct file *file,
	unsigned int cmd, unsigned long arg)
{
	switch(cmd) {
		case PMAC_NVRAM_GET_OFFSET:
		{
			int part, offset;
			if (copy_from_user(&part, (void __user*)arg, sizeof(part)) != 0)
				return -EFAULT;
			if (part < pmac_nvram_OF || part > pmac_nvram_NR)
				return -EINVAL;
			offset = pmac_get_partition(part);
			if (copy_to_user((void __user*)arg, &offset, sizeof(offset)) != 0)
				return -EFAULT;
			break;
		}

		default:
			return -EINVAL;
	}

	return 0;
}

const struct file_operations nvram_fops = {
	.owner		= THIS_MODULE,
	.llseek		= nvram_llseek,
	.read		= read_nvram,
	.write		= write_nvram,
	.ioctl		= nvram_ioctl,
};

static struct miscdevice nvram_dev = {
	NVRAM_MINOR,
	"nvram",
	&nvram_fops
};

int __init nvram_init(void)
{
	printk(KERN_INFO "Macintosh non-volatile memory driver v%s\n",
		NVRAM_VERSION);
	return misc_register(&nvram_dev);
}

void __exit nvram_cleanup(void)
{
        misc_deregister( &nvram_dev );
}

module_init(nvram_init);
module_exit(nvram_cleanup);
MODULE_LICENSE("GPL");
