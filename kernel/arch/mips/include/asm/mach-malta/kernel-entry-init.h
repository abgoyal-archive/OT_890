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
 * Chris Dearman (chris@mips.com)
 * Copyright (C) 2007 Mips Technologies, Inc.
 */
#ifndef __ASM_MACH_MIPS_KERNEL_ENTRY_INIT_H
#define __ASM_MACH_MIPS_KERNEL_ENTRY_INIT_H

	.macro	kernel_entry_setup
#ifdef CONFIG_MIPS_MT_SMTC
	mfc0	t0, CP0_CONFIG
	bgez	t0, 9f
	mfc0	t0, CP0_CONFIG, 1
	bgez	t0, 9f
	mfc0	t0, CP0_CONFIG, 2
	bgez	t0, 9f
	mfc0	t0, CP0_CONFIG, 3
	and	t0, 1<<2
	bnez	t0, 0f
9:
	/* Assume we came from YAMON... */
	PTR_LA	v0, 0x9fc00534	/* YAMON print */
	lw	v0, (v0)
	move	a0, zero
	PTR_LA	a1, nonmt_processor
	jal	v0

	PTR_LA	v0, 0x9fc00520	/* YAMON exit */
	lw	v0, (v0)
	li	a0, 1
	jal	v0

1:	b	1b

	__INITDATA
nonmt_processor:
	.asciz	"SMTC kernel requires the MT ASE to run\n"
	__FINIT
0:
#endif
	.endm

/*
 * Do SMP slave processor setup necessary before we can safely execute C code.
 */
	.macro	smp_slave_setup
	.endm

#endif /* __ASM_MACH_MIPS_KERNEL_ENTRY_INIT_H */
