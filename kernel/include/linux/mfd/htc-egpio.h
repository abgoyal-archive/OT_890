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
 * HTC simple EGPIO irq and gpio extender
 */

#ifndef __HTC_EGPIO_H__
#define __HTC_EGPIO_H__

#include <linux/gpio.h>

/* Descriptive values for all-in or all-out htc_egpio_chip descriptors. */
#define HTC_EGPIO_OUTPUT (~0)
#define HTC_EGPIO_INPUT  0

/**
 * struct htc_egpio_chip - descriptor to create gpio_chip for register range
 * @reg_start: index of first register
 * @gpio_base: gpio number of first pin in this register range
 * @num_gpios: number of gpios in this register range, max BITS_PER_LONG
 *    (number of registers = DIV_ROUND_UP(num_gpios, reg_width))
 * @direction: bitfield, '0' = input, '1' = output,
 */
struct htc_egpio_chip {
	int           reg_start;
	int           gpio_base;
	int           num_gpios;
	unsigned long direction;
	unsigned long initial_values;
};

/**
 * struct htc_egpio_platform_data - description provided by the arch
 * @irq_base: beginning of available IRQs (eg, IRQ_BOARD_START)
 * @num_irqs: number of irqs
 * @reg_width: number of bits per register, either 8 or 16 bit
 * @bus_width: alignment of the registers, either 16 or 32 bit
 * @invert_acks: set if chip requires writing '0' to ack an irq, instead of '1'
 * @ack_register: location of the irq/ack register
 * @chip: pointer to array of htc_egpio_chip descriptors
 * @num_chips: number of egpio chip descriptors
 */
struct htc_egpio_platform_data {
	int                   bus_width;
	int                   reg_width;

	int                   irq_base;
	int                   num_irqs;
	int                   invert_acks;
	int                   ack_register;

	struct htc_egpio_chip *chip;
	int                   num_chips;
};

/* Determine the wakeup irq, to be called during early resume */
extern int htc_egpio_get_wakeup_irq(struct device *dev);

#endif
