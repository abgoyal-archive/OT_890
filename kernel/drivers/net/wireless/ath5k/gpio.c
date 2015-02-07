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
 * Copyright (c) 2004-2008 Reyk Floeter <reyk@openbsd.org>
 * Copyright (c) 2006-2008 Nick Kossifidis <mickflemm@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

/****************\
  GPIO Functions
\****************/

#include "ath5k.h"
#include "reg.h"
#include "debug.h"
#include "base.h"

/*
 * Set led state
 */
void ath5k_hw_set_ledstate(struct ath5k_hw *ah, unsigned int state)
{
	u32 led;
	/*5210 has different led mode handling*/
	u32 led_5210;

	ATH5K_TRACE(ah->ah_sc);

	/*Reset led status*/
	if (ah->ah_version != AR5K_AR5210)
		AR5K_REG_DISABLE_BITS(ah, AR5K_PCICFG,
			AR5K_PCICFG_LEDMODE |  AR5K_PCICFG_LED);
	else
		AR5K_REG_DISABLE_BITS(ah, AR5K_PCICFG, AR5K_PCICFG_LED);

	/*
	 * Some blinking values, define at your wish
	 */
	switch (state) {
	case AR5K_LED_SCAN:
	case AR5K_LED_AUTH:
		led = AR5K_PCICFG_LEDMODE_PROP | AR5K_PCICFG_LED_PEND;
		led_5210 = AR5K_PCICFG_LED_PEND | AR5K_PCICFG_LED_BCTL;
		break;

	case AR5K_LED_INIT:
		led = AR5K_PCICFG_LEDMODE_PROP | AR5K_PCICFG_LED_NONE;
		led_5210 = AR5K_PCICFG_LED_PEND;
		break;

	case AR5K_LED_ASSOC:
	case AR5K_LED_RUN:
		led = AR5K_PCICFG_LEDMODE_PROP | AR5K_PCICFG_LED_ASSOC;
		led_5210 = AR5K_PCICFG_LED_ASSOC;
		break;

	default:
		led = AR5K_PCICFG_LEDMODE_PROM | AR5K_PCICFG_LED_NONE;
		led_5210 = AR5K_PCICFG_LED_PEND;
		break;
	}

	/*Write new status to the register*/
	if (ah->ah_version != AR5K_AR5210)
		AR5K_REG_ENABLE_BITS(ah, AR5K_PCICFG, led);
	else
		AR5K_REG_ENABLE_BITS(ah, AR5K_PCICFG, led_5210);
}

/*
 * Set GPIO inputs
 */
int ath5k_hw_set_gpio_input(struct ath5k_hw *ah, u32 gpio)
{
	ATH5K_TRACE(ah->ah_sc);
	if (gpio > AR5K_NUM_GPIO)
		return -EINVAL;

	ath5k_hw_reg_write(ah,
		(ath5k_hw_reg_read(ah, AR5K_GPIOCR) & ~AR5K_GPIOCR_OUT(gpio))
		| AR5K_GPIOCR_IN(gpio), AR5K_GPIOCR);

	return 0;
}

/*
 * Set GPIO outputs
 */
int ath5k_hw_set_gpio_output(struct ath5k_hw *ah, u32 gpio)
{
	ATH5K_TRACE(ah->ah_sc);
	if (gpio > AR5K_NUM_GPIO)
		return -EINVAL;

	ath5k_hw_reg_write(ah,
		(ath5k_hw_reg_read(ah, AR5K_GPIOCR) & ~AR5K_GPIOCR_OUT(gpio))
		| AR5K_GPIOCR_OUT(gpio), AR5K_GPIOCR);

	return 0;
}

/*
 * Get GPIO state
 */
u32 ath5k_hw_get_gpio(struct ath5k_hw *ah, u32 gpio)
{
	ATH5K_TRACE(ah->ah_sc);
	if (gpio > AR5K_NUM_GPIO)
		return 0xffffffff;

	/* GPIO input magic */
	return ((ath5k_hw_reg_read(ah, AR5K_GPIODI) & AR5K_GPIODI_M) >> gpio) &
		0x1;
}

/*
 * Set GPIO state
 */
int ath5k_hw_set_gpio(struct ath5k_hw *ah, u32 gpio, u32 val)
{
	u32 data;
	ATH5K_TRACE(ah->ah_sc);

	if (gpio > AR5K_NUM_GPIO)
		return -EINVAL;

	/* GPIO output magic */
	data = ath5k_hw_reg_read(ah, AR5K_GPIODO);

	data &= ~(1 << gpio);
	data |= (val & 1) << gpio;

	ath5k_hw_reg_write(ah, data, AR5K_GPIODO);

	return 0;
}

/*
 * Initialize the GPIO interrupt (RFKill switch)
 */
void ath5k_hw_set_gpio_intr(struct ath5k_hw *ah, unsigned int gpio,
		u32 interrupt_level)
{
	u32 data;

	ATH5K_TRACE(ah->ah_sc);
	if (gpio > AR5K_NUM_GPIO)
		return;

	/*
	 * Set the GPIO interrupt
	 */
	data = (ath5k_hw_reg_read(ah, AR5K_GPIOCR) &
		~(AR5K_GPIOCR_INT_SEL(gpio) | AR5K_GPIOCR_INT_SELH |
		AR5K_GPIOCR_INT_ENA | AR5K_GPIOCR_OUT(gpio))) |
		(AR5K_GPIOCR_INT_SEL(gpio) | AR5K_GPIOCR_INT_ENA);

	ath5k_hw_reg_write(ah, interrupt_level ? data :
		(data | AR5K_GPIOCR_INT_SELH), AR5K_GPIOCR);

	ah->ah_imr |= AR5K_IMR_GPIO;

	/* Enable GPIO interrupts */
	AR5K_REG_ENABLE_BITS(ah, AR5K_PIMR, AR5K_IMR_GPIO);
}

