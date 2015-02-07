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

#ifndef _ASM_M68K_ZORRO_H
#define _ASM_M68K_ZORRO_H

#include <asm/raw_io.h>

#define z_readb raw_inb
#define z_readw raw_inw
#define z_readl raw_inl

#define z_writeb raw_outb
#define z_writew raw_outw
#define z_writel raw_outl

#define z_memset_io(a,b,c)	memset((void *)(a),(b),(c))
#define z_memcpy_fromio(a,b,c)	memcpy((a),(void *)(b),(c))
#define z_memcpy_toio(a,b,c)	memcpy((void *)(a),(b),(c))

static inline void __iomem *z_remap_nocache_ser(unsigned long physaddr,
					unsigned long size)
{
	return __ioremap(physaddr, size, IOMAP_NOCACHE_SER);
}

static inline void __iomem *z_remap_nocache_nonser(unsigned long physaddr,
					   unsigned long size)
{
	return __ioremap(physaddr, size, IOMAP_NOCACHE_NONSER);
}

static inline void __iomem *z_remap_writethrough(unsigned long physaddr,
					 unsigned long size)
{
	return __ioremap(physaddr, size, IOMAP_WRITETHROUGH);
}
static inline void __iomem *z_remap_fullcache(unsigned long physaddr,
				      unsigned long size)
{
	return __ioremap(physaddr, size, IOMAP_FULL_CACHING);
}

#define z_unmap iounmap
#define z_iounmap iounmap
#define z_ioremap z_remap_nocache_ser

#endif /* _ASM_M68K_ZORRO_H */
