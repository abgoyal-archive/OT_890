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

#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/module.h>

#include "debugfs.h"

static struct dentry *d_xen_debug;

struct dentry * __init xen_init_debugfs(void)
{
	if (!d_xen_debug) {
		d_xen_debug = debugfs_create_dir("xen", NULL);

		if (!d_xen_debug)
			pr_warning("Could not create 'xen' debugfs directory\n");
	}

	return d_xen_debug;
}

struct array_data
{
	void *array;
	unsigned elements;
};

static int u32_array_open(struct inode *inode, struct file *file)
{
	file->private_data = NULL;
	return nonseekable_open(inode, file);
}

static size_t format_array(char *buf, size_t bufsize, const char *fmt,
			   u32 *array, unsigned array_size)
{
	size_t ret = 0;
	unsigned i;

	for(i = 0; i < array_size; i++) {
		size_t len;

		len = snprintf(buf, bufsize, fmt, array[i]);
		len++;	/* ' ' or '\n' */
		ret += len;

		if (buf) {
			buf += len;
			bufsize -= len;
			buf[-1] = (i == array_size-1) ? '\n' : ' ';
		}
	}

	ret++;		/* \0 */
	if (buf)
		*buf = '\0';

	return ret;
}

static char *format_array_alloc(const char *fmt, u32 *array, unsigned array_size)
{
	size_t len = format_array(NULL, 0, fmt, array, array_size);
	char *ret;

	ret = kmalloc(len, GFP_KERNEL);
	if (ret == NULL)
		return NULL;

	format_array(ret, len, fmt, array, array_size);
	return ret;
}

static ssize_t u32_array_read(struct file *file, char __user *buf, size_t len,
			      loff_t *ppos)
{
	struct inode *inode = file->f_path.dentry->d_inode;
	struct array_data *data = inode->i_private;
	size_t size;

	if (*ppos == 0) {
		if (file->private_data) {
			kfree(file->private_data);
			file->private_data = NULL;
		}

		file->private_data = format_array_alloc("%u", data->array, data->elements);
	}

	size = 0;
	if (file->private_data)
		size = strlen(file->private_data);

	return simple_read_from_buffer(buf, len, ppos, file->private_data, size);
}

static int xen_array_release(struct inode *inode, struct file *file)
{
	kfree(file->private_data);

	return 0;
}

static struct file_operations u32_array_fops = {
	.owner	= THIS_MODULE,
	.open	= u32_array_open,
	.release= xen_array_release,
	.read	= u32_array_read,
};

struct dentry *xen_debugfs_create_u32_array(const char *name, mode_t mode,
					    struct dentry *parent,
					    u32 *array, unsigned elements)
{
	struct array_data *data = kmalloc(sizeof(*data), GFP_KERNEL);

	if (data == NULL)
		return NULL;

	data->array = array;
	data->elements = elements;

	return debugfs_create_file(name, mode, parent, data, &u32_array_fops);
}
