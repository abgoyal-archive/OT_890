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
 * arch/arm/mach-shark/include/mach/memory.h
 *
 * by Alexander Schulz
 *
 * derived from:
 * arch/arm/mach-ebsa110/include/mach/memory.h
 * Copyright (c) 1996-1999 Russell King.
 */
#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

#include <asm/sizes.h>

/*
 * Physical DRAM offset.
 */
#define PHYS_OFFSET     UL(0x08000000)

#ifndef __ASSEMBLY__

static inline void __arch_adjust_zones(int node, unsigned long *zone_size, unsigned long *zhole_size) 
{
  if (node != 0) return;
  /* Only the first 4 MB (=1024 Pages) are usable for DMA */
  zone_size[1] = zone_size[0] - 1024;
  zone_size[0] = 1024;
  zhole_size[1] = zhole_size[0];
  zhole_size[0] = 0;
}

#define arch_adjust_zones(node, size, holes) \
	__arch_adjust_zones(node, size, holes)

#define ISA_DMA_THRESHOLD	(PHYS_OFFSET + SZ_4M - 1)
#define MAX_DMA_ADDRESS		(PAGE_OFFSET + SZ_4M)

#endif

/*
 * Cache flushing area
 */
#define FLUSH_BASE_PHYS		0x80000000
#define FLUSH_BASE		0xdf000000

#endif
