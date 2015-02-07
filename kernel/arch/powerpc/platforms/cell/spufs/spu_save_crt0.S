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
 * crt0_s.S: Entry function for SPU-side context save.
 *
 * Copyright (C) 2005 IBM
 *
 * Entry function for SPU-side of the context save sequence.
 * Saves all 128 GPRs, sets up an initial stack frame, then
 * branches to 'main'.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <asm/spu_csa.h>

.data
.align 7
.globl regs_spill
regs_spill:
.space SIZEOF_SPU_SPILL_REGS, 0x0

.text
.global _start
_start:
	/* SPU Context Save Step 1: Save the first 16 GPRs. */
	stqa $0, regs_spill + 0
	stqa $1, regs_spill + 16
	stqa $2, regs_spill + 32
	stqa $3, regs_spill + 48
	stqa $4, regs_spill + 64
	stqa $5, regs_spill + 80
	stqa $6, regs_spill + 96
	stqa $7, regs_spill + 112
	stqa $8, regs_spill + 128
	stqa $9, regs_spill + 144
	stqa $10, regs_spill + 160
	stqa $11, regs_spill + 176
	stqa $12, regs_spill + 192
	stqa $13, regs_spill + 208
	stqa $14, regs_spill + 224
	stqa $15, regs_spill + 240

	/* SPU Context Save, Step 8: Save the remaining 112 GPRs. */
	ila     $3, regs_spill + 256
save_regs:
	lqr     $4, save_reg_insts
save_reg_loop:
	ai      $4, $4, 4
	.balignl 16, 0x40200000
save_reg_insts:       /* must be quad-word aligned. */
	stqd    $16, 0($3)
	stqd    $17, 16($3)
	stqd    $18, 32($3)
	stqd    $19, 48($3)
	andi    $5, $4, 0x7F
	stqr    $4, save_reg_insts
	ai      $3, $3, 64
	brnz    $5, save_reg_loop

	/* Initialize the stack pointer to point to 16368
	 * (16kb-16). The back chain pointer is initialized
	 * to NULL.
	 */
	il	$0, 0
	il	$SP, 16368
	stqd	$0, 0($SP)

	/* Allocate a minimum stack frame for the called main.
	 * This is needed so that main has a place to save the
	 * link register when it calls another function.
	 */
	stqd	$SP, -160($SP)
	ai	$SP, $SP, -160

	/* Call the program's main function. */
	brsl	$0, main

	/* In this case main should not return; if it does
	 * there has been an error in the sequence.  Execute
	 * stop-and-signal with code=0.
	 */
.global exit
.global	_exit
exit:
_exit:
	stop	0x0

	/* Pad the size of this crt0.o to be multiple of 16 bytes. */
.balignl 16, 0x0

