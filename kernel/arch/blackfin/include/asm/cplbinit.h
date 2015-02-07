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
 * File:         include/asm-blackfin/cplbinit.h
 * Based on:
 * Author:
 *
 * Created:
 * Description:
 *
 * Modified:
 *               Copyright 2004-2006 Analog Devices Inc.
 *
 * Bugs:         Enter bugs at http://blackfin.uclinux.org/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __ASM_CPLBINIT_H__
#define __ASM_CPLBINIT_H__

#include <asm/blackfin.h>
#include <asm/cplb.h>
#include <linux/threads.h>

#ifdef CONFIG_CPLB_SWITCH_TAB_L1
# define PDT_ATTR __attribute__((l1_data))
#else
# define PDT_ATTR
#endif

struct cplb_entry {
	unsigned long data, addr;
};

struct cplb_boundary {
	unsigned long eaddr; /* End of this region.  */
	unsigned long data; /* CPLB data value.  */
};

extern struct cplb_boundary dcplb_bounds[];
extern struct cplb_boundary icplb_bounds[];
extern int dcplb_nr_bounds, icplb_nr_bounds;

extern struct cplb_entry dcplb_tbl[NR_CPUS][MAX_CPLBS];
extern struct cplb_entry icplb_tbl[NR_CPUS][MAX_CPLBS];
extern int first_switched_icplb;
extern int first_switched_dcplb;

extern int nr_dcplb_miss[], nr_icplb_miss[], nr_icplb_supv_miss[];
extern int nr_dcplb_prot[], nr_cplb_flush[];

#ifdef CONFIG_MPU

extern int first_mask_dcplb;

extern int page_mask_order;
extern int page_mask_nelts;

extern unsigned long *current_rwx_mask[NR_CPUS];

extern void flush_switched_cplbs(unsigned int);
extern void set_mask_dcplbs(unsigned long *, unsigned int);

extern void __noreturn panic_cplb_error(int seqstat, struct pt_regs *);

#endif /* CONFIG_MPU */

extern void bfin_icache_init(struct cplb_entry *icplb_tbl);
extern void bfin_dcache_init(struct cplb_entry *icplb_tbl);

#if defined(CONFIG_BFIN_DCACHE) || defined(CONFIG_BFIN_ICACHE)
extern void generate_cplb_tables_all(void);
extern void generate_cplb_tables_cpu(unsigned int cpu);
#endif
#endif
