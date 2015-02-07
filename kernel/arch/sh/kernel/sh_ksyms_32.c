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

#include <linux/module.h>
#include <linux/smp.h>
#include <linux/user.h>
#include <linux/elfcore.h>
#include <linux/sched.h>
#include <linux/in6.h>
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/pci.h>
#include <linux/irq.h>
#include <asm/sections.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
#include <asm/checksum.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <asm/tlbflush.h>
#include <asm/cacheflush.h>
#include <asm/ftrace.h>

extern int dump_fpu(struct pt_regs *, elf_fpregset_t *);
extern struct hw_interrupt_type no_irq_type;

/* platform dependent support */
EXPORT_SYMBOL(dump_fpu);
EXPORT_SYMBOL(kernel_thread);
EXPORT_SYMBOL(irq_desc);
EXPORT_SYMBOL(no_irq_type);

EXPORT_SYMBOL(strlen);

/* PCI exports */
#ifdef CONFIG_PCI
EXPORT_SYMBOL(pci_alloc_consistent);
EXPORT_SYMBOL(pci_free_consistent);
#endif

/* mem exports */
EXPORT_SYMBOL(memchr);
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memset);
EXPORT_SYMBOL(memmove);
EXPORT_SYMBOL(__copy_user);

#ifdef CONFIG_MMU
EXPORT_SYMBOL(get_vm_area);
#endif

EXPORT_SYMBOL(__udelay);
EXPORT_SYMBOL(__ndelay);
EXPORT_SYMBOL(__const_udelay);

#define DECLARE_EXPORT(name)		\
	extern void name(void);EXPORT_SYMBOL(name)

DECLARE_EXPORT(__udivsi3);
DECLARE_EXPORT(__sdivsi3);
DECLARE_EXPORT(__lshrsi3);
DECLARE_EXPORT(__ashrsi3);
DECLARE_EXPORT(__ashlsi3);
DECLARE_EXPORT(__ashiftrt_r4_6);
DECLARE_EXPORT(__ashiftrt_r4_7);
DECLARE_EXPORT(__ashiftrt_r4_8);
DECLARE_EXPORT(__ashiftrt_r4_9);
DECLARE_EXPORT(__ashiftrt_r4_10);
DECLARE_EXPORT(__ashiftrt_r4_11);
DECLARE_EXPORT(__ashiftrt_r4_12);
DECLARE_EXPORT(__ashiftrt_r4_13);
DECLARE_EXPORT(__ashiftrt_r4_14);
DECLARE_EXPORT(__ashiftrt_r4_15);
DECLARE_EXPORT(__ashiftrt_r4_20);
DECLARE_EXPORT(__ashiftrt_r4_21);
DECLARE_EXPORT(__ashiftrt_r4_22);
DECLARE_EXPORT(__ashiftrt_r4_23);
DECLARE_EXPORT(__ashiftrt_r4_24);
DECLARE_EXPORT(__ashiftrt_r4_27);
DECLARE_EXPORT(__ashiftrt_r4_30);
DECLARE_EXPORT(__movstr);
DECLARE_EXPORT(__movstrSI8);
DECLARE_EXPORT(__movstrSI12);
DECLARE_EXPORT(__movstrSI16);
DECLARE_EXPORT(__movstrSI20);
DECLARE_EXPORT(__movstrSI24);
DECLARE_EXPORT(__movstrSI28);
DECLARE_EXPORT(__movstrSI32);
DECLARE_EXPORT(__movstrSI36);
DECLARE_EXPORT(__movstrSI40);
DECLARE_EXPORT(__movstrSI44);
DECLARE_EXPORT(__movstrSI48);
DECLARE_EXPORT(__movstrSI52);
DECLARE_EXPORT(__movstrSI56);
DECLARE_EXPORT(__movstrSI60);
DECLARE_EXPORT(__movstr_i4_even);
DECLARE_EXPORT(__movstr_i4_odd);
DECLARE_EXPORT(__movstrSI12_i4);
DECLARE_EXPORT(__movmem_i4_even);
DECLARE_EXPORT(__movmem_i4_odd);
DECLARE_EXPORT(__movmemSI12_i4);
DECLARE_EXPORT(__udiv_qrnnd_16);
DECLARE_EXPORT(__sdivsi3_i4);
DECLARE_EXPORT(__udivsi3_i4);
DECLARE_EXPORT(__sdivsi3_i4i);
DECLARE_EXPORT(__udivsi3_i4i);

#if !defined(CONFIG_CACHE_OFF) && (defined(CONFIG_CPU_SH4) || \
	defined(CONFIG_SH7705_CACHE_32KB))
/* needed by some modules */
EXPORT_SYMBOL(flush_cache_all);
EXPORT_SYMBOL(flush_cache_range);
EXPORT_SYMBOL(flush_dcache_page);
#endif

#if !defined(CONFIG_CACHE_OFF) && defined(CONFIG_MMU) && \
	(defined(CONFIG_CPU_SH4) || defined(CONFIG_SH7705_CACHE_32KB))
EXPORT_SYMBOL(clear_user_page);
#endif

#ifdef CONFIG_FUNCTION_TRACER
EXPORT_SYMBOL(mcount);
#endif
EXPORT_SYMBOL(csum_partial);
EXPORT_SYMBOL(csum_partial_copy_generic);
#ifdef CONFIG_IPV6
EXPORT_SYMBOL(csum_ipv6_magic);
#endif
EXPORT_SYMBOL(clear_page);
EXPORT_SYMBOL(copy_page);
EXPORT_SYMBOL(__clear_user);
EXPORT_SYMBOL(_ebss);
EXPORT_SYMBOL(empty_zero_page);

#ifndef CONFIG_CACHE_OFF
EXPORT_SYMBOL(__flush_purge_region);
EXPORT_SYMBOL(__flush_wback_region);
EXPORT_SYMBOL(__flush_invalidate_region);
#endif
