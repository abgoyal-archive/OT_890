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
 * UWB PAL support.
 *
 * Copyright (C) 2008 Cambridge Silicon Radio Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/uwb.h>

#include "uwb-internal.h"

/**
 * uwb_pal_init - initialize a UWB PAL
 * @pal: the PAL to initialize
 */
void uwb_pal_init(struct uwb_pal *pal)
{
	INIT_LIST_HEAD(&pal->node);
}
EXPORT_SYMBOL_GPL(uwb_pal_init);

/**
 * uwb_pal_register - register a UWB PAL
 * @pal: the PAL
 *
 * The PAL must be initialized with uwb_pal_init().
 */
int uwb_pal_register(struct uwb_pal *pal)
{
	struct uwb_rc *rc = pal->rc;
	int ret;

	if (pal->device) {
		ret = sysfs_create_link(&pal->device->kobj,
					&rc->uwb_dev.dev.kobj, "uwb_rc");
		if (ret < 0)
			return ret;
		ret = sysfs_create_link(&rc->uwb_dev.dev.kobj,
					&pal->device->kobj, pal->name);
		if (ret < 0) {
			sysfs_remove_link(&pal->device->kobj, "uwb_rc");
			return ret;
		}
	}

	pal->debugfs_dir = uwb_dbg_create_pal_dir(pal);

	mutex_lock(&rc->uwb_dev.mutex);
	list_add(&pal->node, &rc->pals);
	mutex_unlock(&rc->uwb_dev.mutex);

	return 0;
}
EXPORT_SYMBOL_GPL(uwb_pal_register);

/**
 * uwb_pal_register - unregister a UWB PAL
 * @pal: the PAL
 */
void uwb_pal_unregister(struct uwb_pal *pal)
{
	struct uwb_rc *rc = pal->rc;

	uwb_radio_stop(pal);

	mutex_lock(&rc->uwb_dev.mutex);
	list_del(&pal->node);
	mutex_unlock(&rc->uwb_dev.mutex);

	debugfs_remove(pal->debugfs_dir);

	if (pal->device) {
		sysfs_remove_link(&rc->uwb_dev.dev.kobj, pal->name);
		sysfs_remove_link(&pal->device->kobj, "uwb_rc");
	}
}
EXPORT_SYMBOL_GPL(uwb_pal_unregister);

/**
 * uwb_rc_pal_init - initialize the PAL related parts of a radio controller
 * @rc: the radio controller
 */
void uwb_rc_pal_init(struct uwb_rc *rc)
{
	INIT_LIST_HEAD(&rc->pals);
}
