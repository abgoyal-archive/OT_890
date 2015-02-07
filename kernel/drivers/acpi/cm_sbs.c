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
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/acpi.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <acpi/acpi_bus.h>
#include <acpi/acpi_drivers.h>

ACPI_MODULE_NAME("cm_sbs");
#define ACPI_AC_CLASS		"ac_adapter"
#define ACPI_BATTERY_CLASS	"battery"
#define _COMPONENT		ACPI_SBS_COMPONENT
static struct proc_dir_entry *acpi_ac_dir;
static struct proc_dir_entry *acpi_battery_dir;

static DEFINE_MUTEX(cm_sbs_mutex);

static int lock_ac_dir_cnt;
static int lock_battery_dir_cnt;

struct proc_dir_entry *acpi_lock_ac_dir(void)
{
	mutex_lock(&cm_sbs_mutex);
	if (!acpi_ac_dir)
		acpi_ac_dir = proc_mkdir(ACPI_AC_CLASS, acpi_root_dir);
	if (acpi_ac_dir) {
		lock_ac_dir_cnt++;
	} else {
		printk(KERN_ERR PREFIX
				  "Cannot create %s\n", ACPI_AC_CLASS);
	}
	mutex_unlock(&cm_sbs_mutex);
	return acpi_ac_dir;
}
EXPORT_SYMBOL(acpi_lock_ac_dir);

void acpi_unlock_ac_dir(struct proc_dir_entry *acpi_ac_dir_param)
{
	mutex_lock(&cm_sbs_mutex);
	if (acpi_ac_dir_param)
		lock_ac_dir_cnt--;
	if (lock_ac_dir_cnt == 0 && acpi_ac_dir_param && acpi_ac_dir) {
		remove_proc_entry(ACPI_AC_CLASS, acpi_root_dir);
		acpi_ac_dir = NULL;
	}
	mutex_unlock(&cm_sbs_mutex);
}
EXPORT_SYMBOL(acpi_unlock_ac_dir);

struct proc_dir_entry *acpi_lock_battery_dir(void)
{
	mutex_lock(&cm_sbs_mutex);
	if (!acpi_battery_dir) {
		acpi_battery_dir =
		    proc_mkdir(ACPI_BATTERY_CLASS, acpi_root_dir);
	}
	if (acpi_battery_dir) {
		lock_battery_dir_cnt++;
	} else {
		printk(KERN_ERR PREFIX
				  "Cannot create %s\n", ACPI_BATTERY_CLASS);
	}
	mutex_unlock(&cm_sbs_mutex);
	return acpi_battery_dir;
}
EXPORT_SYMBOL(acpi_lock_battery_dir);

void acpi_unlock_battery_dir(struct proc_dir_entry *acpi_battery_dir_param)
{
	mutex_lock(&cm_sbs_mutex);
	if (acpi_battery_dir_param)
		lock_battery_dir_cnt--;
	if (lock_battery_dir_cnt == 0 && acpi_battery_dir_param
	    && acpi_battery_dir) {
		remove_proc_entry(ACPI_BATTERY_CLASS, acpi_root_dir);
		acpi_battery_dir = NULL;
	}
	mutex_unlock(&cm_sbs_mutex);
	return;
}
EXPORT_SYMBOL(acpi_unlock_battery_dir);
