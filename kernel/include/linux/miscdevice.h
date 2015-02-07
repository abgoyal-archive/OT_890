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

#ifndef _LINUX_MISCDEVICE_H
#define _LINUX_MISCDEVICE_H
#include <linux/module.h>
#include <linux/major.h>

#define PSMOUSE_MINOR		1
#define MS_BUSMOUSE_MINOR	2
#define ATIXL_BUSMOUSE_MINOR	3
/*#define AMIGAMOUSE_MINOR	4	FIXME OBSOLETE */
#define ATARIMOUSE_MINOR	5
#define SUN_MOUSE_MINOR		6
#define APOLLO_MOUSE_MINOR	7
#define PC110PAD_MINOR		9
/*#define ADB_MOUSE_MINOR	10	FIXME OBSOLETE */
#define WATCHDOG_MINOR		130	/* Watchdog timer     */
#define TEMP_MINOR		131	/* Temperature Sensor */
#define RTC_MINOR		135
#define EFI_RTC_MINOR		136	/* EFI Time services */
#define SUN_OPENPROM_MINOR	139
#define DMAPI_MINOR		140	/* DMAPI */
#define NVRAM_MINOR		144
#define SGI_MMTIMER		153
#define STORE_QUEUE_MINOR	155
#define I2O_MINOR		166
#define MICROCODE_MINOR		184
#define TUN_MINOR		200
#define MWAVE_MINOR		219	/* ACP/Mwave Modem */
#define MPT_MINOR		220
#define HPET_MINOR		228
#define FUSE_MINOR		229
#define KVM_MINOR		232
#define MISC_DYNAMIC_MINOR	255

struct device;

struct miscdevice  {
	int minor;
	const char *name;
	const struct file_operations *fops;
	struct list_head list;
	struct device *parent;
	struct device *this_device;
};

extern int misc_register(struct miscdevice * misc);
extern int misc_deregister(struct miscdevice *misc);

#define MODULE_ALIAS_MISCDEV(minor)				\
	MODULE_ALIAS("char-major-" __stringify(MISC_MAJOR)	\
	"-" __stringify(minor))
#endif
