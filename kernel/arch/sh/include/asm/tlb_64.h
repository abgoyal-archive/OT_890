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
 * include/asm-sh/tlb_64.h
 *
 * Copyright (C) 2003  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef __ASM_SH_TLB_64_H
#define __ASM_SH_TLB_64_H

/* ITLB defines */
#define ITLB_FIXED	0x00000000	/* First fixed ITLB, see head.S */
#define ITLB_LAST_VAR_UNRESTRICTED	0x000003F0	/* Last ITLB */

/* DTLB defines */
#define DTLB_FIXED	0x00800000	/* First fixed DTLB, see head.S */
#define DTLB_LAST_VAR_UNRESTRICTED	0x008003F0	/* Last DTLB */

#ifndef __ASSEMBLY__

/**
 * for_each_dtlb_entry - Iterate over free (non-wired) DTLB entries
 *
 * @tlb:	TLB entry
 */
#define for_each_dtlb_entry(tlb)		\
	for (tlb  = cpu_data->dtlb.first;	\
	     tlb <= cpu_data->dtlb.last;	\
	     tlb += cpu_data->dtlb.step)

/**
 * for_each_itlb_entry - Iterate over free (non-wired) ITLB entries
 *
 * @tlb:	TLB entry
 */
#define for_each_itlb_entry(tlb)		\
	for (tlb  = cpu_data->itlb.first;	\
	     tlb <= cpu_data->itlb.last;	\
	     tlb += cpu_data->itlb.step)

/**
 * __flush_tlb_slot - Flushes TLB slot @slot.
 *
 * @slot:	Address of TLB slot.
 */
static inline void __flush_tlb_slot(unsigned long long slot)
{
	__asm__ __volatile__ ("putcfg %0, 0, r63\n" : : "r" (slot));
}

#ifdef CONFIG_MMU
/* arch/sh64/mm/tlb.c */
int sh64_tlb_init(void);
unsigned long long sh64_next_free_dtlb_entry(void);
unsigned long long sh64_get_wired_dtlb_entry(void);
int sh64_put_wired_dtlb_entry(unsigned long long entry);
void sh64_setup_tlb_slot(unsigned long long config_addr, unsigned long eaddr,
			 unsigned long asid, unsigned long paddr);
void sh64_teardown_tlb_slot(unsigned long long config_addr);
#else
#define sh64_tlb_init()					do { } while (0)
#define sh64_next_free_dtlb_entry()			(0)
#define sh64_get_wired_dtlb_entry()			(0)
#define sh64_put_wired_dtlb_entry(entry)		do { } while (0)
#define sh64_setup_tlb_slot(conf, virt, asid, phys)	do { } while (0)
#define sh64_teardown_tlb_slot(addr)			do { } while (0)
#endif /* CONFIG_MMU */
#endif /* __ASSEMBLY__ */
#endif /* __ASM_SH_TLB_64_H */
