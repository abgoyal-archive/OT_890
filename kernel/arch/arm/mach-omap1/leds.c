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
 * linux/arch/arm/mach-omap1/leds.c
 *
 * OMAP LEDs dispatcher
 */
#include <linux/kernel.h>
#include <linux/init.h>

#include <asm/leds.h>
#include <asm/mach-types.h>

#include <mach/gpio.h>
#include <mach/mux.h>

#include "leds.h"

static int __init
omap_leds_init(void)
{
	if (machine_is_omap_innovator())
		leds_event = innovator_leds_event;

	else if (machine_is_omap_h2()
			|| machine_is_omap_h3()
			|| machine_is_omap_perseus2())
		leds_event = h2p2_dbg_leds_event;

	else if (machine_is_omap_osk())
		leds_event = osk_leds_event;

	else
		return -1;

	if (machine_is_omap_h2()
			|| machine_is_omap_h3()
#ifdef	CONFIG_OMAP_OSK_MISTRAL
			|| machine_is_omap_osk()
#endif
			) {

		/* LED1/LED2 pins can be used as GPIO (as done here), or by
		 * the LPG (works even in deep sleep!), to drive a bicolor
		 * LED on the H2 sample board, and another on the H2/P2
		 * "surfer" expansion board.
		 *
		 * The same pins drive a LED on the OSK Mistral board, but
		 * that's a different kind of LED (just one color at a time).
		 */
		omap_cfg_reg(P18_1610_GPIO3);
		if (gpio_request(3, "LED red") == 0)
			gpio_direction_output(3, 1);
		else
			printk(KERN_WARNING "LED: can't get GPIO3/red?\n");

		omap_cfg_reg(MPUIO4);
		if (gpio_request(OMAP_MPUIO(4), "LED green") == 0)
			gpio_direction_output(OMAP_MPUIO(4), 1);
		else
			printk(KERN_WARNING "LED: can't get MPUIO4/green?\n");
	}

	leds_event(led_start);
	return 0;
}

__initcall(omap_leds_init);
