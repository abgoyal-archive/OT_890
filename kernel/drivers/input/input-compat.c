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
 * 32bit compatibility wrappers for the input subsystem.
 *
 * Very heavily based on evdev.c - Copyright (c) 1999-2002 Vojtech Pavlik
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

#include <asm/uaccess.h>
#include "input-compat.h"

#ifdef CONFIG_COMPAT

int input_event_from_user(const char __user *buffer,
			  struct input_event *event)
{
	if (INPUT_COMPAT_TEST) {
		struct input_event_compat compat_event;

		if (copy_from_user(&compat_event, buffer,
				   sizeof(struct input_event_compat)))
			return -EFAULT;

		event->time.tv_sec = compat_event.time.tv_sec;
		event->time.tv_usec = compat_event.time.tv_usec;
		event->type = compat_event.type;
		event->code = compat_event.code;
		event->value = compat_event.value;

	} else {
		if (copy_from_user(event, buffer, sizeof(struct input_event)))
			return -EFAULT;
	}

	return 0;
}

int input_event_to_user(char __user *buffer,
			const struct input_event *event)
{
	if (INPUT_COMPAT_TEST) {
		struct input_event_compat compat_event;

		compat_event.time.tv_sec = event->time.tv_sec;
		compat_event.time.tv_usec = event->time.tv_usec;
		compat_event.type = event->type;
		compat_event.code = event->code;
		compat_event.value = event->value;

		if (copy_to_user(buffer, &compat_event,
				 sizeof(struct input_event_compat)))
			return -EFAULT;

	} else {
		if (copy_to_user(buffer, event, sizeof(struct input_event)))
			return -EFAULT;
	}

	return 0;
}

int input_ff_effect_from_user(const char __user *buffer, size_t size,
			      struct ff_effect *effect)
{
	if (INPUT_COMPAT_TEST) {
		struct ff_effect_compat *compat_effect;

		if (size != sizeof(struct ff_effect_compat))
			return -EINVAL;

		/*
		 * It so happens that the pointer which needs to be changed
		 * is the last field in the structure, so we can retrieve the
		 * whole thing and replace just the pointer.
		 */
		compat_effect = (struct ff_effect_compat *)effect;

		if (copy_from_user(compat_effect, buffer,
				   sizeof(struct ff_effect_compat)))
			return -EFAULT;

		if (compat_effect->type == FF_PERIODIC &&
		    compat_effect->u.periodic.waveform == FF_CUSTOM)
			effect->u.periodic.custom_data =
				compat_ptr(compat_effect->u.periodic.custom_data);
	} else {
		if (size != sizeof(struct ff_effect))
			return -EINVAL;

		if (copy_from_user(effect, buffer, sizeof(struct ff_effect)))
			return -EFAULT;
	}

	return 0;
}

#else

int input_event_from_user(const char __user *buffer,
			 struct input_event *event)
{
	if (copy_from_user(event, buffer, sizeof(struct input_event)))
		return -EFAULT;

	return 0;
}

int input_event_to_user(char __user *buffer,
			const struct input_event *event)
{
	if (copy_to_user(buffer, event, sizeof(struct input_event)))
		return -EFAULT;

	return 0;
}

int input_ff_effect_from_user(const char __user *buffer, size_t size,
			      struct ff_effect *effect)
{
	if (size != sizeof(struct ff_effect))
		return -EINVAL;

	if (copy_from_user(effect, buffer, sizeof(struct ff_effect)))
		return -EFAULT;

	return 0;
}

#endif /* CONFIG_COMPAT */

EXPORT_SYMBOL_GPL(input_event_from_user);
EXPORT_SYMBOL_GPL(input_event_to_user);
EXPORT_SYMBOL_GPL(input_ff_effect_from_user);
