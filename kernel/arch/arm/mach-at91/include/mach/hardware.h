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
 * arch/arm/mach-at91/include/mach/hardware.h
 *
 *  Copyright (C) 2003 SAN People
 *  Copyright (C) 2003 ATMEL
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

#include <asm/sizes.h>

#if defined(CONFIG_ARCH_AT91RM9200)
#include <mach/at91rm9200.h>
#elif defined(CONFIG_ARCH_AT91SAM9260) || defined(CONFIG_ARCH_AT91SAM9G20)
#include <mach/at91sam9260.h>
#elif defined(CONFIG_ARCH_AT91SAM9261)
#include <mach/at91sam9261.h>
#elif defined(CONFIG_ARCH_AT91SAM9263)
#include <mach/at91sam9263.h>
#elif defined(CONFIG_ARCH_AT91SAM9RL)
#include <mach/at91sam9rl.h>
#elif defined(CONFIG_ARCH_AT91CAP9)
#include <mach/at91cap9.h>
#elif defined(CONFIG_ARCH_AT91X40)
#include <mach/at91x40.h>
#else
#error "Unsupported AT91 processor"
#endif


#ifdef CONFIG_MMU
/*
 * Remap the peripherals from address 0xFFF78000 .. 0xFFFFFFFF
 * to 0xFEF78000 .. 0xFF000000.  (544Kb)
 */
#define AT91_IO_PHYS_BASE	0xFFF78000
#define AT91_IO_VIRT_BASE	(0xFF000000 - AT91_IO_SIZE)
#else
/*
 * Identity mapping for the non MMU case.
 */
#define AT91_IO_PHYS_BASE	AT91_BASE_SYS
#define AT91_IO_VIRT_BASE	AT91_IO_PHYS_BASE
#endif

#define AT91_IO_SIZE		(0xFFFFFFFF - AT91_IO_PHYS_BASE + 1)

 /* Convert a physical IO address to virtual IO address */
#define AT91_IO_P2V(x)		((x) - AT91_IO_PHYS_BASE + AT91_IO_VIRT_BASE)

/*
 * Virtual to Physical Address mapping for IO devices.
 */
#define AT91_VA_BASE_SYS	AT91_IO_P2V(AT91_BASE_SYS)
#define AT91_VA_BASE_EMAC	AT91_IO_P2V(AT91RM9200_BASE_EMAC)

 /* Internal SRAM is mapped below the IO devices */
#define AT91_SRAM_MAX		SZ_1M
#define AT91_VIRT_BASE		(AT91_IO_VIRT_BASE - AT91_SRAM_MAX)

/* Serial ports */
#define ATMEL_MAX_UART		7		/* 6 USART3's and one DBGU port (SAM9260) */

/* External Memory Map */
#define AT91_CHIPSELECT_0	0x10000000
#define AT91_CHIPSELECT_1	0x20000000
#define AT91_CHIPSELECT_2	0x30000000
#define AT91_CHIPSELECT_3	0x40000000
#define AT91_CHIPSELECT_4	0x50000000
#define AT91_CHIPSELECT_5	0x60000000
#define AT91_CHIPSELECT_6	0x70000000
#define AT91_CHIPSELECT_7	0x80000000

/* SDRAM */
#ifdef CONFIG_DRAM_BASE
#define AT91_SDRAM_BASE		CONFIG_DRAM_BASE
#else
#define AT91_SDRAM_BASE		AT91_CHIPSELECT_1
#endif

/* Clocks */
#define AT91_SLOW_CLOCK		32768		/* slow clock */


#endif
