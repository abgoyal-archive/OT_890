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
 * transport_class.h - a generic container for all transport classes
 *
 * Copyright (c) 2005 - James Bottomley <James.Bottomley@steeleye.com>
 *
 * This file is licensed under GPLv2
 */

#ifndef _TRANSPORT_CLASS_H_
#define _TRANSPORT_CLASS_H_

#include <linux/device.h>
#include <linux/attribute_container.h>

struct transport_container;

struct transport_class {
	struct class class;
	int (*setup)(struct transport_container *, struct device *,
		     struct device *);
	int (*configure)(struct transport_container *, struct device *,
			 struct device *);
	int (*remove)(struct transport_container *, struct device *,
		      struct device *);
};

#define DECLARE_TRANSPORT_CLASS(cls, nm, su, rm, cfg)			\
struct transport_class cls = {						\
	.class = {							\
		.name = nm,						\
	},								\
	.setup = su,							\
	.remove = rm,							\
	.configure = cfg,						\
}


struct anon_transport_class {
	struct transport_class tclass;
	struct attribute_container container;
};

#define DECLARE_ANON_TRANSPORT_CLASS(cls, mtch, cfg)		\
struct anon_transport_class cls = {				\
	.tclass = {						\
		.configure = cfg,				\
	},							\
	. container = {						\
		.match = mtch,					\
	},							\
}

#define class_to_transport_class(x) \
	container_of(x, struct transport_class, class)

struct transport_container {
	struct attribute_container ac;
	struct attribute_group *statistics;
};

#define attribute_container_to_transport_container(x) \
	container_of(x, struct transport_container, ac)

void transport_remove_device(struct device *);
void transport_add_device(struct device *);
void transport_setup_device(struct device *);
void transport_configure_device(struct device *);
void transport_destroy_device(struct device *);

static inline void
transport_register_device(struct device *dev)
{
	transport_setup_device(dev);
	transport_add_device(dev);
}

static inline void
transport_unregister_device(struct device *dev)
{
	transport_remove_device(dev);
	transport_destroy_device(dev);
}

static inline int transport_container_register(struct transport_container *tc)
{
	return attribute_container_register(&tc->ac);
}

static inline void transport_container_unregister(struct transport_container *tc)
{
	if (unlikely(attribute_container_unregister(&tc->ac)))
		BUG();
}

int transport_class_register(struct transport_class *);
int anon_transport_class_register(struct anon_transport_class *);
void transport_class_unregister(struct transport_class *);
void anon_transport_class_unregister(struct anon_transport_class *);


#endif
