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
 * driver.c - driver support
 *
 * (C) 2006-2007 Venkatesh Pallipadi <venkatesh.pallipadi@intel.com>
 *               Shaohua Li <shaohua.li@intel.com>
 *               Adam Belay <abelay@novell.com>
 *
 * This code is licenced under the GPL.
 */

#include <linux/mutex.h>
#include <linux/module.h>
#include <linux/cpuidle.h>

#include "cpuidle.h"

struct cpuidle_driver *cpuidle_curr_driver;
DEFINE_SPINLOCK(cpuidle_driver_lock);

/**
 * cpuidle_register_driver - registers a driver
 * @drv: the driver
 */
int cpuidle_register_driver(struct cpuidle_driver *drv)
{
	if (!drv)
		return -EINVAL;

	spin_lock(&cpuidle_driver_lock);
	if (cpuidle_curr_driver) {
		spin_unlock(&cpuidle_driver_lock);
		return -EBUSY;
	}
	cpuidle_curr_driver = drv;
	spin_unlock(&cpuidle_driver_lock);

	return 0;
}

EXPORT_SYMBOL_GPL(cpuidle_register_driver);

/**
 * cpuidle_unregister_driver - unregisters a driver
 * @drv: the driver
 */
void cpuidle_unregister_driver(struct cpuidle_driver *drv)
{
	if (!drv)
		return;

	spin_lock(&cpuidle_driver_lock);
	cpuidle_curr_driver = NULL;
	spin_unlock(&cpuidle_driver_lock);
}

EXPORT_SYMBOL_GPL(cpuidle_unregister_driver);
