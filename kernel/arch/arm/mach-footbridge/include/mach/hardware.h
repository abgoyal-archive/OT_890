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
 *  arch/arm/mach-footbridge/include/mach/hardware.h
 *
 *  Copyright (C) 1998-1999 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  This file contains the hardware definitions of the EBSA-285.
 */
#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

/*   Virtual      Physical	Size
 * 0xff800000	0x40000000	1MB	X-Bus
 * 0xff000000	0x7c000000	1MB	PCI I/O space
 * 0xfe000000	0x42000000	1MB	CSR
 * 0xfd000000	0x78000000	1MB	Outbound write flush (not supported)
 * 0xfc000000	0x79000000	1MB	PCI IACK/special space
 * 0xfb000000	0x7a000000	16MB	PCI Config type 1
 * 0xfa000000	0x7b000000	16MB	PCI Config type 0
 * 0xf9000000	0x50000000	1MB	Cache flush
 * 0xf0000000	0x80000000	16MB	ISA memory
 */
#define XBUS_SIZE		0x00100000
#define XBUS_BASE		0xff800000

#define ARMCSR_SIZE		0x00100000
#define ARMCSR_BASE		0xfe000000

#define WFLUSH_SIZE		0x00100000
#define WFLUSH_BASE		0xfd000000

#define PCIIACK_SIZE		0x00100000
#define PCIIACK_BASE		0xfc000000

#define PCICFG1_SIZE		0x01000000
#define PCICFG1_BASE		0xfb000000

#define PCICFG0_SIZE		0x01000000
#define PCICFG0_BASE		0xfa000000

#define PCIMEM_SIZE		0x01000000
#define PCIMEM_BASE		0xf0000000

#define XBUS_LEDS		((volatile unsigned char *)(XBUS_BASE + 0x12000))
#define XBUS_LED_AMBER		(1 << 0)
#define XBUS_LED_GREEN		(1 << 1)
#define XBUS_LED_RED		(1 << 2)
#define XBUS_LED_TOGGLE		(1 << 8)

#define XBUS_SWITCH		((volatile unsigned char *)(XBUS_BASE + 0x12000))
#define XBUS_SWITCH_SWITCH	((*XBUS_SWITCH) & 15)
#define XBUS_SWITCH_J17_13	((*XBUS_SWITCH) & (1 << 4))
#define XBUS_SWITCH_J17_11	((*XBUS_SWITCH) & (1 << 5))
#define XBUS_SWITCH_J17_9	((*XBUS_SWITCH) & (1 << 6))

#define UNCACHEABLE_ADDR	(ARMCSR_BASE + 0x108)


/* PIC irq control */
#define PIC_LO			0x20
#define PIC_MASK_LO		0x21
#define PIC_HI			0xA0
#define PIC_MASK_HI		0xA1

/* GPIO pins */
#define GPIO_CCLK		0x800
#define GPIO_DSCLK		0x400
#define GPIO_E2CLK		0x200
#define GPIO_IOLOAD		0x100
#define GPIO_RED_LED		0x080
#define GPIO_WDTIMER		0x040
#define GPIO_DATA		0x020
#define GPIO_IOCLK		0x010
#define GPIO_DONE		0x008
#define GPIO_FAN		0x004
#define GPIO_GREEN_LED		0x002
#define GPIO_RESET		0x001

/* CPLD pins */
#define CPLD_DS_ENABLE		8
#define CPLD_7111_DISABLE	4
#define CPLD_UNMUTE		2
#define CPLD_FLASH_WR_ENABLE	1

#ifndef __ASSEMBLY__
extern spinlock_t nw_gpio_lock;
extern void nw_gpio_modify_op(unsigned int mask, unsigned int set);
extern void nw_gpio_modify_io(unsigned int mask, unsigned int in);
extern unsigned int nw_gpio_read(void);
extern void nw_cpld_modify(unsigned int mask, unsigned int set);
#endif

#define pcibios_assign_all_busses()	1

#define PCIBIOS_MIN_IO		0x1000
#define PCIBIOS_MIN_MEM 	0x81000000

#endif
