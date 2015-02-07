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

#ifndef _LINUX_BUG_H
#define _LINUX_BUG_H

#include <linux/module.h>
#include <asm/bug.h>

enum bug_trap_type {
	BUG_TRAP_TYPE_NONE = 0,
	BUG_TRAP_TYPE_WARN = 1,
	BUG_TRAP_TYPE_BUG = 2,
};

struct pt_regs;

#ifdef CONFIG_GENERIC_BUG
#include <asm-generic/bug.h>

static inline int is_warning_bug(const struct bug_entry *bug)
{
	return bug->flags & BUGFLAG_WARNING;
}

const struct bug_entry *find_bug(unsigned long bugaddr);

enum bug_trap_type report_bug(unsigned long bug_addr, struct pt_regs *regs);

int  module_bug_finalize(const Elf_Ehdr *, const Elf_Shdr *,
			 struct module *);
void module_bug_cleanup(struct module *);

/* These are defined by the architecture */
int is_valid_bugaddr(unsigned long addr);

#else	/* !CONFIG_GENERIC_BUG */

static inline enum bug_trap_type report_bug(unsigned long bug_addr,
					    struct pt_regs *regs)
{
	return BUG_TRAP_TYPE_BUG;
}
static inline int  module_bug_finalize(const Elf_Ehdr *hdr,
					const Elf_Shdr *sechdrs,
					struct module *mod)
{
	return 0;
}
static inline void module_bug_cleanup(struct module *mod) {}

#endif	/* CONFIG_GENERIC_BUG */
#endif	/* _LINUX_BUG_H */
