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
 * arch/arm/mach-ep93xx/include/mach/uncompress.h
 *
 * Copyright (C) 2006 Lennert Buytenhek <buytenh@wantstofly.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */

#include <mach/ep93xx-regs.h>

static unsigned char __raw_readb(unsigned int ptr)
{
	return *((volatile unsigned char *)ptr);
}

static unsigned int __raw_readl(unsigned int ptr)
{
	return *((volatile unsigned int *)ptr);
}

static void __raw_writeb(unsigned char value, unsigned int ptr)
{
	*((volatile unsigned char *)ptr) = value;
}

static void __raw_writel(unsigned int value, unsigned int ptr)
{
	*((volatile unsigned int *)ptr) = value;
}

#if defined(CONFIG_EP93XX_EARLY_UART1)
#define UART_BASE		EP93XX_UART1_PHYS_BASE
#elif defined(CONFIG_EP93XX_EARLY_UART2)
#define UART_BASE		EP93XX_UART2_PHYS_BASE
#elif defined(CONFIG_EP93XX_EARLY_UART3)
#define UART_BASE		EP93XX_UART3_PHYS_BASE
#else
#define UART_BASE		EP93XX_UART1_PHYS_BASE
#endif

#define PHYS_UART_DATA		(UART_BASE + 0x00)
#define PHYS_UART_FLAG		(UART_BASE + 0x18)
#define UART_FLAG_TXFF		0x20

static inline void putc(int c)
{
	int i;

	for (i = 0; i < 1000; i++) {
		/* Transmit fifo not full?  */
		if (!(__raw_readb(PHYS_UART_FLAG) & UART_FLAG_TXFF))
			break;
	}

	__raw_writeb(c, PHYS_UART_DATA);
}

static inline void flush(void)
{
}


/*
 * Some bootloaders don't turn off DMA from the ethernet MAC before
 * jumping to linux, which means that we might end up with bits of RX
 * status and packet data scribbled over the uncompressed kernel image.
 * Work around this by resetting the ethernet MAC before we uncompress.
 */
#define PHYS_ETH_SELF_CTL		0x80010020
#define ETH_SELF_CTL_RESET		0x00000001

static void ethernet_reset(void)
{
	unsigned int v;

	/* Reset the ethernet MAC.  */
	v = __raw_readl(PHYS_ETH_SELF_CTL);
	__raw_writel(v | ETH_SELF_CTL_RESET, PHYS_ETH_SELF_CTL);

	/* Wait for reset to finish.  */
	while (__raw_readl(PHYS_ETH_SELF_CTL) & ETH_SELF_CTL_RESET)
		;
}


static void arch_decomp_setup(void)
{
	ethernet_reset();
}

#define arch_decomp_wdog()
