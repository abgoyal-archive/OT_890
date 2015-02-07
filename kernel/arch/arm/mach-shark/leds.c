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
 * arch/arm/mach-shark/leds.c
 * by Alexander Schulz
 *
 * derived from:
 * arch/arm/kernel/leds-footbridge.c
 * Copyright (C) 1998-1999 Russell King
 *
 * DIGITAL Shark LED control routines.
 *
 * The leds use is as follows:
 *  - Green front - toggles state every 50 timer interrupts
 *  - Amber front - Unused, this is a dual color led (Amber/Green)
 *  - Amber back  - On if system is not idle
 *
 * Changelog:
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/ioport.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <asm/leds.h>
#include <asm/system.h>

#define LED_STATE_ENABLED	1
#define LED_STATE_CLAIMED	2
static char led_state;
static short hw_led_state;
static short saved_state;

static DEFINE_SPINLOCK(leds_lock);

short sequoia_read(int addr) {
  outw(addr,0x24);
  return inw(0x26);
}

void sequoia_write(short value,short addr) {
  outw(addr,0x24);
  outw(value,0x26);
}

static void sequoia_leds_event(led_event_t evt)
{
	unsigned long flags;

	spin_lock_irqsave(&leds_lock, flags);

	hw_led_state = sequoia_read(0x09);

	switch (evt) {
	case led_start:
		hw_led_state |= SEQUOIA_LED_GREEN;
		hw_led_state |= SEQUOIA_LED_AMBER;
#ifdef CONFIG_LEDS_CPU
		hw_led_state |= SEQUOIA_LED_BACK;
#else
		hw_led_state &= ~SEQUOIA_LED_BACK;
#endif
		led_state |= LED_STATE_ENABLED;
		break;

	case led_stop:
		hw_led_state &= ~SEQUOIA_LED_BACK;
		hw_led_state |= SEQUOIA_LED_GREEN;
		hw_led_state |= SEQUOIA_LED_AMBER;
		led_state &= ~LED_STATE_ENABLED;
		break;

	case led_claim:
		led_state |= LED_STATE_CLAIMED;
		saved_state = hw_led_state;
		hw_led_state &= ~SEQUOIA_LED_BACK;
		hw_led_state |= SEQUOIA_LED_GREEN;
		hw_led_state |= SEQUOIA_LED_AMBER;
		break;

	case led_release:
		led_state &= ~LED_STATE_CLAIMED;
		hw_led_state = saved_state;
		break;

#ifdef CONFIG_LEDS_TIMER
	case led_timer:
		if (!(led_state & LED_STATE_CLAIMED))
			hw_led_state ^= SEQUOIA_LED_GREEN;
		break;
#endif

#ifdef CONFIG_LEDS_CPU
	case led_idle_start:
		if (!(led_state & LED_STATE_CLAIMED))
			hw_led_state &= ~SEQUOIA_LED_BACK;
		break;

	case led_idle_end:
		if (!(led_state & LED_STATE_CLAIMED))
			hw_led_state |= SEQUOIA_LED_BACK;
		break;
#endif

	case led_green_on:
		if (led_state & LED_STATE_CLAIMED)
			hw_led_state &= ~SEQUOIA_LED_GREEN;
		break;

	case led_green_off:
		if (led_state & LED_STATE_CLAIMED)
			hw_led_state |= SEQUOIA_LED_GREEN;
		break;

	case led_amber_on:
		if (led_state & LED_STATE_CLAIMED)
			hw_led_state &= ~SEQUOIA_LED_AMBER;
		break;

	case led_amber_off:
		if (led_state & LED_STATE_CLAIMED)
			hw_led_state |= SEQUOIA_LED_AMBER;
		break;

	case led_red_on:
		if (led_state & LED_STATE_CLAIMED)
			hw_led_state |= SEQUOIA_LED_BACK;
		break;

	case led_red_off:
		if (led_state & LED_STATE_CLAIMED)
			hw_led_state &= ~SEQUOIA_LED_BACK;
		break;

	default:
		break;
	}

	if  (led_state & LED_STATE_ENABLED)
		sequoia_write(hw_led_state,0x09);

	spin_unlock_irqrestore(&leds_lock, flags);
}

static int __init leds_init(void)
{
	extern void (*leds_event)(led_event_t);
	short temp;
	
	leds_event = sequoia_leds_event;

	/* Make LEDs independent of power-state */
	request_region(0x24,4,"sequoia");
	temp = sequoia_read(0x09);
	temp |= 1<<10;
	sequoia_write(temp,0x09);
	leds_event(led_start);
	return 0;
}

__initcall(leds_init);
