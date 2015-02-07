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

#include<linux/compat.h>
#include<linux/compat_siginfo.h>
#include<asm/compat_ucontext.h>

#ifndef _ASM_PARISC_COMPAT_RT_SIGFRAME_H
#define _ASM_PARISC_COMPAT_RT_SIGFRAME_H

/* In a deft move of uber-hackery, we decide to carry the top half of all
 * 64-bit registers in a non-portable, non-ABI, hidden structure.
 * Userspace can read the hidden structure if it *wants* but is never
 * guaranteed to be in the same place. Infact the uc_sigmask from the 
 * ucontext_t structure may push the hidden register file downards
 */
struct compat_regfile {
	/* Upper half of all the 64-bit registers that were truncated
	   on a copy to a 32-bit userspace */
	compat_int_t rf_gr[32];
	compat_int_t rf_iasq[2];
	compat_int_t rf_iaoq[2];
	compat_int_t rf_sar;
};

#define COMPAT_SIGRETURN_TRAMP 4
#define COMPAT_SIGRESTARTBLOCK_TRAMP 5 
#define COMPAT_TRAMP_SIZE (COMPAT_SIGRETURN_TRAMP + COMPAT_SIGRESTARTBLOCK_TRAMP)

struct compat_rt_sigframe {
	/* XXX: Must match trampoline size in arch/parisc/kernel/signal.c 
	        Secondary to that it must protect the ERESTART_RESTARTBLOCK
		trampoline we left on the stack (we were bad and didn't 
		change sp so we could run really fast.) */
	compat_uint_t tramp[COMPAT_TRAMP_SIZE];
	compat_siginfo_t info;
	struct compat_ucontext uc;
	/* Hidden location of truncated registers, *must* be last. */
	struct compat_regfile regs; 
};

/*
 * The 32-bit ABI wants at least 48 bytes for a function call frame:
 * 16 bytes for arg0-arg3, and 32 bytes for magic (the only part of
 * which Linux/parisc uses is sp-20 for the saved return pointer...)
 * Then, the stack pointer must be rounded to a cache line (64 bytes).
 */
#define SIGFRAME32		64
#define FUNCTIONCALLFRAME32	48
#define PARISC_RT_SIGFRAME_SIZE32					\
	(((sizeof(struct compat_rt_sigframe) + FUNCTIONCALLFRAME32) + SIGFRAME32) & -SIGFRAME32)

#endif
