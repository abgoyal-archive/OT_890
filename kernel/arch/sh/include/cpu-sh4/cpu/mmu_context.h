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
 * include/asm-sh/cpu-sh4/mmu_context.h
 *
 * Copyright (C) 1999 Niibe Yutaka
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef __ASM_CPU_SH4_MMU_CONTEXT_H
#define __ASM_CPU_SH4_MMU_CONTEXT_H

#define MMU_PTEH	0xFF000000	/* Page table entry register HIGH */
#define MMU_PTEL	0xFF000004	/* Page table entry register LOW */
#define MMU_TTB		0xFF000008	/* Translation table base register */
#define MMU_TEA		0xFF00000C	/* TLB Exception Address */
#define MMU_PTEA	0xFF000034	/* Page table entry assistance register */

#define MMUCR		0xFF000010	/* MMU Control Register */

#define MMU_ITLB_ADDRESS_ARRAY	0xF2000000
#define MMU_UTLB_ADDRESS_ARRAY	0xF6000000
#define MMU_PAGE_ASSOC_BIT	0x80

#define MMUCR_TI		(1<<2)

#ifdef CONFIG_X2TLB
#define MMUCR_ME		(1 << 7)
#else
#define MMUCR_ME		(0)
#endif

#if defined(CONFIG_32BIT) && defined(CONFIG_CPU_SUBTYPE_ST40)
#define MMUCR_SE		(1 << 4)
#else
#define MMUCR_SE		(0)
#endif

#ifdef CONFIG_SH_STORE_QUEUES
#define MMUCR_SQMD		(1 << 9)
#else
#define MMUCR_SQMD		(0)
#endif

#define MMU_NTLB_ENTRIES	64
#define MMU_CONTROL_INIT	(0x05|MMUCR_SQMD|MMUCR_ME|MMUCR_SE)

#define MMU_ITLB_DATA_ARRAY	0xF3000000
#define MMU_UTLB_DATA_ARRAY	0xF7000000

#define MMU_UTLB_ENTRIES	   64
#define MMU_U_ENTRY_SHIFT	    8
#define MMU_UTLB_VALID		0x100
#define MMU_ITLB_ENTRIES	    4
#define MMU_I_ENTRY_SHIFT	    8
#define MMU_ITLB_VALID		0x100

#define TRA	0xff000020
#define EXPEVT	0xff000024
#define INTEVT	0xff000028

#endif /* __ASM_CPU_SH4_MMU_CONTEXT_H */

