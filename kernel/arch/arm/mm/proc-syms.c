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
 *  linux/arch/arm/mm/proc-syms.c
 *
 *  Copyright (C) 2000-2002 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/mm.h>

#include <asm/cacheflush.h>
#include <asm/proc-fns.h>
#include <asm/tlbflush.h>
#include <asm/page.h>

#ifndef MULTI_CPU
EXPORT_SYMBOL(cpu_dcache_clean_area);
EXPORT_SYMBOL(cpu_set_pte_ext);
#else
EXPORT_SYMBOL(processor);
#endif

#ifndef MULTI_CACHE
EXPORT_SYMBOL(__cpuc_flush_kern_all);
EXPORT_SYMBOL(__cpuc_flush_user_all);
EXPORT_SYMBOL(__cpuc_flush_user_range);
EXPORT_SYMBOL(__cpuc_coherent_kern_range);
EXPORT_SYMBOL(dmac_inv_range);  /* because of flush_ioremap_region() */
#else
EXPORT_SYMBOL(cpu_cache);
#endif

#ifdef CONFIG_MMU
#ifndef MULTI_USER
EXPORT_SYMBOL(__cpu_clear_user_highpage);
EXPORT_SYMBOL(__cpu_copy_user_highpage);
#else
EXPORT_SYMBOL(cpu_user);
#endif
#endif

/*
 * No module should need to touch the TLB (and currently
 * no modules do.  We export this for "loadkernel" support
 * (booting a new kernel from within a running kernel.)
 */
#ifdef MULTI_TLB
EXPORT_SYMBOL(cpu_tlb);
#endif
