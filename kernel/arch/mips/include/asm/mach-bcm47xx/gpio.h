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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2007 Aurelien Jarno <aurelien@aurel32.net>
 */

#ifndef __BCM47XX_GPIO_H
#define __BCM47XX_GPIO_H

#include <linux/ssb/ssb_embedded.h>
#include <asm/mach-bcm47xx/bcm47xx.h>

#define BCM47XX_EXTIF_GPIO_LINES	5
#define BCM47XX_CHIPCO_GPIO_LINES	16

extern int gpio_request(unsigned gpio, const char *label);
extern void gpio_free(unsigned gpio);
extern int gpio_to_irq(unsigned gpio);

static inline int gpio_get_value(unsigned gpio)
{
	return ssb_gpio_in(&ssb_bcm47xx, 1 << gpio);
}

static inline void gpio_set_value(unsigned gpio, int value)
{
	ssb_gpio_out(&ssb_bcm47xx, 1 << gpio, value ? 1 << gpio : 0);
}

static inline int gpio_direction_input(unsigned gpio)
{
	return ssb_gpio_outen(&ssb_bcm47xx, 1 << gpio, 0);
}

static inline int gpio_direction_output(unsigned gpio, int value)
{
	return ssb_gpio_outen(&ssb_bcm47xx, 1 << gpio, 1 << gpio);
}

static int gpio_intmask(unsigned gpio, int value)
{
	return ssb_gpio_intmask(&ssb_bcm47xx, 1 << gpio,
				value ? 1 << gpio : 0);
}

static int gpio_polarity(unsigned gpio, int value)
{
	return ssb_gpio_polarity(&ssb_bcm47xx, 1 << gpio,
				 value ? 1 << gpio : 0);
}


/* cansleep wrappers */
#include <asm-generic/gpio.h>

#endif /* __BCM47XX_GPIO_H */
