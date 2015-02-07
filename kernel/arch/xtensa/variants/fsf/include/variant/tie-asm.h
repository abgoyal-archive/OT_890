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
 * This header file contains assembly-language definitions (assembly
 * macros, etc.) for this specific Xtensa processor's TIE extensions
 * and options.  It is customized to this Xtensa processor configuration.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1999-2008 Tensilica Inc.
 */

#ifndef _XTENSA_CORE_TIE_ASM_H
#define _XTENSA_CORE_TIE_ASM_H

/*  Selection parameter values for save-area save/restore macros:  */
/*  Option vs. TIE:  */
#define XTHAL_SAS_TIE	0x0001	/* custom extension or coprocessor */
#define XTHAL_SAS_OPT	0x0002	/* optional (and not a coprocessor) */
/*  Whether used automatically by compiler:  */
#define XTHAL_SAS_NOCC	0x0004	/* not used by compiler w/o special opts/code */
#define XTHAL_SAS_CC	0x0008	/* used by compiler without special opts/code */
/*  ABI handling across function calls:  */
#define XTHAL_SAS_CALR	0x0010	/* caller-saved */
#define XTHAL_SAS_CALE	0x0020	/* callee-saved */
#define XTHAL_SAS_GLOB	0x0040	/* global across function calls (in thread) */
/*  Misc  */
#define XTHAL_SAS_ALL	0xFFFF	/* include all default NCP contents */



/* Macro to save all non-coprocessor (extra) custom TIE and optional state
 * (not including zero-overhead loop registers).
 * Save area ptr (clobbered):  ptr  (1 byte aligned)
 * Scratch regs  (clobbered):  at1..at4  (only first XCHAL_NCP_NUM_ATMPS needed)
 */
	.macro xchal_ncp_store  ptr at1 at2 at3 at4  continue=0 ofs=-1 select=XTHAL_SAS_ALL
	xchal_sa_start	\continue, \ofs
	.ifeq (XTHAL_SAS_OPT | XTHAL_SAS_CC | XTHAL_SAS_GLOB) & ~\select
	xchal_sa_align	\ptr, 0, 1024-4, 4, 4
	rur	\at1, THREADPTR		// threadptr option
	s32i	\at1, \ptr, .Lxchal_ofs_ + 0
	.set	.Lxchal_ofs_, .Lxchal_ofs_ + 4
	.endif
	.endm	// xchal_ncp_store

/* Macro to save all non-coprocessor (extra) custom TIE and optional state
 * (not including zero-overhead loop registers).
 * Save area ptr (clobbered):  ptr  (1 byte aligned)
 * Scratch regs  (clobbered):  at1..at4  (only first XCHAL_NCP_NUM_ATMPS needed)
 */
	.macro xchal_ncp_load  ptr at1 at2 at3 at4  continue=0 ofs=-1 select=XTHAL_SAS_ALL
	xchal_sa_start	\continue, \ofs
	.ifeq (XTHAL_SAS_OPT | XTHAL_SAS_CC | XTHAL_SAS_GLOB) & ~\select
	xchal_sa_align	\ptr, 0, 1024-4, 4, 4
	l32i	\at1, \ptr, .Lxchal_ofs_ + 0
	wur	\at1, THREADPTR		// threadptr option
	.set	.Lxchal_ofs_, .Lxchal_ofs_ + 4
	.endif
	.endm	// xchal_ncp_load



#define XCHAL_NCP_NUM_ATMPS	1


#define XCHAL_SA_NUM_ATMPS	1

#endif /*_XTENSA_CORE_TIE_ASM_H*/

