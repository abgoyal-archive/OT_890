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
 * include/asm-parisc/cache.h
 */

#ifndef __ARCH_PARISC_CACHE_H
#define __ARCH_PARISC_CACHE_H


/*
 * PA 2.0 processors have 64-byte cachelines; PA 1.1 processors have
 * 32-byte cachelines.  The default configuration is not for SMP anyway,
 * so if you're building for SMP, you should select the appropriate
 * processor type.  There is a potential livelock danger when running
 * a machine with this value set too small, but it's more probable you'll
 * just ruin performance.
 */
#ifdef CONFIG_PA20
#define L1_CACHE_BYTES 64
#define L1_CACHE_SHIFT 6
#else
#define L1_CACHE_BYTES 32
#define L1_CACHE_SHIFT 5
#endif

#ifndef __ASSEMBLY__

#define L1_CACHE_ALIGN(x)       (((x)+(L1_CACHE_BYTES-1))&~(L1_CACHE_BYTES-1))

#define SMP_CACHE_BYTES L1_CACHE_BYTES

#define __read_mostly __attribute__((__section__(".data.read_mostly")))

void parisc_cache_init(void);	/* initializes cache-flushing */
void disable_sr_hashing_asm(int); /* low level support for above */
void disable_sr_hashing(void);   /* turns off space register hashing */
void free_sid(unsigned long);
unsigned long alloc_sid(void);

struct seq_file;
extern void show_cache_info(struct seq_file *m);

extern int split_tlb;
extern int dcache_stride;
extern int icache_stride;
extern struct pdc_cache_info cache_info;
void parisc_setup_cache_timing(void);

#define pdtlb(addr)         asm volatile("pdtlb 0(%%sr1,%0)" : : "r" (addr));
#define pitlb(addr)         asm volatile("pitlb 0(%%sr1,%0)" : : "r" (addr));
#define pdtlb_kernel(addr)  asm volatile("pdtlb 0(%0)" : : "r" (addr));

#endif /* ! __ASSEMBLY__ */

/* Classes of processor wrt: disabling space register hashing */

#define SRHASH_PCXST    0   /* pcxs, pcxt, pcxt_ */
#define SRHASH_PCXL     1   /* pcxl */
#define SRHASH_PA20     2   /* pcxu, pcxu_, pcxw, pcxw_ */

#endif
