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
 * Copyright (C) 2003 Silicon Graphics, Inc. All rights reserved.
 *
 * The generic kernel requires function pointers to these routines, so
 * we wrap the inlines from asm/ia64/sn/sn2/io.h here.
 */

#include <asm/sn/io.h>

#ifdef CONFIG_IA64_GENERIC

#undef __sn_inb
#undef __sn_inw
#undef __sn_inl
#undef __sn_outb
#undef __sn_outw
#undef __sn_outl
#undef __sn_readb
#undef __sn_readw
#undef __sn_readl
#undef __sn_readq
#undef __sn_readb_relaxed
#undef __sn_readw_relaxed
#undef __sn_readl_relaxed
#undef __sn_readq_relaxed

unsigned int __sn_inb(unsigned long port)
{
	return ___sn_inb(port);
}

unsigned int __sn_inw(unsigned long port)
{
	return ___sn_inw(port);
}

unsigned int __sn_inl(unsigned long port)
{
	return ___sn_inl(port);
}

void __sn_outb(unsigned char val, unsigned long port)
{
	___sn_outb(val, port);
}

void __sn_outw(unsigned short val, unsigned long port)
{
	___sn_outw(val, port);
}

void __sn_outl(unsigned int val, unsigned long port)
{
	___sn_outl(val, port);
}

unsigned char __sn_readb(void __iomem *addr)
{
	return ___sn_readb(addr);
}

unsigned short __sn_readw(void __iomem *addr)
{
	return ___sn_readw(addr);
}

unsigned int __sn_readl(void __iomem *addr)
{
	return ___sn_readl(addr);
}

unsigned long __sn_readq(void __iomem *addr)
{
	return ___sn_readq(addr);
}

unsigned char __sn_readb_relaxed(void __iomem *addr)
{
	return ___sn_readb_relaxed(addr);
}

unsigned short __sn_readw_relaxed(void __iomem *addr)
{
	return ___sn_readw_relaxed(addr);
}

unsigned int __sn_readl_relaxed(void __iomem *addr)
{
	return ___sn_readl_relaxed(addr);
}

unsigned long __sn_readq_relaxed(void __iomem *addr)
{
	return ___sn_readq_relaxed(addr);
}

#endif
