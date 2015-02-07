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

#ifndef _ASM_POWERPC_CODE_PATCHING_H
#define _ASM_POWERPC_CODE_PATCHING_H

/*
 * Copyright 2008, Michael Ellerman, IBM Corporation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <asm/types.h>

#define PPC_NOP_INSTR		0x60000000
#define PPC_LWSYNC_INSTR	0x7c2004ac

/* Flags for create_branch:
 * "b"   == create_branch(addr, target, 0);
 * "ba"  == create_branch(addr, target, BRANCH_ABSOLUTE);
 * "bl"  == create_branch(addr, target, BRANCH_SET_LINK);
 * "bla" == create_branch(addr, target, BRANCH_ABSOLUTE | BRANCH_SET_LINK);
 */
#define BRANCH_SET_LINK	0x1
#define BRANCH_ABSOLUTE	0x2

unsigned int create_branch(const unsigned int *addr,
			   unsigned long target, int flags);
unsigned int create_cond_branch(const unsigned int *addr,
				unsigned long target, int flags);
void patch_branch(unsigned int *addr, unsigned long target, int flags);
void patch_instruction(unsigned int *addr, unsigned int instr);

int instr_is_relative_branch(unsigned int instr);
int instr_is_branch_to_addr(const unsigned int *instr, unsigned long addr);
unsigned long branch_target(const unsigned int *instr);
unsigned int translate_branch(const unsigned int *dest,
			      const unsigned int *src);

static inline unsigned long ppc_function_entry(void *func)
{
#ifdef CONFIG_PPC64
	/*
	 * On PPC64 the function pointer actually points to the function's
	 * descriptor. The first entry in the descriptor is the address
	 * of the function text.
	 */
	return ((func_descr_t *)func)->entry;
#else
	return (unsigned long)func;
#endif
}

#endif /* _ASM_POWERPC_CODE_PATCHING_H */
