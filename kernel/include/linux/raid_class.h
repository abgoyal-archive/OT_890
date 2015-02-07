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
 * raid_class.h - a generic raid visualisation class
 *
 * Copyright (c) 2005 - James Bottomley <James.Bottomley@steeleye.com>
 *
 * This file is licensed under GPLv2
 */
#include <linux/transport_class.h>

struct raid_template {
	struct transport_container raid_attrs;
};

struct raid_function_template {
	void *cookie;
	int (*is_raid)(struct device *);
	void (*get_resync)(struct device *);
	void (*get_state)(struct device *);
};

enum raid_state {
	RAID_STATE_UNKNOWN = 0,
	RAID_STATE_ACTIVE,
	RAID_STATE_DEGRADED,
	RAID_STATE_RESYNCING,
	RAID_STATE_OFFLINE,
};

enum raid_level {
	RAID_LEVEL_UNKNOWN = 0,
	RAID_LEVEL_LINEAR,
	RAID_LEVEL_0,
	RAID_LEVEL_1,
	RAID_LEVEL_10,
	RAID_LEVEL_3,
	RAID_LEVEL_4,
	RAID_LEVEL_5,
	RAID_LEVEL_50,
	RAID_LEVEL_6,
};

struct raid_data {
	struct list_head component_list;
	int component_count;
	enum raid_level level;
	enum raid_state state;
	int resync;
};

/* resync complete goes from 0 to this */
#define RAID_MAX_RESYNC		(10000)

#define DEFINE_RAID_ATTRIBUTE(type, attr)				      \
static inline void							      \
raid_set_##attr(struct raid_template *r, struct device *dev, type value) {    \
	struct device *device =						      \
		attribute_container_find_class_device(&r->raid_attrs.ac, dev);\
	struct raid_data *rd;						      \
	BUG_ON(!device);						      \
	rd = dev_get_drvdata(device);					      \
	rd->attr = value;						      \
}									      \
static inline type							      \
raid_get_##attr(struct raid_template *r, struct device *dev) {		      \
	struct device *device =						      \
		attribute_container_find_class_device(&r->raid_attrs.ac, dev);\
	struct raid_data *rd;						      \
	BUG_ON(!device);						      \
	rd = dev_get_drvdata(device);					      \
	return rd->attr;						      \
}

DEFINE_RAID_ATTRIBUTE(enum raid_level, level)
DEFINE_RAID_ATTRIBUTE(int, resync)
DEFINE_RAID_ATTRIBUTE(enum raid_state, state)
	
struct raid_template *raid_class_attach(struct raid_function_template *);
void raid_class_release(struct raid_template *);

int __must_check raid_component_add(struct raid_template *, struct device *,
				    struct device *);

