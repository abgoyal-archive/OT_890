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
 * linux/arch/arm/mach-pxa/leds-idp.c
 *
 * Copyright (C) 2000 John Dorsey <john+@cs.cmu.edu>
 *
 * Copyright (c) 2001 Jeff Sutherland <jeffs@accelent.com>
 *
 * Original (leds-footbridge.c) by Russell King
 *
 * Macros for actual LED manipulation should be in machine specific
 * files in this 'mach' directory.
 */


#include <linux/init.h>

#include <mach/hardware.h>
#include <asm/leds.h>
#include <asm/system.h>

#include <mach/pxa-regs.h>
#include <mach/idp.h>

#include "leds.h"

#define LED_STATE_ENABLED	1
#define LED_STATE_CLAIMED	2

static unsigned int led_state;
static unsigned int hw_led_state;

void idp_leds_event(led_event_t evt)
{
	unsigned long flags;

	local_irq_save(flags);

	switch (evt) {
	case led_start:
		hw_led_state = IDP_HB_LED | IDP_BUSY_LED;
		led_state = LED_STATE_ENABLED;
		break;

	case led_stop:
		led_state &= ~LED_STATE_ENABLED;
		break;

	case led_claim:
		led_state |= LED_STATE_CLAIMED;
		hw_led_state = IDP_HB_LED | IDP_BUSY_LED;
		break;

	case led_release:
		led_state &= ~LED_STATE_CLAIMED;
		hw_led_state = IDP_HB_LED | IDP_BUSY_LED;
		break;

#ifdef CONFIG_LEDS_TIMER
	case led_timer:
		if (!(led_state & LED_STATE_CLAIMED))
			hw_led_state ^= IDP_HB_LED;
		break;
#endif

#ifdef CONFIG_LEDS_CPU
	case led_idle_start:
		if (!(led_state & LED_STATE_CLAIMED))
			hw_led_state &= ~IDP_BUSY_LED;
		break;

	case led_idle_end:
		if (!(led_state & LED_STATE_CLAIMED))
			hw_led_state |= IDP_BUSY_LED;
		break;
#endif

	case led_halted:
		break;

	case led_green_on:
		if (led_state & LED_STATE_CLAIMED)
			hw_led_state |= IDP_HB_LED;
		break;

	case led_green_off:
		if (led_state & LED_STATE_CLAIMED)
			hw_led_state &= ~IDP_HB_LED;
		break;

	case led_amber_on:
		break;

	case led_amber_off:
		break;

	case led_red_on:
		if (led_state & LED_STATE_CLAIMED)
			hw_led_state |= IDP_BUSY_LED;
		break;

	case led_red_off:
		if (led_state & LED_STATE_CLAIMED)
			hw_led_state &= ~IDP_BUSY_LED;
		break;

	default:
		break;
	}

	if  (led_state & LED_STATE_ENABLED)
		IDP_CPLD_LED_CONTROL = ( (IDP_CPLD_LED_CONTROL | IDP_LEDS_MASK) & ~hw_led_state);
	else
		IDP_CPLD_LED_CONTROL |= IDP_LEDS_MASK;

	local_irq_restore(flags);
}
