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
 * arch/arm/plat-omap/include/mach/uncompress.h
 *
 * Serial port stubs for kernel decompress status messages
 *
 * Initially based on:
 * linux-2.4.15-rmk1-dsplinux1.6/arch/arm/plat-omap/include/mach1510/uncompress.h
 * Copyright (C) 2000 RidgeRun, Inc.
 * Author: Greg Lonnon <glonnon@ridgerun.com>
 *
 * Rewritten by:
 * Author: <source@mvista.com>
 * 2004 (c) MontaVista Software, Inc.
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#include <linux/types.h>
#include <linux/serial_reg.h>
#include <mach/serial.h>

unsigned int system_rev;

#define UART_OMAP_MDR1		0x08	/* mode definition register */
#define OMAP_ID_730		0x355F
#define ID_MASK			0x7fff
#define check_port(base, shift) ((base[UART_OMAP_MDR1 << shift] & 7) == 0)
#define omap_get_id() ((*(volatile unsigned int *)(0xfffed404)) >> 12) & ID_MASK

static void putc(int c)
{
	volatile u8 * uart = 0;
	int shift = 2;

#ifdef CONFIG_MACH_OMAP_PALMTE
	return;
#endif

#ifdef CONFIG_ARCH_OMAP
#ifdef	CONFIG_OMAP_LL_DEBUG_UART3
	uart = (volatile u8 *)(OMAP_UART3_BASE);
#elif defined(CONFIG_OMAP_LL_DEBUG_UART2)
	uart = (volatile u8 *)(OMAP_UART2_BASE);
#else
	uart = (volatile u8 *)(OMAP_UART1_BASE);
#endif

#ifdef CONFIG_ARCH_OMAP1
	/* Determine which serial port to use */
	do {
		/* MMU is not on, so cpu_is_omapXXXX() won't work here */
		unsigned int omap_id = omap_get_id();

		if (omap_id == OMAP_ID_730)
			shift = 0;

		if (check_port(uart, shift))
			break;
		/* Silent boot if no serial ports are enabled. */
		return;
	} while (0);
#endif /* CONFIG_ARCH_OMAP1 */
#endif

	/*
	 * Now, xmit each character
	 */
	while (!(uart[UART_LSR << shift] & UART_LSR_THRE))
		barrier();
	uart[UART_TX << shift] = c;
}

static inline void flush(void)
{
}

/*
 * nothing to do
 */
#define arch_decomp_setup()
#define arch_decomp_wdog()
