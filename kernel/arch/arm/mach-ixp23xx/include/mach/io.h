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
 * arch/arm/mach-ixp23xx/include/mach/io.h
 *
 * Original Author: Naeem M Afzal <naeem.m.afzal@intel.com>
 * Maintainer: Deepak Saxena <dsaxena@plexity.net>
 *
 * Copyright (C) 2003-2005 Intel Corp.
 * Copyright (C) 2005 MontaVista Software, Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_ARCH_IO_H
#define __ASM_ARCH_IO_H

#define IO_SPACE_LIMIT 0xffffffff

#define __io(p)		((void __iomem*)((p) + IXP23XX_PCI_IO_VIRT))
#define __mem_pci(a)	(a)

static inline void __iomem *
ixp23xx_ioremap(unsigned long addr, unsigned long size, unsigned int mtype)
{
	if (addr >= IXP23XX_PCI_MEM_START &&
		addr <= IXP23XX_PCI_MEM_START + IXP23XX_PCI_MEM_SIZE) {
		if (addr + size > IXP23XX_PCI_MEM_START + IXP23XX_PCI_MEM_SIZE)
			return NULL;

		return (void __iomem *)
 			((addr - IXP23XX_PCI_MEM_START) + IXP23XX_PCI_MEM_VIRT);
	}

	return __arm_ioremap(addr, size, mtype);
}

static inline void
ixp23xx_iounmap(void __iomem *addr)
{
	if ((((u32)addr) >= IXP23XX_PCI_MEM_VIRT) &&
	    (((u32)addr) < IXP23XX_PCI_MEM_VIRT + IXP23XX_PCI_MEM_SIZE))
		return;

	__iounmap(addr);
}

#define __arch_ioremap(a,s,f)	ixp23xx_ioremap(a,s,f)
#define __arch_iounmap(a)	ixp23xx_iounmap(a)


#endif
