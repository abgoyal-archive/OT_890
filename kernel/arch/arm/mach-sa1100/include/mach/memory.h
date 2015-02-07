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
 * arch/arm/mach-sa1100/include/mach/memory.h
 *
 * Copyright (C) 1999-2000 Nicolas Pitre <nico@cam.org>
 */

#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

#include <asm/sizes.h>

/*
 * Physical DRAM offset is 0xc0000000 on the SA1100
 */
#define PHYS_OFFSET	UL(0xc0000000)

#ifndef __ASSEMBLY__

#ifdef CONFIG_SA1111
void sa1111_adjust_zones(int node, unsigned long *size, unsigned long *holes);

#define arch_adjust_zones(node, size, holes) \
	sa1111_adjust_zones(node, size, holes)

#define ISA_DMA_THRESHOLD	(PHYS_OFFSET + SZ_1M - 1)
#define MAX_DMA_ADDRESS		(PAGE_OFFSET + SZ_1M)

#endif
#endif

/*
 * Because of the wide memory address space between physical RAM banks on the
 * SA1100, it's much convenient to use Linux's SparseMEM support to implement
 * our memory map representation.  Assuming all memory nodes have equal access
 * characteristics, we then have generic discontiguous memory support.
 *
 * The sparsemem banks are matched with the physical memory bank addresses
 * which are incidentally the same as virtual addresses.
 * 
 * 	node 0:  0xc0000000 - 0xc7ffffff
 * 	node 1:  0xc8000000 - 0xcfffffff
 * 	node 2:  0xd0000000 - 0xd7ffffff
 * 	node 3:  0xd8000000 - 0xdfffffff
 */
#define MAX_PHYSMEM_BITS	32
#define SECTION_SIZE_BITS	27

/*
 * Cache flushing area - SA1100 zero bank
 */
#define FLUSH_BASE_PHYS		0xe0000000
#define FLUSH_BASE		0xf5000000
#define FLUSH_BASE_MINICACHE	0xf5100000

#endif
