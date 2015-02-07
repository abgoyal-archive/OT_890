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

#ifndef _ASM_X86_MACH_RDC321X_GPIO_H
#define _ASM_X86_MACH_RDC321X_GPIO_H

#include <linux/kernel.h>

extern int rdc_gpio_get_value(unsigned gpio);
extern void rdc_gpio_set_value(unsigned gpio, int value);
extern int rdc_gpio_direction_input(unsigned gpio);
extern int rdc_gpio_direction_output(unsigned gpio, int value);
extern int rdc_gpio_request(unsigned gpio, const char *label);
extern void rdc_gpio_free(unsigned gpio);
extern void __init rdc321x_gpio_setup(void);

/* Wrappers for the arch-neutral GPIO API */

static inline int gpio_request(unsigned gpio, const char *label)
{
	return rdc_gpio_request(gpio, label);
}

static inline void gpio_free(unsigned gpio)
{
	might_sleep();
	rdc_gpio_free(gpio);
}

static inline int gpio_direction_input(unsigned gpio)
{
	return rdc_gpio_direction_input(gpio);
}

static inline int gpio_direction_output(unsigned gpio, int value)
{
	return rdc_gpio_direction_output(gpio, value);
}

static inline int gpio_get_value(unsigned gpio)
{
	return rdc_gpio_get_value(gpio);
}

static inline void gpio_set_value(unsigned gpio, int value)
{
	rdc_gpio_set_value(gpio, value);
}

static inline int gpio_to_irq(unsigned gpio)
{
	return gpio;
}

static inline int irq_to_gpio(unsigned irq)
{
	return irq;
}

/* For cansleep */
#include <asm-generic/gpio.h>

#endif /* _ASM_X86_MACH_RDC321X_GPIO_H */
