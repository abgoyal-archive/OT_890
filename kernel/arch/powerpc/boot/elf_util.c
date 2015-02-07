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

/*
 * Copyright (C) Paul Mackerras 1997.
 *
 * Updates for PPC64 by Todd Inglett, Dave Engebretsen & Peter Bergner.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#include <stdarg.h>
#include <stddef.h>
#include "elf.h"
#include "page.h"
#include "string.h"
#include "stdio.h"

int parse_elf64(void *hdr, struct elf_info *info)
{
	Elf64_Ehdr *elf64 = hdr;
	Elf64_Phdr *elf64ph;
	unsigned int i;

	if (!(elf64->e_ident[EI_MAG0]  == ELFMAG0	&&
	      elf64->e_ident[EI_MAG1]  == ELFMAG1	&&
	      elf64->e_ident[EI_MAG2]  == ELFMAG2	&&
	      elf64->e_ident[EI_MAG3]  == ELFMAG3	&&
	      elf64->e_ident[EI_CLASS] == ELFCLASS64	&&
	      elf64->e_ident[EI_DATA]  == ELFDATA2MSB	&&
	      (elf64->e_type            == ET_EXEC ||
	       elf64->e_type            == ET_DYN)	&&
	      elf64->e_machine         == EM_PPC64))
		return 0;

	elf64ph = (Elf64_Phdr *)((unsigned long)elf64 +
				 (unsigned long)elf64->e_phoff);
	for (i = 0; i < (unsigned int)elf64->e_phnum; i++, elf64ph++)
		if (elf64ph->p_type == PT_LOAD)
			break;
	if (i >= (unsigned int)elf64->e_phnum)
		return 0;

	info->loadsize = (unsigned long)elf64ph->p_filesz;
	info->memsize = (unsigned long)elf64ph->p_memsz;
	info->elfoffset = (unsigned long)elf64ph->p_offset;

	return 1;
}

int parse_elf32(void *hdr, struct elf_info *info)
{
	Elf32_Ehdr *elf32 = hdr;
	Elf32_Phdr *elf32ph;
	unsigned int i;

	if (!(elf32->e_ident[EI_MAG0]  == ELFMAG0	&&
	      elf32->e_ident[EI_MAG1]  == ELFMAG1	&&
	      elf32->e_ident[EI_MAG2]  == ELFMAG2	&&
	      elf32->e_ident[EI_MAG3]  == ELFMAG3	&&
	      elf32->e_ident[EI_CLASS] == ELFCLASS32	&&
	      elf32->e_ident[EI_DATA]  == ELFDATA2MSB	&&
	      (elf32->e_type            == ET_EXEC ||
	       elf32->e_type            == ET_DYN)      &&
	      elf32->e_machine         == EM_PPC))
		return 0;

	elf32ph = (Elf32_Phdr *) ((unsigned long)elf32 + elf32->e_phoff);
	for (i = 0; i < elf32->e_phnum; i++, elf32ph++)
		if (elf32ph->p_type == PT_LOAD)
			break;
	if (i >= elf32->e_phnum)
		return 0;

	info->loadsize = elf32ph->p_filesz;
	info->memsize = elf32ph->p_memsz;
	info->elfoffset = elf32ph->p_offset;
	return 1;
}
