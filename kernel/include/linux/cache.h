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

#ifndef __LINUX_CACHE_H
#define __LINUX_CACHE_H

#include <linux/kernel.h>
#include <asm/cache.h>

#ifndef L1_CACHE_ALIGN
#define L1_CACHE_ALIGN(x) ALIGN(x, L1_CACHE_BYTES)
#endif

#ifndef SMP_CACHE_BYTES
#define SMP_CACHE_BYTES L1_CACHE_BYTES
#endif

#ifndef __read_mostly
#define __read_mostly
#endif

#ifndef ____cacheline_aligned
#define ____cacheline_aligned __attribute__((__aligned__(SMP_CACHE_BYTES)))
#endif

#ifndef ____cacheline_aligned_in_smp
#ifdef CONFIG_SMP
#define ____cacheline_aligned_in_smp ____cacheline_aligned
#else
#define ____cacheline_aligned_in_smp
#endif /* CONFIG_SMP */
#endif

#ifndef __cacheline_aligned
#define __cacheline_aligned					\
  __attribute__((__aligned__(SMP_CACHE_BYTES),			\
		 __section__(".data.cacheline_aligned")))
#endif /* __cacheline_aligned */

#ifndef __cacheline_aligned_in_smp
#ifdef CONFIG_SMP
#define __cacheline_aligned_in_smp __cacheline_aligned
#else
#define __cacheline_aligned_in_smp
#endif /* CONFIG_SMP */
#endif

/*
 * The maximum alignment needed for some critical structures
 * These could be inter-node cacheline sizes/L3 cacheline
 * size etc.  Define this in asm/cache.h for your arch
 */
#ifndef INTERNODE_CACHE_SHIFT
#define INTERNODE_CACHE_SHIFT L1_CACHE_SHIFT
#endif

#if !defined(____cacheline_internodealigned_in_smp)
#if defined(CONFIG_SMP)
#define ____cacheline_internodealigned_in_smp \
	__attribute__((__aligned__(1 << (INTERNODE_CACHE_SHIFT))))
#else
#define ____cacheline_internodealigned_in_smp
#endif
#endif

#ifndef CONFIG_ARCH_HAS_CACHE_LINE_SIZE
#define cache_line_size()	L1_CACHE_BYTES
#endif

#endif /* __LINUX_CACHE_H */
