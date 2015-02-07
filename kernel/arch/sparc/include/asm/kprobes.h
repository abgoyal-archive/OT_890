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

#ifndef _SPARC64_KPROBES_H
#define _SPARC64_KPROBES_H

#include <linux/types.h>
#include <linux/percpu.h>

typedef u32 kprobe_opcode_t;

#define BREAKPOINT_INSTRUCTION   0x91d02070 /* ta 0x70 */
#define BREAKPOINT_INSTRUCTION_2 0x91d02071 /* ta 0x71 */
#define MAX_INSN_SIZE 2

#define kretprobe_blacklist_size 0

#define arch_remove_kprobe(p)	do {} while (0)

#define flush_insn_slot(p)		\
do { 	flushi(&(p)->ainsn.insn[0]);	\
	flushi(&(p)->ainsn.insn[1]);	\
} while (0)

void kretprobe_trampoline(void);

/* Architecture specific copy of original instruction*/
struct arch_specific_insn {
	/* copy of the original instruction */
	kprobe_opcode_t insn[MAX_INSN_SIZE];
};

struct prev_kprobe {
	struct kprobe *kp;
	unsigned long status;
	unsigned long orig_tnpc;
	unsigned long orig_tstate_pil;
};

/* per-cpu kprobe control block */
struct kprobe_ctlblk {
	unsigned long kprobe_status;
	unsigned long kprobe_orig_tnpc;
	unsigned long kprobe_orig_tstate_pil;
	struct pt_regs jprobe_saved_regs;
	struct prev_kprobe prev_kprobe;
};

extern int kprobe_exceptions_notify(struct notifier_block *self,
				    unsigned long val, void *data);
extern int kprobe_fault_handler(struct pt_regs *regs, int trapnr);
#endif /* _SPARC64_KPROBES_H */
