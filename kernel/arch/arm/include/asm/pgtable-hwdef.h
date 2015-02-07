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
 *  arch/arm/include/asm/pgtable-hwdef.h
 *
 *  Copyright (C) 1995-2002 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _ASMARM_PGTABLE_HWDEF_H
#define _ASMARM_PGTABLE_HWDEF_H

/*
 * Hardware page table definitions.
 *
 * + Level 1 descriptor (PMD)
 *   - common
 */
#define PMD_TYPE_MASK		(3 << 0)
#define PMD_TYPE_FAULT		(0 << 0)
#define PMD_TYPE_TABLE		(1 << 0)
#define PMD_TYPE_SECT		(2 << 0)
#define PMD_BIT4		(1 << 4)
#define PMD_DOMAIN(x)		((x) << 5)
#define PMD_PROTECTION		(1 << 9)	/* v5 */
/*
 *   - section
 */
#define PMD_SECT_BUFFERABLE	(1 << 2)
#define PMD_SECT_CACHEABLE	(1 << 3)
#define PMD_SECT_XN		(1 << 4)	/* v6 */
#define PMD_SECT_AP_WRITE	(1 << 10)
#define PMD_SECT_AP_READ	(1 << 11)
#define PMD_SECT_TEX(x)		((x) << 12)	/* v5 */
#define PMD_SECT_APX		(1 << 15)	/* v6 */
#define PMD_SECT_S		(1 << 16)	/* v6 */
#define PMD_SECT_nG		(1 << 17)	/* v6 */
#define PMD_SECT_SUPER		(1 << 18)	/* v6 */

#define PMD_SECT_UNCACHED	(0)
#define PMD_SECT_BUFFERED	(PMD_SECT_BUFFERABLE)
#define PMD_SECT_WT		(PMD_SECT_CACHEABLE)
#define PMD_SECT_WB		(PMD_SECT_CACHEABLE | PMD_SECT_BUFFERABLE)
#define PMD_SECT_MINICACHE	(PMD_SECT_TEX(1) | PMD_SECT_CACHEABLE)
#define PMD_SECT_WBWA		(PMD_SECT_TEX(1) | PMD_SECT_CACHEABLE | PMD_SECT_BUFFERABLE)
#define PMD_SECT_NONSHARED_DEV	(PMD_SECT_TEX(2))

/*
 *   - coarse table (not used)
 */

/*
 * + Level 2 descriptor (PTE)
 *   - common
 */
#define PTE_TYPE_MASK		(3 << 0)
#define PTE_TYPE_FAULT		(0 << 0)
#define PTE_TYPE_LARGE		(1 << 0)
#define PTE_TYPE_SMALL		(2 << 0)
#define PTE_TYPE_EXT		(3 << 0)	/* v5 */
#define PTE_BUFFERABLE		(1 << 2)
#define PTE_CACHEABLE		(1 << 3)

/*
 *   - extended small page/tiny page
 */
#define PTE_EXT_XN		(1 << 0)	/* v6 */
#define PTE_EXT_AP_MASK		(3 << 4)
#define PTE_EXT_AP0		(1 << 4)
#define PTE_EXT_AP1		(2 << 4)
#define PTE_EXT_AP_UNO_SRO	(0 << 4)
#define PTE_EXT_AP_UNO_SRW	(PTE_EXT_AP0)
#define PTE_EXT_AP_URO_SRW	(PTE_EXT_AP1)
#define PTE_EXT_AP_URW_SRW	(PTE_EXT_AP1|PTE_EXT_AP0)
#define PTE_EXT_TEX(x)		((x) << 6)	/* v5 */
#define PTE_EXT_APX		(1 << 9)	/* v6 */
#define PTE_EXT_COHERENT	(1 << 9)	/* XScale3 */
#define PTE_EXT_SHARED		(1 << 10)	/* v6 */
#define PTE_EXT_NG		(1 << 11)	/* v6 */

/*
 *   - small page
 */
#define PTE_SMALL_AP_MASK	(0xff << 4)
#define PTE_SMALL_AP_UNO_SRO	(0x00 << 4)
#define PTE_SMALL_AP_UNO_SRW	(0x55 << 4)
#define PTE_SMALL_AP_URO_SRW	(0xaa << 4)
#define PTE_SMALL_AP_URW_SRW	(0xff << 4)

#endif
