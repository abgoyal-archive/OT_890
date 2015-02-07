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

#ifndef _ASM_S390_MODULE_H
#define _ASM_S390_MODULE_H
/*
 * This file contains the s390 architecture specific module code.
 */

struct mod_arch_syminfo
{
	unsigned long got_offset;
	unsigned long plt_offset;
	int got_initialized;
	int plt_initialized;
};

struct mod_arch_specific
{
	/* Starting offset of got in the module core memory. */
	unsigned long got_offset;
	/* Starting offset of plt in the module core memory. */
	unsigned long plt_offset;
	/* Size of the got. */
	unsigned long got_size;
	/* Size of the plt. */
	unsigned long plt_size;
	/* Number of symbols in syminfo. */
	int nsyms;
	/* Additional symbol information (got and plt offsets). */
	struct mod_arch_syminfo *syminfo;
};

#ifdef __s390x__
#define ElfW(x) Elf64_ ## x
#define ELFW(x) ELF64_ ## x
#else
#define ElfW(x) Elf32_ ## x
#define ELFW(x) ELF32_ ## x
#endif

#define Elf_Addr ElfW(Addr)
#define Elf_Rela ElfW(Rela)
#define Elf_Shdr ElfW(Shdr)
#define Elf_Sym ElfW(Sym)
#define Elf_Ehdr ElfW(Ehdr)
#define ELF_R_SYM ELFW(R_SYM)
#define ELF_R_TYPE ELFW(R_TYPE)
#endif /* _ASM_S390_MODULE_H */
