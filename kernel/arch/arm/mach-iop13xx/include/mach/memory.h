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

#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

#include <mach/hardware.h>

/*
 * Physical DRAM offset.
 */
#define PHYS_OFFSET	UL(0x00000000)

#ifndef __ASSEMBLY__

#if defined(CONFIG_ARCH_IOP13XX)
#define IOP13XX_PMMR_V_START (IOP13XX_PMMR_VIRT_MEM_BASE)
#define IOP13XX_PMMR_V_END   (IOP13XX_PMMR_VIRT_MEM_BASE + IOP13XX_PMMR_SIZE)
#define IOP13XX_PMMR_P_START (IOP13XX_PMMR_PHYS_MEM_BASE)
#define IOP13XX_PMMR_P_END   (IOP13XX_PMMR_PHYS_MEM_BASE + IOP13XX_PMMR_SIZE)

static inline dma_addr_t __virt_to_lbus(unsigned long x)
{
	return x + IOP13XX_PMMR_PHYS_MEM_BASE - IOP13XX_PMMR_VIRT_MEM_BASE;
}

static inline unsigned long __lbus_to_virt(dma_addr_t x)
{
	return x + IOP13XX_PMMR_VIRT_MEM_BASE - IOP13XX_PMMR_PHYS_MEM_BASE;
}

#define __is_lbus_dma(a)				\
	((a) >= IOP13XX_PMMR_P_START && (a) < IOP13XX_PMMR_P_END)

#define __is_lbus_virt(a)				\
	((a) >= IOP13XX_PMMR_V_START && (a) < IOP13XX_PMMR_V_END)

/* Device is an lbus device if it is on the platform bus of the IOP13XX */
#define is_lbus_device(dev) 				\
	(dev && strncmp(dev->bus->name, "platform", 8) == 0)

#define __arch_dma_to_virt(dev, addr)					\
	({								\
		unsigned long __virt;					\
		dma_addr_t __dma = addr;				\
		if (is_lbus_device(dev) && __is_lbus_dma(__dma))	\
			__virt = __lbus_to_virt(__dma);			\
		else							\
			__virt = __phys_to_virt(__dma);			\
		(void *)__virt;						\
	})

#define __arch_virt_to_dma(dev, addr)					\
	({								\
		unsigned long __virt = (unsigned long)addr;		\
		dma_addr_t __dma;					\
		if (is_lbus_device(dev) && __is_lbus_virt(__virt))	\
			__dma = __virt_to_lbus(__virt);			\
		else							\
			__dma = __virt_to_phys(__virt);			\
		__dma;							\
	})

#define __arch_page_to_dma(dev, page)					\
	__arch_virt_to_dma(dev, page_address(page))

#endif /* CONFIG_ARCH_IOP13XX */
#endif /* !ASSEMBLY */

#define PFN_TO_NID(addr)	(0)

#endif
