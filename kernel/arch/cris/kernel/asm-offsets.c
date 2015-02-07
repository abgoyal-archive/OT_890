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

#include <linux/sched.h>
#include <asm/thread_info.h>
#include <linux/autoconf.h>

/*
 * Generate definitions needed by assembly language modules.
 * This code generates raw asm output which is post-processed to extract
 * and format the required data.
 */

#define DEFINE(sym, val) \
	asm volatile("\n->" #sym " %0 " #val : : "i" (val))

#define BLANK() asm volatile("\n->" : : )

#if !defined(CONFIG_ETRAX_ARCH_V10) && !defined(CONFIG_ETRAX_ARCH_V32)
#error One of ARCH v10 and ARCH v32 must be true!
#endif

int main(void)
{
#define ENTRY(entry) DEFINE(PT_ ## entry, offsetof(struct pt_regs, entry))
	ENTRY(orig_r10);
	ENTRY(r13);
	ENTRY(r12);
	ENTRY(r11);
	ENTRY(r10);
	ENTRY(r9);
#ifdef CONFIG_ETRAX_ARCH_V32
	ENTRY(acr);
	ENTRY(srs);
#endif
	ENTRY(mof);
#ifdef CONFIG_ETRAX_ARCH_V10
	ENTRY(dccr);
#else
	ENTRY(ccs);
#endif
	ENTRY(srp);
	BLANK();
#undef ENTRY
#define ENTRY(entry) DEFINE(TI_ ## entry, offsetof(struct thread_info, entry))
	ENTRY(task);
	ENTRY(flags);
	ENTRY(preempt_count);
	BLANK();
#undef ENTRY
#define ENTRY(entry) DEFINE(THREAD_ ## entry, offsetof(struct thread_struct, entry))
	ENTRY(ksp);
	ENTRY(usp);
#ifdef CONFIG_ETRAX_ARCH_V10
	ENTRY(dccr);
#else
	ENTRY(ccs);
#endif
	BLANK();
#undef ENTRY
#define ENTRY(entry) DEFINE(TASK_ ## entry, offsetof(struct task_struct, entry))
	ENTRY(pid);
	BLANK();
	DEFINE(LCLONE_VM, CLONE_VM);
	DEFINE(LCLONE_UNTRACED, CLONE_UNTRACED);
	return 0;
}
