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

#ifndef _ASM_IA64_MODULE_H
#define _ASM_IA64_MODULE_H

/*
 * IA-64-specific support for kernel module loader.
 *
 * Copyright (C) 2003 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 */

struct elf64_shdr;			/* forward declration */

struct mod_arch_specific {
	struct elf64_shdr *core_plt;	/* core PLT section */
	struct elf64_shdr *init_plt;	/* init PLT section */
	struct elf64_shdr *got;		/* global offset table */
	struct elf64_shdr *opd;		/* official procedure descriptors */
	struct elf64_shdr *unwind;	/* unwind-table section */
	unsigned long gp;		/* global-pointer for module */

	void *core_unw_table;		/* core unwind-table cookie returned by unwinder */
	void *init_unw_table;		/* init unwind-table cookie returned by unwinder */
	unsigned int next_got_entry;	/* index of next available got entry */
};

#define Elf_Shdr	Elf64_Shdr
#define Elf_Sym		Elf64_Sym
#define Elf_Ehdr	Elf64_Ehdr

#define MODULE_PROC_FAMILY	"ia64"
#define MODULE_ARCH_VERMAGIC	MODULE_PROC_FAMILY \
	"gcc-" __stringify(__GNUC__) "." __stringify(__GNUC_MINOR__)

#define ARCH_SHF_SMALL	SHF_IA_64_SHORT

#endif /* _ASM_IA64_MODULE_H */
