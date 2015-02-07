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
 * linux/arch/arm/mach-pxa/leds-mainstone.c
 *
 * Author:     Nicolas Pitre
 * Created:    Nov 05, 2002
 * Copyright:  MontaVista Software Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>

#include <mach/hardware.h>
#include <asm/leds.h>
#include <asm/system.h>

#include <mach/pxa-regs.h>
#include <mach/mainstone.h>

#include "leds.h"


/* 8 discrete leds available for general use: */
#define D28			(1 << 0)
#define D27			(1 << 1)
#define D26			(1 << 2)
#define D25			(1 << 3)
#define D24			(1 << 4)
#define D23			(1 << 5)
#define D22			(1 << 6)
#define D21			(1 << 7)

#define LED_STATE_ENABLED	1
#define LED_STATE_CLAIMED	2

static unsigned int led_state;
static unsigned int hw_led_state;

void mainstone_leds_event(led_event_t evt)
{
	unsigned long flags;

	local_irq_save(flags);

	switch (evt) {
	case led_start:
		hw_led_state = 0;
		led_state = LED_STATE_ENABLED;
		break;

	case led_stop:
		led_state &= ~LED_STATE_ENABLED;
		break;

	case led_claim:
		led_state |= LED_STATE_CLAIMED;
		hw_led_state = 0;
		break;

	case led_release:
		led_state &= ~LED_STATE_CLAIMED;
		hw_led_state = 0;
		break;

#ifdef CONFIG_LEDS_TIMER
	case led_timer:
		hw_led_state ^= D26;
		break;
#endif

#ifdef CONFIG_LEDS_CPU
	case led_idle_start:
		hw_led_state &= ~D27;
		break;

	case led_idle_end:
		hw_led_state |= D27;
		break;
#endif

	case led_halted:
		break;

	case led_green_on:
		hw_led_state |= D21;
		break;

	case led_green_off:
		hw_led_state &= ~D21;
		break;

	case led_amber_on:
		hw_led_state |= D22;
		break;

	case led_amber_off:
		hw_led_state &= ~D22;
		break;

	case led_red_on:
		hw_led_state |= D23;
		break;

	case led_red_off:
		hw_led_state &= ~D23;
		break;

	default:
		break;
	}

	if  (led_state & LED_STATE_ENABLED)
		MST_LEDCTRL = (MST_LEDCTRL | 0xff) & ~hw_led_state;
	else
		MST_LEDCTRL |= 0xff;

	local_irq_restore(flags);
}
