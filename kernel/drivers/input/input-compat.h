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

#ifndef _INPUT_COMPAT_H
#define _INPUT_COMPAT_H

/*
 * 32bit compatibility wrappers for the input subsystem.
 *
 * Very heavily based on evdev.c - Copyright (c) 1999-2002 Vojtech Pavlik
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

#include <linux/compiler.h>
#include <linux/compat.h>
#include <linux/input.h>

#ifdef CONFIG_COMPAT

/* Note to the author of this code: did it ever occur to
   you why the ifdefs are needed? Think about it again. -AK */
#ifdef CONFIG_X86_64
#  define INPUT_COMPAT_TEST is_compat_task()
#elif defined(CONFIG_IA64)
#  define INPUT_COMPAT_TEST IS_IA32_PROCESS(task_pt_regs(current))
#elif defined(CONFIG_S390)
#  define INPUT_COMPAT_TEST test_thread_flag(TIF_31BIT)
#elif defined(CONFIG_MIPS)
#  define INPUT_COMPAT_TEST test_thread_flag(TIF_32BIT_ADDR)
#else
#  define INPUT_COMPAT_TEST test_thread_flag(TIF_32BIT)
#endif

struct input_event_compat {
	struct compat_timeval time;
	__u16 type;
	__u16 code;
	__s32 value;
};

struct ff_periodic_effect_compat {
	__u16 waveform;
	__u16 period;
	__s16 magnitude;
	__s16 offset;
	__u16 phase;

	struct ff_envelope envelope;

	__u32 custom_len;
	compat_uptr_t custom_data;
};

struct ff_effect_compat {
	__u16 type;
	__s16 id;
	__u16 direction;
	struct ff_trigger trigger;
	struct ff_replay replay;

	union {
		struct ff_constant_effect constant;
		struct ff_ramp_effect ramp;
		struct ff_periodic_effect_compat periodic;
		struct ff_condition_effect condition[2]; /* One for each axis */
		struct ff_rumble_effect rumble;
	} u;
};

static inline size_t input_event_size(void)
{
	return INPUT_COMPAT_TEST ?
		sizeof(struct input_event_compat) : sizeof(struct input_event);
}

#else

static inline size_t input_event_size(void)
{
	return sizeof(struct input_event);
}

#endif /* CONFIG_COMPAT */

int input_event_from_user(const char __user *buffer,
			 struct input_event *event);

int input_event_to_user(char __user *buffer,
			const struct input_event *event);

int input_ff_effect_from_user(const char __user *buffer, size_t size,
			      struct ff_effect *effect);

#endif /* _INPUT_COMPAT_H */
