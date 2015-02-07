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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 1997, 1998, 2003 by Ralf Baechle
 */
#ifndef __ASM_MACH_GENERIC_FLOPPY_H
#define __ASM_MACH_GENERIC_FLOPPY_H

#include <linux/delay.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/sched.h>
#include <linux/linkage.h>
#include <linux/types.h>
#include <linux/mm.h>

#include <asm/bootinfo.h>
#include <asm/cachectl.h>
#include <asm/dma.h>
#include <asm/floppy.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/pgtable.h>

/*
 * How to access the FDC's registers.
 */
static inline unsigned char fd_inb(unsigned int port)
{
	return inb_p(port);
}

static inline void fd_outb(unsigned char value, unsigned int port)
{
	outb_p(value, port);
}

/*
 * How to access the floppy DMA functions.
 */
static inline void fd_enable_dma(void)
{
	enable_dma(FLOPPY_DMA);
}

static inline void fd_disable_dma(void)
{
	disable_dma(FLOPPY_DMA);
}

static inline int fd_request_dma(void)
{
	return request_dma(FLOPPY_DMA, "floppy");
}

static inline void fd_free_dma(void)
{
	free_dma(FLOPPY_DMA);
}

static inline void fd_clear_dma_ff(void)
{
	clear_dma_ff(FLOPPY_DMA);
}

static inline void fd_set_dma_mode(char mode)
{
	set_dma_mode(FLOPPY_DMA, mode);
}

static inline void fd_set_dma_addr(char *addr)
{
	set_dma_addr(FLOPPY_DMA, (unsigned long) addr);
}

static inline void fd_set_dma_count(unsigned int count)
{
	set_dma_count(FLOPPY_DMA, count);
}

static inline int fd_get_dma_residue(void)
{
	return get_dma_residue(FLOPPY_DMA);
}

static inline void fd_enable_irq(void)
{
	enable_irq(FLOPPY_IRQ);
}

static inline void fd_disable_irq(void)
{
	disable_irq(FLOPPY_IRQ);
}

static inline int fd_request_irq(void)
{
	return request_irq(FLOPPY_IRQ, floppy_interrupt,
	                   IRQF_DISABLED, "floppy", NULL);
}

static inline void fd_free_irq(void)
{
	free_irq(FLOPPY_IRQ, NULL);
}

#define fd_free_irq()           free_irq(FLOPPY_IRQ, NULL);


static inline unsigned long fd_getfdaddr1(void)
{
	return 0x3f0;
}

static inline unsigned long fd_dma_mem_alloc(unsigned long size)
{
	unsigned long mem;

	mem = __get_dma_pages(GFP_KERNEL, get_order(size));

	return mem;
}

static inline void fd_dma_mem_free(unsigned long addr, unsigned long size)
{
	free_pages(addr, get_order(size));
}

static inline unsigned long fd_drive_type(unsigned long n)
{
	if (n == 0)
		return 4;	/* 3,5", 1.44mb */

	return 0;
}

#endif /* __ASM_MACH_GENERIC_FLOPPY_H */
