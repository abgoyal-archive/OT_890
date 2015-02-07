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
 * linux/arch/arm/mach-sa1100/leds-hackkit.c
 *
 * based on leds-lart.c
 *
 * (C) Erik Mouw (J.A.K.Mouw@its.tudelft.nl), April 21, 2000
 * (C) Stefan Eletzhofer <stefan.eletzhofer@eletztrick.de>, 2002
 *
 * The HackKit has two leds (GPIO 22/23). The red led (gpio 22) is used
 * as cpu led, the green one is used as timer led.
 */
#include <linux/init.h>

#include <mach/hardware.h>
#include <asm/leds.h>
#include <asm/system.h>

#include "leds.h"


#define LED_STATE_ENABLED	1
#define LED_STATE_CLAIMED	2

static unsigned int led_state;
static unsigned int hw_led_state;

#define LED_GREEN    GPIO_GPIO23
#define LED_RED    GPIO_GPIO22
#define LED_MASK  (LED_RED | LED_GREEN)

void hackkit_leds_event(led_event_t evt)
{
	unsigned long flags;

	local_irq_save(flags);

	switch(evt) {
		case led_start:
			/* pin 22/23 are outputs */
			GPDR |= LED_MASK;
			hw_led_state = LED_MASK;
			led_state = LED_STATE_ENABLED;
			break;

		case led_stop:
			led_state &= ~LED_STATE_ENABLED;
			break;

		case led_claim:
			led_state |= LED_STATE_CLAIMED;
			hw_led_state = LED_MASK;
			break;

		case led_release:
			led_state &= ~LED_STATE_CLAIMED;
			hw_led_state = LED_MASK;
			break;

#ifdef CONFIG_LEDS_TIMER
		case led_timer:
			if (!(led_state & LED_STATE_CLAIMED))
				hw_led_state ^= LED_GREEN;
			break;
#endif

#ifdef CONFIG_LEDS_CPU
		case led_idle_start:
			/* The LART people like the LED to be off when the
			   system is idle... */
			if (!(led_state & LED_STATE_CLAIMED))
				hw_led_state &= ~LED_RED;
			break;

		case led_idle_end:
			/* ... and on if the system is not idle */
			if (!(led_state & LED_STATE_CLAIMED))
				hw_led_state |= LED_RED;
			break;
#endif

		case led_red_on:
			if (led_state & LED_STATE_CLAIMED)
				hw_led_state &= ~LED_RED;
			break;

		case led_red_off:
			if (led_state & LED_STATE_CLAIMED)
				hw_led_state |= LED_RED;
			break;

		case led_green_on:
			if (led_state & LED_STATE_CLAIMED)
				hw_led_state &= ~LED_GREEN;
			break;

		case led_green_off:
			if (led_state & LED_STATE_CLAIMED)
				hw_led_state |= LED_GREEN;
			break;

		default:
			break;
	}

	/* Now set the GPIO state, or nothing will happen at all */
	if (led_state & LED_STATE_ENABLED) {
		GPSR = hw_led_state;
		GPCR = hw_led_state ^ LED_MASK;
	}

	local_irq_restore(flags);
}
