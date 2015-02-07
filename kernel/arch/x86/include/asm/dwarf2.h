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

#ifndef _ASM_X86_DWARF2_H
#define _ASM_X86_DWARF2_H

#ifndef __ASSEMBLY__
#warning "asm/dwarf2.h should be only included in pure assembly files"
#endif

/*
 * Macros for dwarf2 CFI unwind table entries.
 * See "as.info" for details on these pseudo ops. Unfortunately
 * they are only supported in very new binutils, so define them
 * away for older version.
 */

#ifdef CONFIG_AS_CFI

#define CFI_STARTPROC		.cfi_startproc
#define CFI_ENDPROC		.cfi_endproc
#define CFI_DEF_CFA		.cfi_def_cfa
#define CFI_DEF_CFA_REGISTER	.cfi_def_cfa_register
#define CFI_DEF_CFA_OFFSET	.cfi_def_cfa_offset
#define CFI_ADJUST_CFA_OFFSET	.cfi_adjust_cfa_offset
#define CFI_OFFSET		.cfi_offset
#define CFI_REL_OFFSET		.cfi_rel_offset
#define CFI_REGISTER		.cfi_register
#define CFI_RESTORE		.cfi_restore
#define CFI_REMEMBER_STATE	.cfi_remember_state
#define CFI_RESTORE_STATE	.cfi_restore_state
#define CFI_UNDEFINED		.cfi_undefined

#ifdef CONFIG_AS_CFI_SIGNAL_FRAME
#define CFI_SIGNAL_FRAME	.cfi_signal_frame
#else
#define CFI_SIGNAL_FRAME
#endif

#else

/*
 * Due to the structure of pre-exisiting code, don't use assembler line
 * comment character # to ignore the arguments. Instead, use a dummy macro.
 */
.macro cfi_ignore a=0, b=0, c=0, d=0
.endm

#define CFI_STARTPROC		cfi_ignore
#define CFI_ENDPROC		cfi_ignore
#define CFI_DEF_CFA		cfi_ignore
#define CFI_DEF_CFA_REGISTER	cfi_ignore
#define CFI_DEF_CFA_OFFSET	cfi_ignore
#define CFI_ADJUST_CFA_OFFSET	cfi_ignore
#define CFI_OFFSET		cfi_ignore
#define CFI_REL_OFFSET		cfi_ignore
#define CFI_REGISTER		cfi_ignore
#define CFI_RESTORE		cfi_ignore
#define CFI_REMEMBER_STATE	cfi_ignore
#define CFI_RESTORE_STATE	cfi_ignore
#define CFI_UNDEFINED		cfi_ignore
#define CFI_SIGNAL_FRAME	cfi_ignore

#endif

/*
 * An attempt to make CFI annotations more or less
 * correct and shorter. It is implied that you know
 * what you're doing if you use them.
 */
#ifdef __ASSEMBLY__
#ifdef CONFIG_X86_64
	.macro pushq_cfi reg
	pushq \reg
	CFI_ADJUST_CFA_OFFSET 8
	.endm

	.macro popq_cfi reg
	popq \reg
	CFI_ADJUST_CFA_OFFSET -8
	.endm

	.macro movq_cfi reg offset=0
	movq %\reg, \offset(%rsp)
	CFI_REL_OFFSET \reg, \offset
	.endm

	.macro movq_cfi_restore offset reg
	movq \offset(%rsp), %\reg
	CFI_RESTORE \reg
	.endm
#else /*!CONFIG_X86_64*/

	/* 32bit defenitions are missed yet */

#endif /*!CONFIG_X86_64*/
#endif /*__ASSEMBLY__*/

#endif /* _ASM_X86_DWARF2_H */
