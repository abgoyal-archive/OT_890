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
 * DaVinci memory space definitions
 *
 * Author: Kevin Hilman, MontaVista Software, Inc. <source@mvista.com>
 *
 * 2007 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */
#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

/**************************************************************************
 * Included Files
 **************************************************************************/
#include <asm/page.h>
#include <asm/sizes.h>

/**************************************************************************
 * Definitions
 **************************************************************************/
#define DAVINCI_DDR_BASE    0x80000000
#define DAVINCI_IRAM_BASE   0x00008000 /* ARM Internal RAM */

#define PHYS_OFFSET DAVINCI_DDR_BASE

/*
 * Increase size of DMA-consistent memory region
 */
#define CONSISTENT_DMA_SIZE (14<<20)

#ifndef __ASSEMBLY__
/*
 * Restrict DMA-able region to workaround silicon bug.  The bug
 * restricts buffers available for DMA to video hardware to be
 * below 128M
 */
static inline void
__arch_adjust_zones(int node, unsigned long *size, unsigned long *holes)
{
	unsigned int sz = (128<<20) >> PAGE_SHIFT;

	if (node != 0)
		sz = 0;

	size[1] = size[0] - sz;
	size[0] = sz;
}

#define arch_adjust_zones(node, zone_size, holes) \
        if ((meminfo.bank[0].size >> 20) > 128) __arch_adjust_zones(node, zone_size, holes)

#define ISA_DMA_THRESHOLD	(PHYS_OFFSET + (128<<20) - 1)
#define MAX_DMA_ADDRESS		(PAGE_OFFSET + (128<<20))

#endif

#endif /* __ASM_ARCH_MEMORY_H */
