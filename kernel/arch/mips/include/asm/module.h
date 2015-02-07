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

#ifndef _ASM_MODULE_H
#define _ASM_MODULE_H

#include <linux/list.h>
#include <asm/uaccess.h>

struct mod_arch_specific {
	/* Data Bus Error exception tables */
	struct list_head dbe_list;
	const struct exception_table_entry *dbe_start;
	const struct exception_table_entry *dbe_end;
};

typedef uint8_t Elf64_Byte;		/* Type for a 8-bit quantity.  */

typedef struct {
	Elf64_Addr r_offset;			/* Address of relocation.  */
	Elf64_Word r_sym;			/* Symbol index.  */
	Elf64_Byte r_ssym;			/* Special symbol.  */
	Elf64_Byte r_type3;			/* Third relocation.  */
	Elf64_Byte r_type2;			/* Second relocation.  */
	Elf64_Byte r_type;			/* First relocation.  */
} Elf64_Mips_Rel;

typedef struct {
	Elf64_Addr r_offset;			/* Address of relocation.  */
	Elf64_Word r_sym;			/* Symbol index.  */
	Elf64_Byte r_ssym;			/* Special symbol.  */
	Elf64_Byte r_type3;			/* Third relocation.  */
	Elf64_Byte r_type2;			/* Second relocation.  */
	Elf64_Byte r_type;			/* First relocation.  */
	Elf64_Sxword r_addend;			/* Addend.  */
} Elf64_Mips_Rela;

#ifdef CONFIG_32BIT

#define Elf_Shdr	Elf32_Shdr
#define Elf_Sym		Elf32_Sym
#define Elf_Ehdr	Elf32_Ehdr
#define Elf_Addr	Elf32_Addr

#define Elf_Mips_Rel	Elf32_Rel
#define Elf_Mips_Rela	Elf32_Rela

#define ELF_MIPS_R_SYM(rel) ELF32_R_SYM(rel.r_info)
#define ELF_MIPS_R_TYPE(rel) ELF32_R_TYPE(rel.r_info)

#endif

#ifdef CONFIG_64BIT

#define Elf_Shdr	Elf64_Shdr
#define Elf_Sym		Elf64_Sym
#define Elf_Ehdr	Elf64_Ehdr
#define Elf_Addr	Elf64_Addr

#define Elf_Mips_Rel	Elf64_Mips_Rel
#define Elf_Mips_Rela	Elf64_Mips_Rela

#define ELF_MIPS_R_SYM(rel) (rel.r_sym)
#define ELF_MIPS_R_TYPE(rel) (rel.r_type)

#endif

#ifdef CONFIG_MODULES
/* Given an address, look for it in the exception tables. */
const struct exception_table_entry*search_module_dbetables(unsigned long addr);
#else
/* Given an address, look for it in the exception tables. */
static inline const struct exception_table_entry *
search_module_dbetables(unsigned long addr)
{
	return NULL;
}
#endif

#ifdef CONFIG_CPU_MIPS32_R1
#define MODULE_PROC_FAMILY "MIPS32_R1 "
#elif defined CONFIG_CPU_MIPS32_R2
#define MODULE_PROC_FAMILY "MIPS32_R2 "
#elif defined CONFIG_CPU_MIPS64_R1
#define MODULE_PROC_FAMILY "MIPS64_R1 "
#elif defined CONFIG_CPU_MIPS64_R2
#define MODULE_PROC_FAMILY "MIPS64_R2 "
#elif defined CONFIG_CPU_R3000
#define MODULE_PROC_FAMILY "R3000 "
#elif defined CONFIG_CPU_TX39XX
#define MODULE_PROC_FAMILY "TX39XX "
#elif defined CONFIG_CPU_VR41XX
#define MODULE_PROC_FAMILY "VR41XX "
#elif defined CONFIG_CPU_R4300
#define MODULE_PROC_FAMILY "R4300 "
#elif defined CONFIG_CPU_R4X00
#define MODULE_PROC_FAMILY "R4X00 "
#elif defined CONFIG_CPU_TX49XX
#define MODULE_PROC_FAMILY "TX49XX "
#elif defined CONFIG_CPU_R5000
#define MODULE_PROC_FAMILY "R5000 "
#elif defined CONFIG_CPU_R5432
#define MODULE_PROC_FAMILY "R5432 "
#elif defined CONFIG_CPU_R5500
#define MODULE_PROC_FAMILY "R5500 "
#elif defined CONFIG_CPU_R6000
#define MODULE_PROC_FAMILY "R6000 "
#elif defined CONFIG_CPU_NEVADA
#define MODULE_PROC_FAMILY "NEVADA "
#elif defined CONFIG_CPU_R8000
#define MODULE_PROC_FAMILY "R8000 "
#elif defined CONFIG_CPU_R10000
#define MODULE_PROC_FAMILY "R10000 "
#elif defined CONFIG_CPU_RM7000
#define MODULE_PROC_FAMILY "RM7000 "
#elif defined CONFIG_CPU_RM9000
#define MODULE_PROC_FAMILY "RM9000 "
#elif defined CONFIG_CPU_SB1
#define MODULE_PROC_FAMILY "SB1 "
#elif defined CONFIG_CPU_LOONGSON2
#define MODULE_PROC_FAMILY "LOONGSON2 "
#elif defined CONFIG_CPU_CAVIUM_OCTEON
#define MODULE_PROC_FAMILY "OCTEON "
#else
#error MODULE_PROC_FAMILY undefined for your processor configuration
#endif

#ifdef CONFIG_32BIT
#define MODULE_KERNEL_TYPE "32BIT "
#elif defined CONFIG_64BIT
#define MODULE_KERNEL_TYPE "64BIT "
#endif

#ifdef CONFIG_MIPS_MT_SMTC
#define MODULE_KERNEL_SMTC "MT_SMTC "
#else
#define MODULE_KERNEL_SMTC ""
#endif

#define MODULE_ARCH_VERMAGIC \
	MODULE_PROC_FAMILY MODULE_KERNEL_TYPE MODULE_KERNEL_SMTC

#endif /* _ASM_MODULE_H */
