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
 * LED driver for KS8695-based boards.
 *
 * Copyright (C) Andrew Victor
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <asm/leds.h>
#include <mach/devices.h>
#include <mach/gpio.h>


static inline void ks8695_led_on(unsigned int led)
{
	gpio_set_value(led, 0);
}

static inline void ks8695_led_off(unsigned int led)
{
	gpio_set_value(led, 1);
}

static inline void ks8695_led_toggle(unsigned int led)
{
	unsigned long is_off = gpio_get_value(led);
	if (is_off)
		ks8695_led_on(led);
	else
		ks8695_led_off(led);
}


/*
 * Handle LED events.
 */
static void ks8695_leds_event(led_event_t evt)
{
	unsigned long flags;

	local_irq_save(flags);

	switch(evt) {
	case led_start:		/* System startup */
		ks8695_led_on(ks8695_leds_cpu);
		break;

	case led_stop:		/* System stop / suspend */
		ks8695_led_off(ks8695_leds_cpu);
		break;

#ifdef CONFIG_LEDS_TIMER
	case led_timer:		/* Every 50 timer ticks */
		ks8695_led_toggle(ks8695_leds_timer);
		break;
#endif

#ifdef CONFIG_LEDS_CPU
	case led_idle_start:	/* Entering idle state */
		ks8695_led_off(ks8695_leds_cpu);
		break;

	case led_idle_end:	/* Exit idle state */
		ks8695_led_on(ks8695_leds_cpu);
		break;
#endif

	default:
		break;
	}

	local_irq_restore(flags);
}


static int __init leds_init(void)
{
	if ((ks8695_leds_timer == -1) || (ks8695_leds_cpu == -1))
		return -ENODEV;

	leds_event = ks8695_leds_event;

	leds_event(led_start);
	return 0;
}

__initcall(leds_init);
