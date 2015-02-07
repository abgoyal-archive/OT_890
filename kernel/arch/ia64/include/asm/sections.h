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

#ifndef _ASM_IA64_SECTIONS_H
#define _ASM_IA64_SECTIONS_H

/*
 * Copyright (C) 1998-2003 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 */

#include <linux/elf.h>
#include <linux/uaccess.h>
#include <asm-generic/sections.h>

extern char __per_cpu_start[], __per_cpu_end[], __phys_per_cpu_start[];
#ifdef	CONFIG_SMP
extern char __cpu0_per_cpu[];
#endif
extern char __start___vtop_patchlist[], __end___vtop_patchlist[];
extern char __start___rse_patchlist[], __end___rse_patchlist[];
extern char __start___mckinley_e9_bundles[], __end___mckinley_e9_bundles[];
extern char __start___phys_stack_reg_patchlist[], __end___phys_stack_reg_patchlist[];
extern char __start_gate_section[];
extern char __start_gate_mckinley_e9_patchlist[], __end_gate_mckinley_e9_patchlist[];
extern char __start_gate_vtop_patchlist[], __end_gate_vtop_patchlist[];
extern char __start_gate_fsyscall_patchlist[], __end_gate_fsyscall_patchlist[];
extern char __start_gate_brl_fsys_bubble_down_patchlist[], __end_gate_brl_fsys_bubble_down_patchlist[];
extern char __start_unwind[], __end_unwind[];
extern char __start_ivt_text[], __end_ivt_text[];

#undef dereference_function_descriptor
static inline void *dereference_function_descriptor(void *ptr)
{
	struct fdesc *desc = ptr;
	void *p;

	if (!probe_kernel_address(&desc->ip, p))
		ptr = p;
	return ptr;
}


#endif /* _ASM_IA64_SECTIONS_H */

