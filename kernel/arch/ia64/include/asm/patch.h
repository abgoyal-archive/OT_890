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

#ifndef _ASM_IA64_PATCH_H
#define _ASM_IA64_PATCH_H

/*
 * Copyright (C) 2003 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 *
 * There are a number of reasons for patching instructions.  Rather than duplicating code
 * all over the place, we put the common stuff here.  Reasons for patching: in-kernel
 * module-loader, virtual-to-physical patch-list, McKinley Errata 9 workaround, and gate
 * shared library.  Undoubtedly, some of these reasons will disappear and others will
 * be added over time.
 */
#include <linux/elf.h>
#include <linux/types.h>

extern void ia64_patch (u64 insn_addr, u64 mask, u64 val);	/* patch any insn slot */
extern void ia64_patch_imm64 (u64 insn_addr, u64 val);		/* patch "movl" w/abs. value*/
extern void ia64_patch_imm60 (u64 insn_addr, u64 val);		/* patch "brl" w/ip-rel value */

extern void ia64_patch_mckinley_e9 (unsigned long start, unsigned long end);
extern void ia64_patch_vtop (unsigned long start, unsigned long end);
extern void ia64_patch_phys_stack_reg(unsigned long val);
extern void ia64_patch_rse (unsigned long start, unsigned long end);
extern void ia64_patch_gate (void);

#endif /* _ASM_IA64_PATCH_H */
