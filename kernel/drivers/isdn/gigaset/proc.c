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
 * Stuff used by all variants of the driver
 *
 * Copyright (c) 2001 by Stefan Eilers,
 *                       Hansjoerg Lipp <hjlipp@web.de>,
 *                       Tilman Schmidt <tilman@imap.cc>.
 *
 * =====================================================================
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation; either version 2 of
 *	the License, or (at your option) any later version.
 * =====================================================================
 */

#include "gigaset.h"
#include <linux/ctype.h>

static ssize_t show_cidmode(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	struct cardstate *cs = dev_get_drvdata(dev);

	return sprintf(buf, "%u\n", cs->cidmode);
}

static ssize_t set_cidmode(struct device *dev, struct device_attribute *attr,
			   const char *buf, size_t count)
{
	struct cardstate *cs = dev_get_drvdata(dev);
	long int value;
	char *end;

	value = simple_strtol(buf, &end, 0);
	while (*end)
		if (!isspace(*end++))
			return -EINVAL;
	if (value < 0 || value > 1)
			return -EINVAL;

	if (mutex_lock_interruptible(&cs->mutex))
		return -ERESTARTSYS; // FIXME -EINTR?

	cs->waiting = 1;
	if (!gigaset_add_event(cs, &cs->at_state, EV_PROC_CIDMODE,
			       NULL, value, NULL)) {
		cs->waiting = 0;
		mutex_unlock(&cs->mutex);
		return -ENOMEM;
	}

	gig_dbg(DEBUG_CMD, "scheduling PROC_CIDMODE");
	gigaset_schedule_event(cs);

	wait_event(cs->waitqueue, !cs->waiting);

	mutex_unlock(&cs->mutex);

	return count;
}

static DEVICE_ATTR(cidmode, S_IRUGO|S_IWUSR, show_cidmode, set_cidmode);

/* free sysfs for device */
void gigaset_free_dev_sysfs(struct cardstate *cs)
{
	if (!cs->tty_dev)
		return;

	gig_dbg(DEBUG_INIT, "removing sysfs entries");
	device_remove_file(cs->tty_dev, &dev_attr_cidmode);
}

/* initialize sysfs for device */
void gigaset_init_dev_sysfs(struct cardstate *cs)
{
	if (!cs->tty_dev)
		return;

	gig_dbg(DEBUG_INIT, "setting up sysfs");
	if (device_create_file(cs->tty_dev, &dev_attr_cidmode))
		dev_err(cs->dev, "could not create sysfs attribute\n");
}
