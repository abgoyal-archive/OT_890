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

/* This file is meant to be include multiple times by other headers */
/* last 2 argments are used by platforms/cell/io-workarounds.[ch] */

DEF_PCI_AC_RET(readb, u8, (const PCI_IO_ADDR addr), (addr), mem, addr)
DEF_PCI_AC_RET(readw, u16, (const PCI_IO_ADDR addr), (addr), mem, addr)
DEF_PCI_AC_RET(readl, u32, (const PCI_IO_ADDR addr), (addr), mem, addr)
DEF_PCI_AC_RET(readw_be, u16, (const PCI_IO_ADDR addr), (addr), mem, addr)
DEF_PCI_AC_RET(readl_be, u32, (const PCI_IO_ADDR addr), (addr), mem, addr)
DEF_PCI_AC_NORET(writeb, (u8 val, PCI_IO_ADDR addr), (val, addr), mem, addr)
DEF_PCI_AC_NORET(writew, (u16 val, PCI_IO_ADDR addr), (val, addr), mem, addr)
DEF_PCI_AC_NORET(writel, (u32 val, PCI_IO_ADDR addr), (val, addr), mem, addr)
DEF_PCI_AC_NORET(writew_be, (u16 val, PCI_IO_ADDR addr), (val, addr), mem, addr)
DEF_PCI_AC_NORET(writel_be, (u32 val, PCI_IO_ADDR addr), (val, addr), mem, addr)

#ifdef __powerpc64__
DEF_PCI_AC_RET(readq, u64, (const PCI_IO_ADDR addr), (addr), mem, addr)
DEF_PCI_AC_RET(readq_be, u64, (const PCI_IO_ADDR addr), (addr), mem, addr)
DEF_PCI_AC_NORET(writeq, (u64 val, PCI_IO_ADDR addr), (val, addr), mem, addr)
DEF_PCI_AC_NORET(writeq_be, (u64 val, PCI_IO_ADDR addr), (val, addr), mem, addr)
#endif /* __powerpc64__ */

DEF_PCI_AC_RET(inb, u8, (unsigned long port), (port), pio, port)
DEF_PCI_AC_RET(inw, u16, (unsigned long port), (port), pio, port)
DEF_PCI_AC_RET(inl, u32, (unsigned long port), (port), pio, port)
DEF_PCI_AC_NORET(outb, (u8 val, unsigned long port), (val, port), pio, port)
DEF_PCI_AC_NORET(outw, (u16 val, unsigned long port), (val, port), pio, port)
DEF_PCI_AC_NORET(outl, (u32 val, unsigned long port), (val, port), pio, port)

DEF_PCI_AC_NORET(readsb, (const PCI_IO_ADDR a, void *b, unsigned long c),
		 (a, b, c), mem, a)
DEF_PCI_AC_NORET(readsw, (const PCI_IO_ADDR a, void *b, unsigned long c),
		 (a, b, c), mem, a)
DEF_PCI_AC_NORET(readsl, (const PCI_IO_ADDR a, void *b, unsigned long c),
		 (a, b, c), mem, a)
DEF_PCI_AC_NORET(writesb, (PCI_IO_ADDR a, const void *b, unsigned long c),
		 (a, b, c), mem, a)
DEF_PCI_AC_NORET(writesw, (PCI_IO_ADDR a, const void *b, unsigned long c),
		 (a, b, c), mem, a)
DEF_PCI_AC_NORET(writesl, (PCI_IO_ADDR a, const void *b, unsigned long c),
		 (a, b, c), mem, a)

DEF_PCI_AC_NORET(insb, (unsigned long p, void *b, unsigned long c),
		 (p, b, c), pio, p)
DEF_PCI_AC_NORET(insw, (unsigned long p, void *b, unsigned long c),
		 (p, b, c), pio, p)
DEF_PCI_AC_NORET(insl, (unsigned long p, void *b, unsigned long c),
		 (p, b, c), pio, p)
DEF_PCI_AC_NORET(outsb, (unsigned long p, const void *b, unsigned long c),
		 (p, b, c), pio, p)
DEF_PCI_AC_NORET(outsw, (unsigned long p, const void *b, unsigned long c),
		 (p, b, c), pio, p)
DEF_PCI_AC_NORET(outsl, (unsigned long p, const void *b, unsigned long c),
		 (p, b, c), pio, p)

DEF_PCI_AC_NORET(memset_io, (PCI_IO_ADDR a, int c, unsigned long n),
		 (a, c, n), mem, a)
DEF_PCI_AC_NORET(memcpy_fromio, (void *d, const PCI_IO_ADDR s, unsigned long n),
		 (d, s, n), mem, s)
DEF_PCI_AC_NORET(memcpy_toio, (PCI_IO_ADDR d, const void *s, unsigned long n),
		 (d, s, n), mem, d)
