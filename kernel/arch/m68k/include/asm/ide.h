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
 *  linux/include/asm-m68k/ide.h
 *
 *  Copyright (C) 1994-1996  Linus Torvalds & authors
 */

/* Copyright(c) 1996 Kars de Jong */
/* Based on the ide driver from 1.2.13pl8 */

/*
 * Credits (alphabetical):
 *
 *  - Bjoern Brauel
 *  - Kars de Jong
 *  - Torsten Ebeling
 *  - Dwight Engen
 *  - Thorsten Floeck
 *  - Roman Hodek
 *  - Guenther Kelleter
 *  - Chris Lawrence
 *  - Michael Rausch
 *  - Christian Sauer
 *  - Michael Schmitz
 *  - Jes Soerensen
 *  - Michael Thurm
 *  - Geert Uytterhoeven
 */

#ifndef _M68K_IDE_H
#define _M68K_IDE_H

#ifdef __KERNEL__


#include <asm/setup.h>
#include <asm/io.h>
#include <asm/irq.h>

#ifdef CONFIG_ATARI
#include <linux/interrupt.h>
#include <asm/atari_stdma.h>
#endif

#ifdef CONFIG_MAC
#include <asm/macints.h>
#endif

/*
 * Get rid of defs from io.h - ide has its private and conflicting versions
 * Since so far no single m68k platform uses ISA/PCI I/O space for IDE, we
 * always use the `raw' MMIO versions
 */
#undef inb
#undef inw
#undef insw
#undef inl
#undef insl
#undef outb
#undef outw
#undef outsw
#undef outl
#undef outsl
#undef readb
#undef readw
#undef readl
#undef writeb
#undef writew
#undef writel

#define inb				in_8
#define inw				in_be16
#define insw(port, addr, n)		raw_insw((u16 *)port, addr, n)
#define inl				in_be32
#define insl(port, addr, n)		raw_insl((u32 *)port, addr, n)
#define outb(val, port)			out_8(port, val)
#define outw(val, port)			out_be16(port, val)
#define outsw(port, addr, n)		raw_outsw((u16 *)port, addr, n)
#define outl(val, port)			out_be32(port, val)
#define outsl(port, addr, n)		raw_outsl((u32 *)port, addr, n)
#define readb				in_8
#define readw				in_be16
#define __ide_mm_insw(port, addr, n)	raw_insw((u16 *)port, addr, n)
#define readl				in_be32
#define __ide_mm_insl(port, addr, n)	raw_insl((u32 *)port, addr, n)
#define writeb(val, port)		out_8(port, val)
#define writew(val, port)		out_be16(port, val)
#define __ide_mm_outsw(port, addr, n)	raw_outsw((u16 *)port, addr, n)
#define writel(val, port)		out_be32(port, val)
#define __ide_mm_outsl(port, addr, n)	raw_outsl((u32 *)port, addr, n)
#if defined(CONFIG_ATARI) || defined(CONFIG_Q40)
#define insw_swapw(port, addr, n)	raw_insw_swapw((u16 *)port, addr, n)
#define outsw_swapw(port, addr, n)	raw_outsw_swapw((u16 *)port, addr, n)
#endif

#ifdef CONFIG_BLK_DEV_FALCON_IDE
#define IDE_ARCH_LOCK

extern int falconide_intr_lock;

static __inline__ void ide_release_lock (void)
{
	if (MACH_IS_ATARI) {
		if (falconide_intr_lock == 0) {
			printk("ide_release_lock: bug\n");
			return;
		}
		falconide_intr_lock = 0;
		stdma_release();
	}
}

static __inline__ void
ide_get_lock(irq_handler_t handler, void *data)
{
	if (MACH_IS_ATARI) {
		if (falconide_intr_lock == 0) {
			if (in_interrupt() > 0)
				panic( "Falcon IDE hasn't ST-DMA lock in interrupt" );
			stdma_lock(handler, data);
			falconide_intr_lock = 1;
		}
	}
}
#endif /* CONFIG_BLK_DEV_FALCON_IDE */

#define IDE_ARCH_ACK_INTR
#define ide_ack_intr(hwif)	((hwif)->ack_intr ? (hwif)->ack_intr(hwif) : 1)

#endif /* __KERNEL__ */
#endif /* _M68K_IDE_H */
