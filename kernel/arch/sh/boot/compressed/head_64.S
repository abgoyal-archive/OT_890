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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * arch/shmedia/boot/compressed/head.S
 *
 * Copied from
 *   arch/shmedia/kernel/head.S
 * which carried the copyright:
 *   Copyright (C) 2000, 2001  Paolo Alberelli
 *
 * Modification for compressed loader:
 *   Copyright (C) 2002 Stuart Menefy (stuart.menefy@st.com)
 */
#include <asm/cache.h>
#include <cpu/mmu_context.h>
#include <cpu/registers.h>

/*
 * Fixed TLB entries to identity map the beginning of RAM
 */
#define MMUIR_TEXT_H	0x0000000000000003 | CONFIG_MEMORY_START
			/* Enabled, Shared, ASID 0, Eff. Add. 0xA0000000 */
#define MMUIR_TEXT_L	0x000000000000009a | CONFIG_MEMORY_START
			/* 512 Mb, Cacheable (Write-back), execute, Not User, Ph. Add. */

#define MMUDR_CACHED_H	0x0000000000000003 | CONFIG_MEMORY_START
			/* Enabled, Shared, ASID 0, Eff. Add. 0xA0000000 */
#define MMUDR_CACHED_L	0x000000000000015a | CONFIG_MEMORY_START
			/* 512 Mb, Cacheable (Write-back), read/write, Not User, Ph. Add. */

#define	ICCR0_INIT_VAL	ICCR0_ON | ICCR0_ICI		/* ICE + ICI */
#define	ICCR1_INIT_VAL	ICCR1_NOLOCK			/* No locking */

#if 1
#define	OCCR0_INIT_VAL	OCCR0_ON | OCCR0_OCI | OCCR0_WB	/* OCE + OCI + WB */
#else
#define	OCCR0_INIT_VAL	OCCR0_OFF
#endif
#define	OCCR1_INIT_VAL	OCCR1_NOLOCK			/* No locking */

	.text

	.global	startup
startup:
	/*
	 * Prevent speculative fetch on device memory due to
	 * uninitialized target registers.
	 * This must be executed before the first branch.
	 */
	ptabs/u	r63, tr0
	ptabs/u	r63, tr1
	ptabs/u	r63, tr2
	ptabs/u	r63, tr3
	ptabs/u	r63, tr4
	ptabs/u	r63, tr5
	ptabs/u	r63, tr6
	ptabs/u	r63, tr7
	synci

	/*
	 * Set initial TLB entries for cached and uncached regions.
	 * Note: PTA/BLINK is PIC code, PTABS/BLINK isn't !
	 */
	/* Clear ITLBs */
	pta	1f, tr1
	movi	ITLB_FIXED, r21
	movi	ITLB_LAST_VAR_UNRESTRICTED+TLB_STEP, r22
1:	putcfg	r21, 0, r63		/* Clear MMUIR[n].PTEH.V */
	addi	r21, TLB_STEP, r21
        bne	r21, r22, tr1

	/* Clear DTLBs */
	pta	1f, tr1
	movi	DTLB_FIXED, r21
	movi	DTLB_LAST_VAR_UNRESTRICTED+TLB_STEP, r22
1:	putcfg	r21, 0, r63		/* Clear MMUDR[n].PTEH.V */
	addi	r21, TLB_STEP, r21
        bne	r21, r22, tr1

	/* Map one big (512Mb) page for ITLB */
	movi	ITLB_FIXED, r21
	movi	MMUIR_TEXT_L, r22	/* PTEL first */
	putcfg	r21, 1, r22		/* Set MMUIR[0].PTEL */
	movi	MMUIR_TEXT_H, r22	/* PTEH last */
	putcfg	r21, 0, r22		/* Set MMUIR[0].PTEH */

	/* Map one big CACHED (512Mb) page for DTLB */
	movi	DTLB_FIXED, r21
	movi	MMUDR_CACHED_L, r22	/* PTEL first */
	putcfg	r21, 1, r22		/* Set MMUDR[0].PTEL */
	movi	MMUDR_CACHED_H, r22	/* PTEH last */
	putcfg	r21, 0, r22		/* Set MMUDR[0].PTEH */

	/* ICache */
	movi	ICCR_BASE, r21
	movi	ICCR0_INIT_VAL, r22
	movi	ICCR1_INIT_VAL, r23
	putcfg	r21, ICCR_REG0, r22
	putcfg	r21, ICCR_REG1, r23
	synci

	/* OCache */
	movi	OCCR_BASE, r21
	movi	OCCR0_INIT_VAL, r22
	movi	OCCR1_INIT_VAL, r23
	putcfg	r21, OCCR_REG0, r22
	putcfg	r21, OCCR_REG1, r23
	synco

	/*
	 * Enable the MMU.
	 * From here-on code can be non-PIC.
	 */
	movi	SR_HARMLESS | SR_ENABLE_MMU, r22
	putcon	r22, SSR
	movi	1f, r22
	putcon	r22, SPC
	synco
	rte				/* And now go into the hyperspace ... */
1:					/* ... that's the next instruction ! */

	/* Set initial stack pointer */
	movi	datalabel stack_start, r0
	ld.l	r0, 0, r15

	/*
	 * Clear bss
	 */
	pt	1f, tr1
	movi	datalabel __bss_start, r22
	movi	datalabel _end, r23
1:	st.l	r22, 0, r63
	addi	r22, 4, r22
	bne	r22, r23, tr1

	/*
	 * Decompress the kernel.
	 */
	pt	decompress_kernel, tr0
	blink	tr0, r18

	/*
	 * Disable the MMU.
	 */
	movi	SR_HARMLESS, r22
	putcon	r22, SSR
	movi	1f, r22
	putcon	r22, SPC
	synco
	rte				/* And now go into the hyperspace ... */
1:					/* ... that's the next instruction ! */

	/* Jump into the decompressed kernel */
	movi	datalabel (CONFIG_MEMORY_START + 0x2000)+1, r19
	ptabs	r19, tr0
	blink	tr0, r18

	/* Shouldn't return here, but just in case, loop forever */
	pt	1f, tr0
1:	blink	tr0, r63
