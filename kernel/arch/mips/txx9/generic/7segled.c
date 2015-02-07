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
 * 7 Segment LED routines
 * Based on RBTX49xx patch from CELF patch archive.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * (C) Copyright TOSHIBA CORPORATION 2005-2007
 * All Rights Reserved.
 */
#include <linux/sysdev.h>
#include <linux/slab.h>
#include <linux/map_to_7segment.h>
#include <asm/txx9/generic.h>

static unsigned int tx_7segled_num;
static void (*tx_7segled_putc)(unsigned int pos, unsigned char val);

void __init txx9_7segled_init(unsigned int num,
			      void (*putc)(unsigned int pos, unsigned char val))
{
	tx_7segled_num = num;
	tx_7segled_putc = putc;
}

static SEG7_CONVERSION_MAP(txx9_seg7map, MAP_ASCII7SEG_ALPHANUM_LC);

int txx9_7segled_putc(unsigned int pos, char c)
{
	if (pos >= tx_7segled_num)
		return -EINVAL;
	c = map_to_seg7(&txx9_seg7map, c);
	if (c < 0)
		return c;
	tx_7segled_putc(pos, c);
	return 0;
}

static ssize_t ascii_store(struct sys_device *dev,
			   struct sysdev_attribute *attr,
			   const char *buf, size_t size)
{
	unsigned int ch = dev->id;
	txx9_7segled_putc(ch, buf[0]);
	return size;
}

static ssize_t raw_store(struct sys_device *dev,
			 struct sysdev_attribute *attr,
			 const char *buf, size_t size)
{
	unsigned int ch = dev->id;
	tx_7segled_putc(ch, buf[0]);
	return size;
}

static SYSDEV_ATTR(ascii, 0200, NULL, ascii_store);
static SYSDEV_ATTR(raw, 0200, NULL, raw_store);

static ssize_t map_seg7_show(struct sysdev_class *class, char *buf)
{
	memcpy(buf, &txx9_seg7map, sizeof(txx9_seg7map));
	return sizeof(txx9_seg7map);
}

static ssize_t map_seg7_store(struct sysdev_class *class,
			      const char *buf, size_t size)
{
	if (size != sizeof(txx9_seg7map))
		return -EINVAL;
	memcpy(&txx9_seg7map, buf, size);
	return size;
}

static SYSDEV_CLASS_ATTR(map_seg7, 0600, map_seg7_show, map_seg7_store);

static struct sysdev_class tx_7segled_sysdev_class = {
	.name	= "7segled",
};

static int __init tx_7segled_init_sysfs(void)
{
	int error, i;
	if (!tx_7segled_num)
		return -ENODEV;
	error = sysdev_class_register(&tx_7segled_sysdev_class);
	if (error)
		return error;
	error = sysdev_class_create_file(&tx_7segled_sysdev_class,
					 &attr_map_seg7);
	if (error)
		return error;
	for (i = 0; i < tx_7segled_num; i++) {
		struct sys_device *dev;
		dev = kzalloc(sizeof(*dev), GFP_KERNEL);
		if (!dev) {
			error = -ENODEV;
			break;
		}
		dev->id = i;
		dev->cls = &tx_7segled_sysdev_class;
		error = sysdev_register(dev);
		if (!error) {
			sysdev_create_file(dev, &attr_ascii);
			sysdev_create_file(dev, &attr_raw);
		}
	}
	return error;
}

device_initcall(tx_7segled_init_sysfs);
