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
 * include/asm-blackfin/cache.h
 */
#ifndef __ARCH_BLACKFIN_CACHE_H
#define __ARCH_BLACKFIN_CACHE_H

/*
 * Bytes per L1 cache line
 * Blackfin loads 32 bytes for cache
 */
#define L1_CACHE_SHIFT	5
#define L1_CACHE_BYTES	(1 << L1_CACHE_SHIFT)
#define SMP_CACHE_BYTES	L1_CACHE_BYTES

#ifdef CONFIG_SMP
#define __cacheline_aligned
#else
#define ____cacheline_aligned

/*
 * Put cacheline_aliged data to L1 data memory
 */
#ifdef CONFIG_CACHELINE_ALIGNED_L1
#define __cacheline_aligned				\
	  __attribute__((__aligned__(L1_CACHE_BYTES),	\
		__section__(".data_l1.cacheline_aligned")))
#endif

#endif

/*
 * largest L1 which this arch supports
 */
#define L1_CACHE_SHIFT_MAX	5

#if defined(CONFIG_SMP) && \
    !defined(CONFIG_BFIN_CACHE_COHERENT) && \
    defined(CONFIG_BFIN_DCACHE)
#define __ARCH_SYNC_CORE_DCACHE
#ifndef __ASSEMBLY__
asmlinkage void __raw_smp_mark_barrier_asm(void);
asmlinkage void __raw_smp_check_barrier_asm(void);

static inline void smp_mark_barrier(void)
{
	__raw_smp_mark_barrier_asm();
}
static inline void smp_check_barrier(void)
{
	__raw_smp_check_barrier_asm();
}

void resync_core_dcache(void);
#endif
#endif


#endif
