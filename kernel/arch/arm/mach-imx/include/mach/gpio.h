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

#ifndef _IMX_GPIO_H

#include <linux/kernel.h>
#include <mach/imx-regs.h>

#define IMX_GPIO_ALLOC_MODE_NORMAL	0
#define IMX_GPIO_ALLOC_MODE_NO_ALLOC	1
#define IMX_GPIO_ALLOC_MODE_TRY_ALLOC	2
#define IMX_GPIO_ALLOC_MODE_ALLOC_ONLY	4
#define IMX_GPIO_ALLOC_MODE_RELEASE	8

extern int imx_gpio_request(unsigned gpio, const char *label);

extern void imx_gpio_free(unsigned gpio);

extern int imx_gpio_setup_multiple_pins(const int *pin_list, unsigned count,
					int alloc_mode, const char *label);

extern int imx_gpio_direction_input(unsigned gpio);

extern int imx_gpio_direction_output(unsigned gpio, int value);

extern void __imx_gpio_set_value(unsigned gpio, int value);

static inline int imx_gpio_get_value(unsigned gpio)
{
	return SSR(gpio >> GPIO_PORT_SHIFT) & (1 << (gpio & GPIO_PIN_MASK));
}

static inline void imx_gpio_set_value_inline(unsigned gpio, int value)
{
	unsigned long flags;

	raw_local_irq_save(flags);
	if(value)
		DR(gpio >> GPIO_PORT_SHIFT) |= (1 << (gpio & GPIO_PIN_MASK));
	else
		DR(gpio >> GPIO_PORT_SHIFT) &= ~(1 << (gpio & GPIO_PIN_MASK));
	raw_local_irq_restore(flags);
}

static inline void imx_gpio_set_value(unsigned gpio, int value)
{
	if(__builtin_constant_p(gpio))
		imx_gpio_set_value_inline(gpio, value);
	else
		__imx_gpio_set_value(gpio, value);
}

extern int imx_gpio_to_irq(unsigned gpio);

extern int imx_irq_to_gpio(unsigned irq);

/*-------------------------------------------------------------------------*/

/* Wrappers for "new style" GPIO calls. These calls i.MX specific versions
 * to allow future extension of GPIO logic.
 */

static inline int gpio_request(unsigned gpio, const char *label)
{
	return imx_gpio_request(gpio, label);
}

static inline void gpio_free(unsigned gpio)
{
	might_sleep();

	imx_gpio_free(gpio);
}

static inline  int gpio_direction_input(unsigned gpio)
{
	return imx_gpio_direction_input(gpio);
}

static inline int gpio_direction_output(unsigned gpio, int value)
{
	return imx_gpio_direction_output(gpio, value);
}

static inline int gpio_get_value(unsigned gpio)
{
	return imx_gpio_get_value(gpio);
}

static inline void gpio_set_value(unsigned gpio, int value)
{
	imx_gpio_set_value(gpio, value);
}

#include <asm-generic/gpio.h>		/* cansleep wrappers */

static inline int gpio_to_irq(unsigned gpio)
{
	return imx_gpio_to_irq(gpio);
}

static inline int irq_to_gpio(unsigned irq)
{
	return imx_irq_to_gpio(irq);
}


#endif
