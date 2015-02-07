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
 * attribute_container.h - a generic container for all classes
 *
 * Copyright (c) 2005 - James Bottomley <James.Bottomley@steeleye.com>
 *
 * This file is licensed under GPLv2
 */

#ifndef _ATTRIBUTE_CONTAINER_H_
#define _ATTRIBUTE_CONTAINER_H_

#include <linux/device.h>
#include <linux/list.h>
#include <linux/klist.h>

struct attribute_container {
	struct list_head	node;
	struct klist		containers;
	struct class		*class;
	struct attribute_group	*grp;
	struct device_attribute **attrs;
	int (*match)(struct attribute_container *, struct device *);
#define	ATTRIBUTE_CONTAINER_NO_CLASSDEVS	0x01
	unsigned long		flags;
};

static inline int
attribute_container_no_classdevs(struct attribute_container *atc)
{
	return atc->flags & ATTRIBUTE_CONTAINER_NO_CLASSDEVS;
}

static inline void
attribute_container_set_no_classdevs(struct attribute_container *atc)
{
	atc->flags |= ATTRIBUTE_CONTAINER_NO_CLASSDEVS;
}

int attribute_container_register(struct attribute_container *cont);
int __must_check attribute_container_unregister(struct attribute_container *cont);
void attribute_container_create_device(struct device *dev,
				       int (*fn)(struct attribute_container *,
						 struct device *,
						 struct device *));
void attribute_container_add_device(struct device *dev,
				    int (*fn)(struct attribute_container *,
					      struct device *,
					      struct device *));
void attribute_container_remove_device(struct device *dev,
				       void (*fn)(struct attribute_container *,
						  struct device *,
						  struct device *));
void attribute_container_device_trigger(struct device *dev, 
					int (*fn)(struct attribute_container *,
						  struct device *,
						  struct device *));
void attribute_container_trigger(struct device *dev, 
				 int (*fn)(struct attribute_container *,
					   struct device *));
int attribute_container_add_attrs(struct device *classdev);
int attribute_container_add_class_device(struct device *classdev);
int attribute_container_add_class_device_adapter(struct attribute_container *cont,
						 struct device *dev,
						 struct device *classdev);
void attribute_container_remove_attrs(struct device *classdev);
void attribute_container_class_device_del(struct device *classdev);
struct attribute_container *attribute_container_classdev_to_container(struct device *);
struct device *attribute_container_find_class_device(struct attribute_container *, struct device *);
struct device_attribute **attribute_container_classdev_to_attrs(const struct device *classdev);

#endif
