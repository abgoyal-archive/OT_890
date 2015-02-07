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

#ifndef __GENERIC_IO_H
#define __GENERIC_IO_H

#include <linux/linkage.h>
#include <asm/byteorder.h>

/*
 * These are the "generic" interfaces for doing new-style
 * memory-mapped or PIO accesses. Architectures may do
 * their own arch-optimized versions, these just act as
 * wrappers around the old-style IO register access functions:
 * read[bwl]/write[bwl]/in[bwl]/out[bwl]
 *
 * Don't include this directly, include it from <asm/io.h>.
 */

/*
 * Read/write from/to an (offsettable) iomem cookie. It might be a PIO
 * access or a MMIO access, these functions don't care. The info is
 * encoded in the hardware mapping set up by the mapping functions
 * (or the cookie itself, depending on implementation and hw).
 *
 * The generic routines just encode the PIO/MMIO as part of the
 * cookie, and coldly assume that the MMIO IO mappings are not
 * in the low address range. Architectures for which this is not
 * true can't use this generic implementation.
 */
extern unsigned int ioread8(void __iomem *);
extern unsigned int ioread16(void __iomem *);
extern unsigned int ioread16be(void __iomem *);
extern unsigned int ioread32(void __iomem *);
extern unsigned int ioread32be(void __iomem *);

extern void iowrite8(u8, void __iomem *);
extern void iowrite16(u16, void __iomem *);
extern void iowrite16be(u16, void __iomem *);
extern void iowrite32(u32, void __iomem *);
extern void iowrite32be(u32, void __iomem *);

/*
 * "string" versions of the above. Note that they
 * use native byte ordering for the accesses (on
 * the assumption that IO and memory agree on a
 * byte order, and CPU byteorder is irrelevant).
 *
 * They do _not_ update the port address. If you
 * want MMIO that copies stuff laid out in MMIO
 * memory across multiple ports, use "memcpy_toio()"
 * and friends.
 */
extern void ioread8_rep(void __iomem *port, void *buf, unsigned long count);
extern void ioread16_rep(void __iomem *port, void *buf, unsigned long count);
extern void ioread32_rep(void __iomem *port, void *buf, unsigned long count);

extern void iowrite8_rep(void __iomem *port, const void *buf, unsigned long count);
extern void iowrite16_rep(void __iomem *port, const void *buf, unsigned long count);
extern void iowrite32_rep(void __iomem *port, const void *buf, unsigned long count);

/* Create a virtual mapping cookie for an IO port range */
extern void __iomem *ioport_map(unsigned long port, unsigned int nr);
extern void ioport_unmap(void __iomem *);

#ifndef ARCH_HAS_IOREMAP_WC
#define ioremap_wc ioremap_nocache
#endif

/* Create a virtual mapping cookie for a PCI BAR (memory or IO) */
struct pci_dev;
extern void __iomem *pci_iomap(struct pci_dev *dev, int bar, unsigned long max);
extern void pci_iounmap(struct pci_dev *dev, void __iomem *);

#endif
