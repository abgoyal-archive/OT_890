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

	.text
	.globl __kernel_vsyscall
	.type __kernel_vsyscall,@function
__kernel_vsyscall:
.LSTART_vsyscall:
	/* XXX: We'll have to do something here once we opt to use the vDSO
	 * page for something other than the signal trampoline.. as well as
	 * fill out .eh_frame -- PFM. */
.LEND_vsyscall:
	.size __kernel_vsyscall,.-.LSTART_vsyscall
	.previous

	.section .eh_frame,"a",@progbits
.LCIE:
	.ualong	.LCIE_end - .LCIE_start
.LCIE_start:
	.ualong	0		/* CIE ID */
	.byte	0x1		/* Version number */
	.string	"zRS"		/* NUL-terminated augmentation string */
	.uleb128 0x1		/* Code alignment factor */
	.sleb128 -4		/* Data alignment factor */
	.byte	0x11		/* Return address register column */
				/* Augmentation length and data (none) */
	.byte	0xc		/* DW_CFA_def_cfa */
	.uleb128 0xf		/* r15 */
	.uleb128 0x0		/* offset 0 */

	.align 2
.LCIE_end:

	.ualong	.LFDE_end-.LFDE_start	/* Length FDE */
.LFDE_start:
	.ualong	.LCIE			/* CIE pointer */
	.ualong	.LSTART_vsyscall-.	/* start address */
	.ualong	.LEND_vsyscall-.LSTART_vsyscall
	.uleb128 0
	.align 2
.LFDE_end:
	.previous

/* Get the common code for the sigreturn entry points */
#include "vsyscall-sigreturn.S"
