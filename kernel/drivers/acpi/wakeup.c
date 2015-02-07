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
 * wakeup.c - support wakeup devices
 * Copyright (C) 2004 Li Shaohua <shaohua.li@intel.com>
 */

#include <linux/init.h>
#include <linux/acpi.h>
#include <acpi/acpi_drivers.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include "sleep.h"

#define _COMPONENT		ACPI_SYSTEM_COMPONENT
ACPI_MODULE_NAME("wakeup_devices")

extern struct list_head acpi_wakeup_device_list;
extern spinlock_t acpi_device_lock;

/**
 * acpi_enable_wakeup_device_prep - prepare wakeup devices
 *	@sleep_state:	ACPI state
 * Enable all wakup devices power if the devices' wakeup level
 * is higher than requested sleep level
 */

void acpi_enable_wakeup_device_prep(u8 sleep_state)
{
	struct list_head *node, *next;

	spin_lock(&acpi_device_lock);
	list_for_each_safe(node, next, &acpi_wakeup_device_list) {
		struct acpi_device *dev = container_of(node,
						       struct acpi_device,
						       wakeup_list);

		if (!dev->wakeup.flags.valid ||
		    !dev->wakeup.state.enabled ||
		    (sleep_state > (u32) dev->wakeup.sleep_state))
			continue;

		spin_unlock(&acpi_device_lock);
		acpi_enable_wakeup_device_power(dev, sleep_state);
		spin_lock(&acpi_device_lock);
	}
	spin_unlock(&acpi_device_lock);
}

/**
 * acpi_enable_wakeup_device - enable wakeup devices
 *	@sleep_state:	ACPI state
 * Enable all wakup devices's GPE
 */
void acpi_enable_wakeup_device(u8 sleep_state)
{
	struct list_head *node, *next;

	/* 
	 * Caution: this routine must be invoked when interrupt is disabled 
	 * Refer ACPI2.0: P212
	 */
	spin_lock(&acpi_device_lock);
	list_for_each_safe(node, next, &acpi_wakeup_device_list) {
		struct acpi_device *dev =
			container_of(node, struct acpi_device, wakeup_list);

		if (!dev->wakeup.flags.valid)
			continue;

		/* If users want to disable run-wake GPE,
		 * we only disable it for wake and leave it for runtime
		 */
		if ((!dev->wakeup.state.enabled && !dev->wakeup.flags.prepared)
		    || sleep_state > (u32) dev->wakeup.sleep_state) {
			if (dev->wakeup.flags.run_wake) {
				spin_unlock(&acpi_device_lock);
				/* set_gpe_type will disable GPE, leave it like that */
				acpi_set_gpe_type(dev->wakeup.gpe_device,
						  dev->wakeup.gpe_number,
						  ACPI_GPE_TYPE_RUNTIME);
				spin_lock(&acpi_device_lock);
			}
			continue;
		}
		spin_unlock(&acpi_device_lock);
		if (!dev->wakeup.flags.run_wake)
			acpi_enable_gpe(dev->wakeup.gpe_device,
					dev->wakeup.gpe_number);
		spin_lock(&acpi_device_lock);
	}
	spin_unlock(&acpi_device_lock);
}

/**
 * acpi_disable_wakeup_device - disable devices' wakeup capability
 *	@sleep_state:	ACPI state
 * Disable all wakup devices's GPE and wakeup capability
 */
void acpi_disable_wakeup_device(u8 sleep_state)
{
	struct list_head *node, *next;

	spin_lock(&acpi_device_lock);
	list_for_each_safe(node, next, &acpi_wakeup_device_list) {
		struct acpi_device *dev =
			container_of(node, struct acpi_device, wakeup_list);

		if (!dev->wakeup.flags.valid)
			continue;

		if ((!dev->wakeup.state.enabled && !dev->wakeup.flags.prepared)
		    || sleep_state > (u32) dev->wakeup.sleep_state) {
			if (dev->wakeup.flags.run_wake) {
				spin_unlock(&acpi_device_lock);
				acpi_set_gpe_type(dev->wakeup.gpe_device,
						  dev->wakeup.gpe_number,
						  ACPI_GPE_TYPE_WAKE_RUN);
				/* Re-enable it, since set_gpe_type will disable it */
				acpi_enable_gpe(dev->wakeup.gpe_device,
						dev->wakeup.gpe_number);
				spin_lock(&acpi_device_lock);
			}
			continue;
		}

		spin_unlock(&acpi_device_lock);
		acpi_disable_wakeup_device_power(dev);
		/* Never disable run-wake GPE */
		if (!dev->wakeup.flags.run_wake) {
			acpi_disable_gpe(dev->wakeup.gpe_device,
					 dev->wakeup.gpe_number);
			acpi_clear_gpe(dev->wakeup.gpe_device,
				       dev->wakeup.gpe_number, ACPI_NOT_ISR);
		}
		spin_lock(&acpi_device_lock);
	}
	spin_unlock(&acpi_device_lock);
}

static int __init acpi_wakeup_device_init(void)
{
	struct list_head *node, *next;

	if (acpi_disabled)
		return 0;

	spin_lock(&acpi_device_lock);
	list_for_each_safe(node, next, &acpi_wakeup_device_list) {
		struct acpi_device *dev = container_of(node,
						       struct acpi_device,
						       wakeup_list);
		/* In case user doesn't load button driver */
		if (!dev->wakeup.flags.run_wake || dev->wakeup.state.enabled)
			continue;
		spin_unlock(&acpi_device_lock);
		acpi_set_gpe_type(dev->wakeup.gpe_device,
				  dev->wakeup.gpe_number,
				  ACPI_GPE_TYPE_WAKE_RUN);
		acpi_enable_gpe(dev->wakeup.gpe_device,
				dev->wakeup.gpe_number);
		dev->wakeup.state.enabled = 1;
		spin_lock(&acpi_device_lock);
	}
	spin_unlock(&acpi_device_lock);
	return 0;
}

late_initcall(acpi_wakeup_device_init);
