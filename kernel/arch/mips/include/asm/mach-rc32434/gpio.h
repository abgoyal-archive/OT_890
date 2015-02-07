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
 * Copyright 2002 Integrated Device Technology, Inc.
 *	All rights reserved.
 *
 * GPIO register definition.
 *
 * Author : ryan.holmQVist@idt.com
 * Date   : 20011005
 * Copyright (C) 2001, 2002 Ryan Holm <ryan.holmQVist@idt.com>
 * Copyright (C) 2008 Florian Fainelli <florian@openwrt.org>
 */

#ifndef _RC32434_GPIO_H_
#define _RC32434_GPIO_H_

#include <linux/types.h>
#include <asm-generic/gpio.h>

#define NR_BUILTIN_GPIO		32

#define gpio_get_value	__gpio_get_value
#define gpio_set_value	__gpio_set_value
#define gpio_cansleep	__gpio_cansleep

#define gpio_to_irq(gpio)	(8 + 4 * 32 + gpio)
#define irq_to_gpio(irq)	(irq - (8 + 4 * 32))

struct rb532_gpio_reg {
	u32   gpiofunc;   /* GPIO Function Register
			   * gpiofunc[x]==0 bit = gpio
			   * func[x]==1  bit = altfunc
			   */
	u32   gpiocfg;	  /* GPIO Configuration Register
			   * gpiocfg[x]==0 bit = input
			   * gpiocfg[x]==1 bit = output
			   */
	u32   gpiod;	  /* GPIO Data Register
			   * gpiod[x] read/write gpio pinX status
			   */
	u32   gpioilevel; /* GPIO Interrupt Status Register
			   * interrupt level (see gpioistat)
			   */
	u32   gpioistat;  /* Gpio Interrupt Status Register
			   * istat[x] = (gpiod[x] == level[x])
			   * cleared in ISR (STICKY bits)
			   */
	u32   gpionmien;  /* GPIO Non-maskable Interrupt Enable Register */
};

/* UART GPIO signals */
#define RC32434_UART0_SOUT	(1 << 0)
#define RC32434_UART0_SIN	(1 << 1)
#define RC32434_UART0_RTS	(1 << 2)
#define RC32434_UART0_CTS	(1 << 3)

/* M & P bus GPIO signals */
#define RC32434_MP_BIT_22	(1 << 4)
#define RC32434_MP_BIT_23	(1 << 5)
#define RC32434_MP_BIT_24	(1 << 6)
#define RC32434_MP_BIT_25	(1 << 7)

/* CPU GPIO signals */
#define RC32434_CPU_GPIO	(1 << 8)

/* Reserved GPIO signals */
#define RC32434_AF_SPARE_6	(1 << 9)
#define RC32434_AF_SPARE_4	(1 << 10)
#define RC32434_AF_SPARE_3	(1 << 11)
#define RC32434_AF_SPARE_2	(1 << 12)

/* PCI messaging unit */
#define RC32434_PCI_MSU_GPIO	(1 << 13)

/* NAND GPIO signals */
#define GPIO_RDY		8
#define GPIO_WPX	9
#define GPIO_ALE		10
#define GPIO_CLE		11

/* Compact Flash GPIO pin */
#define CF_GPIO_NUM		13

extern void rb532_gpio_set_ilevel(int bit, unsigned gpio);
extern void rb532_gpio_set_istat(int bit, unsigned gpio);
extern void rb532_gpio_set_func(unsigned gpio);

#endif /* _RC32434_GPIO_H_ */