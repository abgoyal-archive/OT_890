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
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#include <stdarg.h>
#include <stddef.h>
#include "types.h"
#include "elf.h"
#include "string.h"
#include "stdio.h"
#include "page.h"
#include "ops.h"

#include "of.h"

/* Value picked to match that used by yaboot */
#define PROG_START	0x01400000	/* only used on 64-bit systems */
#define RAM_END		(512<<20)	/* Fixme: use OF */
#define	ONE_MB		0x100000



static unsigned long claim_base;

static void *of_try_claim(unsigned long size)
{
	unsigned long addr = 0;

	if (claim_base == 0)
		claim_base = _ALIGN_UP((unsigned long)_end, ONE_MB);

	for(; claim_base < RAM_END; claim_base += ONE_MB) {
#ifdef DEBUG
		printf("    trying: 0x%08lx\n\r", claim_base);
#endif
		addr = (unsigned long)of_claim(claim_base, size, 0);
		if ((void *)addr != (void *)-1)
			break;
	}
	if (addr == 0)
		return NULL;
	claim_base = PAGE_ALIGN(claim_base + size);
	return (void *)addr;
}

static void of_image_hdr(const void *hdr)
{
	const Elf64_Ehdr *elf64 = hdr;

	if (elf64->e_ident[EI_CLASS] == ELFCLASS64) {
		/*
		 * Maintain a "magic" minimum address. This keeps some older
		 * firmware platforms running.
		 */
		if (claim_base < PROG_START)
			claim_base = PROG_START;
	}
}

void platform_init(unsigned long a1, unsigned long a2, void *promptr)
{
	platform_ops.image_hdr = of_image_hdr;
	platform_ops.malloc = of_try_claim;
	platform_ops.exit = of_exit;
	platform_ops.vmlinux_alloc = of_vmlinux_alloc;

	dt_ops.finddevice = of_finddevice;
	dt_ops.getprop = of_getprop;
	dt_ops.setprop = of_setprop;

	of_console_init();

	of_init(promptr);
	loader_info.promptr = promptr;
	if (a1 && a2 && a2 != 0xdeadbeef) {
		loader_info.initrd_addr = a1;
		loader_info.initrd_size = a2;
	}
}
