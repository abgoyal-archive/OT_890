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
 * linux/arch/arm/mach-omap1/leds-osk.c
 *
 * LED driver for OSK with optional Mistral QVGA board
 */
#include <linux/init.h>

#include <mach/hardware.h>
#include <asm/leds.h>
#include <asm/system.h>

#include <mach/gpio.h>

#include "leds.h"


#define LED_STATE_ENABLED	(1 << 0)
#define LED_STATE_CLAIMED	(1 << 1)
static u8 led_state;

#define	TIMER_LED		(1 << 3)	/* Mistral board */
#define	IDLE_LED		(1 << 4)	/* Mistral board */
static u8 hw_led_state;


#ifdef	CONFIG_OMAP_OSK_MISTRAL

/* For now, all system indicators require the Mistral board, since that
 * LED can be manipulated without a task context.  This LED is either red,
 * or green, but not both; it can't give the full "disco led" effect.
 */

#define GPIO_LED_RED		3
#define GPIO_LED_GREEN		OMAP_MPUIO(4)

static void mistral_setled(void)
{
	int	red = 0;
	int	green = 0;

	if (hw_led_state & TIMER_LED)
		red = 1;
	else if (hw_led_state & IDLE_LED)
		green = 1;
	/* else both sides are disabled */

	gpio_set_value(GPIO_LED_GREEN, green);
	gpio_set_value(GPIO_LED_RED, red);
}

#endif

void osk_leds_event(led_event_t evt)
{
	unsigned long	flags;
	u16		leds;

	local_irq_save(flags);

	if (!(led_state & LED_STATE_ENABLED) && evt != led_start)
		goto done;

	leds = hw_led_state;
	switch (evt) {
	case led_start:
		led_state |= LED_STATE_ENABLED;
		hw_led_state = 0;
		leds = ~0;
		break;

	case led_halted:
	case led_stop:
		led_state &= ~LED_STATE_ENABLED;
		hw_led_state = 0;
		break;

	case led_claim:
		led_state |= LED_STATE_CLAIMED;
		hw_led_state = 0;
		leds = ~0;
		break;

	case led_release:
		led_state &= ~LED_STATE_CLAIMED;
		hw_led_state = 0;
		break;

#ifdef	CONFIG_OMAP_OSK_MISTRAL

	case led_timer:
		hw_led_state ^= TIMER_LED;
		mistral_setled();
		break;

	case led_idle_start:	/* idle == off */
		hw_led_state &= ~IDLE_LED;
		mistral_setled();
		break;

	case led_idle_end:
		hw_led_state |= IDLE_LED;
		mistral_setled();
		break;

#endif	/* CONFIG_OMAP_OSK_MISTRAL */

	default:
		break;
	}

	leds ^= hw_led_state;

done:
	local_irq_restore(flags);
}
