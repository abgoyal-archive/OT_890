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

/* arch/arm/mach-lh7a40x/include/mach/memory.h
 *
 *  Copyright (C) 2004 Coastal Environmental Systems
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.
 *
 *
 *  Refer to <file:Documentation/arm/Sharp-LH/SDRAM> for more information.
 *
 */

#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

/*
 * Physical DRAM offset.
 */
#define PHYS_OFFSET	UL(0xc0000000)

#ifdef CONFIG_DISCONTIGMEM

/*
 * Given a kernel address, find the home node of the underlying memory.
 */

# ifdef CONFIG_LH7A40X_ONE_BANK_PER_NODE
#  define KVADDR_TO_NID(addr) \
  (  ((((unsigned long) (addr) - PAGE_OFFSET) >> 24) &  1)\
   | ((((unsigned long) (addr) - PAGE_OFFSET) >> 25) & ~1))
# else  /* 2 banks per node */
#  define KVADDR_TO_NID(addr) \
      (((unsigned long) (addr) - PAGE_OFFSET) >> 26)
# endif

/*
 * Given a page frame number, convert it to a node id.
 */

# ifdef CONFIG_LH7A40X_ONE_BANK_PER_NODE
#  define PFN_TO_NID(pfn) \
  (((((pfn) - PHYS_PFN_OFFSET) >> (24 - PAGE_SHIFT)) &  1)\
 | ((((pfn) - PHYS_PFN_OFFSET) >> (25 - PAGE_SHIFT)) & ~1))
# else  /* 2 banks per node */
#  define PFN_TO_NID(pfn) \
    (((pfn) - PHYS_PFN_OFFSET) >> (26 - PAGE_SHIFT))
#endif

/*
 * Given a kaddr, LOCAL_MEM_MAP finds the owning node of the memory
 * and returns the index corresponding to the appropriate page in the
 * node's mem_map.
 */

# ifdef CONFIG_LH7A40X_ONE_BANK_PER_NODE
#  define LOCAL_MAP_NR(addr) \
       (((unsigned long)(addr) & 0x003fffff) >> PAGE_SHIFT)
# else  /* 2 banks per node */
#  define LOCAL_MAP_NR(addr) \
       (((unsigned long)(addr) & 0x01ffffff) >> PAGE_SHIFT)
# endif

#endif

/*
 * Sparsemem version of the above
 */
#define MAX_PHYSMEM_BITS	32
#define SECTION_SIZE_BITS	24

#endif
