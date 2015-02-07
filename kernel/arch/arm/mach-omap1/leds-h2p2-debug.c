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
 * linux/arch/arm/mach-omap1/leds-h2p2-debug.c
 *
 * Copyright 2003 by Texas Instruments Incorporated
 *
 * There are 16 LEDs on the debug board (all green); four may be used
 * for logical 'green', 'amber', 'red', and 'blue' (after "claiming").
 *
 * The "surfer" expansion board and H2 sample board also have two-color
 * green+red LEDs (in parallel), used here for timer and idle indicators.
 */
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/sched.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <asm/leds.h>
#include <asm/system.h>
#include <asm/mach-types.h>

#include <mach/fpga.h>
#include <mach/gpio.h>

#include "leds.h"


#define GPIO_LED_RED		3
#define GPIO_LED_GREEN		OMAP_MPUIO(4)


#define LED_STATE_ENABLED	0x01
#define LED_STATE_CLAIMED	0x02
#define LED_TIMER_ON		0x04

#define GPIO_IDLE		GPIO_LED_GREEN
#define GPIO_TIMER		GPIO_LED_RED


void h2p2_dbg_leds_event(led_event_t evt)
{
	unsigned long flags;

	static struct h2p2_dbg_fpga __iomem *fpga;
	static u16 led_state, hw_led_state;

	local_irq_save(flags);

	if (!(led_state & LED_STATE_ENABLED) && evt != led_start)
		goto done;

	switch (evt) {
	case led_start:
		if (!fpga)
			fpga = ioremap(H2P2_DBG_FPGA_START,
						H2P2_DBG_FPGA_SIZE);
		if (fpga) {
			led_state |= LED_STATE_ENABLED;
			__raw_writew(~0, &fpga->leds);
		}
		break;

	case led_stop:
	case led_halted:
		/* all leds off during suspend or shutdown */

		if (! machine_is_omap_perseus2()) {
			gpio_set_value(GPIO_TIMER, 0);
			gpio_set_value(GPIO_IDLE, 0);
		}

		__raw_writew(~0, &fpga->leds);
		led_state &= ~LED_STATE_ENABLED;
		if (evt == led_halted) {
			iounmap(fpga);
			fpga = NULL;
		}

		goto done;

	case led_claim:
		led_state |= LED_STATE_CLAIMED;
		hw_led_state = 0;
		break;

	case led_release:
		led_state &= ~LED_STATE_CLAIMED;
		break;

#ifdef CONFIG_LEDS_TIMER
	case led_timer:
		led_state ^= LED_TIMER_ON;

		if (machine_is_omap_perseus2())
			hw_led_state ^= H2P2_DBG_FPGA_P2_LED_TIMER;
		else {
			gpio_set_value(GPIO_TIMER, led_state & LED_TIMER_ON);
			goto done;
		}

		break;
#endif

#ifdef CONFIG_LEDS_CPU
	case led_idle_start:
		if (machine_is_omap_perseus2())
			hw_led_state |= H2P2_DBG_FPGA_P2_LED_IDLE;
		else {
			gpio_set_value(GPIO_IDLE, 1);
			goto done;
		}

		break;

	case led_idle_end:
		if (machine_is_omap_perseus2())
			hw_led_state &= ~H2P2_DBG_FPGA_P2_LED_IDLE;
		else {
			gpio_set_value(GPIO_IDLE, 0);
			goto done;
		}

		break;
#endif

	case led_green_on:
		hw_led_state |= H2P2_DBG_FPGA_LED_GREEN;
		break;
	case led_green_off:
		hw_led_state &= ~H2P2_DBG_FPGA_LED_GREEN;
		break;

	case led_amber_on:
		hw_led_state |= H2P2_DBG_FPGA_LED_AMBER;
		break;
	case led_amber_off:
		hw_led_state &= ~H2P2_DBG_FPGA_LED_AMBER;
		break;

	case led_red_on:
		hw_led_state |= H2P2_DBG_FPGA_LED_RED;
		break;
	case led_red_off:
		hw_led_state &= ~H2P2_DBG_FPGA_LED_RED;
		break;

	case led_blue_on:
		hw_led_state |= H2P2_DBG_FPGA_LED_BLUE;
		break;
	case led_blue_off:
		hw_led_state &= ~H2P2_DBG_FPGA_LED_BLUE;
		break;

	default:
		break;
	}


	/*
	 *  Actually burn the LEDs
	 */
	if (led_state & LED_STATE_ENABLED)
		__raw_writew(~hw_led_state, &fpga->leds);

done:
	local_irq_restore(flags);
}
