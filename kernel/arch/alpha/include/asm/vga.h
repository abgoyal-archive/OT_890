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
 *	Access to VGA videoram
 *
 *	(c) 1998 Martin Mares <mj@ucw.cz>
 */

#ifndef _LINUX_ASM_VGA_H_
#define _LINUX_ASM_VGA_H_

#include <asm/io.h>

#define VT_BUF_HAVE_RW
#define VT_BUF_HAVE_MEMSETW
#define VT_BUF_HAVE_MEMCPYW

static inline void scr_writew(u16 val, volatile u16 *addr)
{
	if (__is_ioaddr(addr))
		__raw_writew(val, (volatile u16 __iomem *) addr);
	else
		*addr = val;
}

static inline u16 scr_readw(volatile const u16 *addr)
{
	if (__is_ioaddr(addr))
		return __raw_readw((volatile const u16 __iomem *) addr);
	else
		return *addr;
}

static inline void scr_memsetw(u16 *s, u16 c, unsigned int count)
{
	if (__is_ioaddr(s))
		memsetw_io((u16 __iomem *) s, c, count);
	else
		memsetw(s, c, count);
}

/* Do not trust that the usage will be correct; analyze the arguments.  */
extern void scr_memcpyw(u16 *d, const u16 *s, unsigned int count);

/* ??? These are currently only used for downloading character sets.  As
   such, they don't need memory barriers.  Is this all they are intended
   to be used for?  */
#define vga_readb(a)	readb((u8 __iomem *)(a))
#define vga_writeb(v,a)	writeb(v, (u8 __iomem *)(a))

#ifdef CONFIG_VGA_HOSE
#include <linux/ioport.h>
#include <linux/pci.h>

extern struct pci_controller *pci_vga_hose;

# define __is_port_vga(a)       \
	(((a) >= 0x3b0) && ((a) < 0x3e0) && \
	 ((a) != 0x3b3) && ((a) != 0x3d3))

# define __is_mem_vga(a) \
	(((a) >= 0xa0000) && ((a) <= 0xc0000))

# define FIXUP_IOADDR_VGA(a) do {                       \
	if (pci_vga_hose && __is_port_vga(a))     \
		(a) += pci_vga_hose->io_space->start;	  \
 } while(0)

# define FIXUP_MEMADDR_VGA(a) do {                       \
	if (pci_vga_hose && __is_mem_vga(a))     \
		(a) += pci_vga_hose->mem_space->start; \
 } while(0)

#else /* CONFIG_VGA_HOSE */
# define pci_vga_hose 0
# define __is_port_vga(a) 0
# define __is_mem_vga(a) 0
# define FIXUP_IOADDR_VGA(a)
# define FIXUP_MEMADDR_VGA(a)
#endif /* CONFIG_VGA_HOSE */

#define VGA_MAP_MEM(x,s)	((unsigned long) ioremap(x, s))

#endif
