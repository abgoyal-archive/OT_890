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
 *  arch/um/kernel/elf_aux.c
 *
 *  Scan the Elf auxiliary vector provided by the host to extract
 *  information about vsyscall-page, etc.
 *
 *  Copyright (C) 2004 Fujitsu Siemens Computers GmbH
 *  Author: Bodo Stroesser (bodo.stroesser@fujitsu-siemens.com)
 */
#include <elf.h>
#include <stddef.h>
#include "init.h"
#include "elf_user.h"
#include "mem_user.h"
#include <kern_constants.h>

/* Use the one from the kernel - the host may miss it, if having old headers. */
#if UM_ELF_CLASS == UM_ELFCLASS32
typedef Elf32_auxv_t elf_auxv_t;
#else
typedef Elf64_auxv_t elf_auxv_t;
#endif

/* These are initialized very early in boot and never changed */
char * elf_aux_platform;
long elf_aux_hwcap;
unsigned long vsyscall_ehdr;
unsigned long vsyscall_end;
unsigned long __kernel_vsyscall;

__init void scan_elf_aux( char **envp)
{
	long page_size = 0;
	elf_auxv_t * auxv;

	while ( *envp++ != NULL) ;

	for ( auxv = (elf_auxv_t *)envp; auxv->a_type != AT_NULL; auxv++) {
		switch ( auxv->a_type ) {
			case AT_SYSINFO:
				__kernel_vsyscall = auxv->a_un.a_val;
				/* See if the page is under TASK_SIZE */
				if (__kernel_vsyscall < (unsigned long) envp)
					__kernel_vsyscall = 0;
				break;
			case AT_SYSINFO_EHDR:
				vsyscall_ehdr = auxv->a_un.a_val;
				/* See if the page is under TASK_SIZE */
				if (vsyscall_ehdr < (unsigned long) envp)
					vsyscall_ehdr = 0;
				break;
			case AT_HWCAP:
				elf_aux_hwcap = auxv->a_un.a_val;
				break;
			case AT_PLATFORM:
                                /* elf.h removed the pointer elements from
                                 * a_un, so we have to use a_val, which is
                                 * all that's left.
                                 */
				elf_aux_platform =
					(char *) (long) auxv->a_un.a_val;
				break;
			case AT_PAGESZ:
				page_size = auxv->a_un.a_val;
				break;
		}
	}
	if ( ! __kernel_vsyscall || ! vsyscall_ehdr ||
	     ! elf_aux_hwcap || ! elf_aux_platform ||
	     ! page_size || (vsyscall_ehdr % page_size) ) {
		__kernel_vsyscall = 0;
		vsyscall_ehdr = 0;
		elf_aux_hwcap = 0;
		elf_aux_platform = "i586";
	}
	else {
		vsyscall_end = vsyscall_ehdr + page_size;
	}
}
