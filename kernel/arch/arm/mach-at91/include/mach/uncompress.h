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
 * arch/arm/mach-at91/include/mach/uncompress.h
 *
 *  Copyright (C) 2003 SAN People
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __ASM_ARCH_UNCOMPRESS_H
#define __ASM_ARCH_UNCOMPRESS_H

#include <linux/io.h>
#include <linux/atmel_serial.h>

#if defined(CONFIG_AT91_EARLY_DBGU)
#define UART_OFFSET (AT91_DBGU + AT91_BASE_SYS)
#elif defined(CONFIG_AT91_EARLY_USART0)
#define UART_OFFSET AT91_USART0
#elif defined(CONFIG_AT91_EARLY_USART1)
#define UART_OFFSET AT91_USART1
#elif defined(CONFIG_AT91_EARLY_USART2)
#define UART_OFFSET AT91_USART2
#elif defined(CONFIG_AT91_EARLY_USART3)
#define UART_OFFSET AT91_USART3
#elif defined(CONFIG_AT91_EARLY_USART4)
#define UART_OFFSET AT91_USART4
#elif defined(CONFIG_AT91_EARLY_USART5)
#define UART_OFFSET AT91_USART5
#endif

/*
 * The following code assumes the serial port has already been
 * initialized by the bootloader.  If you didn't setup a port in
 * your bootloader then nothing will appear (which might be desired).
 *
 * This does not append a newline
 */
static void putc(int c)
{
#ifdef UART_OFFSET
	void __iomem *sys = (void __iomem *) UART_OFFSET;	/* physical address */

	while (!(__raw_readl(sys + ATMEL_US_CSR) & ATMEL_US_TXRDY))
		barrier();
	__raw_writel(c, sys + ATMEL_US_THR);
#endif
}

static inline void flush(void)
{
#ifdef UART_OFFSET
	void __iomem *sys = (void __iomem *) UART_OFFSET;	/* physical address */

	/* wait for transmission to complete */
	while (!(__raw_readl(sys + ATMEL_US_CSR) & ATMEL_US_TXEMPTY))
		barrier();
#endif
}

#define arch_decomp_setup()

#define arch_decomp_wdog()

#endif
