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

#ifndef _LINUX_FIRMWARE_H
#define _LINUX_FIRMWARE_H

#include <linux/module.h>
#include <linux/types.h>
#include <linux/compiler.h>

#define FIRMWARE_NAME_MAX 30 
#define FW_ACTION_NOHOTPLUG 0
#define FW_ACTION_HOTPLUG 1

struct firmware {
	size_t size;
	const u8 *data;
};

struct device;

struct builtin_fw {
	char *name;
	void *data;
	unsigned long size;
};

/* We have to play tricks here much like stringify() to get the
   __COUNTER__ macro to be expanded as we want it */
#define __fw_concat1(x, y) x##y
#define __fw_concat(x, y) __fw_concat1(x, y)

#define DECLARE_BUILTIN_FIRMWARE(name, blob)				     \
	DECLARE_BUILTIN_FIRMWARE_SIZE(name, &(blob), sizeof(blob))

#define DECLARE_BUILTIN_FIRMWARE_SIZE(name, blob, size)			     \
	static const struct builtin_fw __fw_concat(__builtin_fw,__COUNTER__) \
	__used __section(.builtin_fw) = { name, blob, size }

#if defined(CONFIG_FW_LOADER) || (defined(CONFIG_FW_LOADER_MODULE) && defined(MODULE))
int request_firmware(const struct firmware **fw, const char *name,
		     struct device *device);
int request_firmware_nowait(
	struct module *module, int uevent,
	const char *name, struct device *device, void *context,
	void (*cont)(const struct firmware *fw, void *context));

void release_firmware(const struct firmware *fw);
#else
static inline int request_firmware(const struct firmware **fw,
				   const char *name,
				   struct device *device)
{
	return -EINVAL;
}
static inline int request_firmware_nowait(
	struct module *module, int uevent,
	const char *name, struct device *device, void *context,
	void (*cont)(const struct firmware *fw, void *context))
{
	return -EINVAL;
}

static inline void release_firmware(const struct firmware *fw)
{
}
#endif

#endif
