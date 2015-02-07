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
 * arch/arm/mach-ixp2000/include/mach/io.h
 *
 * Original Author: Naeem M Afzal <naeem.m.afzal@intel.com>
 * Maintainer: Deepak Saxena <dsaxena@plexity.net>
 *
 * Copyright (C) 2002  Intel Corp.
 * Copyrgiht (C) 2003-2004 MontaVista Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H

#include <mach/hardware.h>

#define IO_SPACE_LIMIT		0xffffffff
#define __mem_pci(a)		(a)

/*
 * The A? revisions of the IXP2000s assert byte lanes for PCI I/O
 * transactions the other way round (MEM transactions don't have this
 * issue), so if we want to support those models, we need to override
 * the standard I/O functions.
 *
 * B0 and later have a bit that can be set to 1 to get the proper
 * behavior for I/O transactions, which then allows us to use the
 * standard I/O functions.  This is what we do if the user does not
 * explicitly ask for support for pre-B0.
 */
#ifdef CONFIG_IXP2000_SUPPORT_BROKEN_PCI_IO
#define ___io(p)		((void __iomem *)((p)+IXP2000_PCI_IO_VIRT_BASE))

#define alignb(addr)		(void __iomem *)((unsigned long)(addr) ^ 3)
#define alignw(addr)		(void __iomem *)((unsigned long)(addr) ^ 2)

#define outb(v,p)		__raw_writeb((v),alignb(___io(p)))
#define outw(v,p)		__raw_writew((v),alignw(___io(p)))
#define outl(v,p)		__raw_writel((v),___io(p))

#define inb(p)		({ unsigned int __v = __raw_readb(alignb(___io(p))); __v; })
#define inw(p)		\
	({ unsigned int __v = (__raw_readw(alignw(___io(p)))); __v; })
#define inl(p)		\
	({ unsigned int __v = (__raw_readl(___io(p))); __v; })

#define outsb(p,d,l)		__raw_writesb(alignb(___io(p)),d,l)
#define outsw(p,d,l)		__raw_writesw(alignw(___io(p)),d,l)
#define outsl(p,d,l)		__raw_writesl(___io(p),d,l)

#define insb(p,d,l)		__raw_readsb(alignb(___io(p)),d,l)
#define insw(p,d,l)		__raw_readsw(alignw(___io(p)),d,l)
#define insl(p,d,l)		__raw_readsl(___io(p),d,l)

#define __is_io_address(p)	((((unsigned long)(p)) & ~(IXP2000_PCI_IO_SIZE - 1)) == IXP2000_PCI_IO_VIRT_BASE)

#define ioread8(p)						\
	({							\
		unsigned int __v;				\
								\
		if (__is_io_address(p)) {			\
			__v = __raw_readb(alignb(p));		\
		} else {					\
			__v = __raw_readb(p);			\
		}						\
								\
		__v;						\
	})							\

#define ioread16(p)						\
	({							\
		unsigned int __v;				\
								\
		if (__is_io_address(p)) {			\
			__v = __raw_readw(alignw(p));		\
		} else {					\
			__v = le16_to_cpu(__raw_readw(p));	\
		}						\
								\
		__v;						\
	})

#define ioread32(p)						\
	({							\
		unsigned int __v;				\
								\
		if (__is_io_address(p)) {			\
			__v = __raw_readl(p);			\
		} else {					\
			__v = le32_to_cpu(__raw_readl(p));	\
		}						\
								\
		 __v;						\
	})

#define iowrite8(v,p)						\
	({							\
		if (__is_io_address(p)) {			\
			__raw_writeb((v), alignb(p));		\
		} else {					\
			__raw_writeb((v), p);			\
		}						\
	})

#define iowrite16(v,p)						\
	({							\
		if (__is_io_address(p)) {			\
			__raw_writew((v), alignw(p));		\
		} else {					\
			__raw_writew(cpu_to_le16(v), p);	\
		}						\
	})

#define iowrite32(v,p)						\
	({							\
		if (__is_io_address(p)) {			\
			__raw_writel((v), p);			\
		} else {					\
			__raw_writel(cpu_to_le32(v), p);	\
		}						\
	})

#define ioport_map(port, nr)	___io(port)

#define ioport_unmap(addr)
#else
#define __io(p)			((void __iomem *)((p)+IXP2000_PCI_IO_VIRT_BASE))
#endif


#endif
