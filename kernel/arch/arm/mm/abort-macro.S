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
 * The ARM LDRD and Thumb LDRSB instructions use bit 20/11 (ARM/Thumb)
 * differently than every other instruction, so it is set to 0 (write)
 * even though the instructions are read instructions. This means that
 * during an abort the instructions will be treated as a write and the
 * handler will raise a signal from unwriteable locations if they
 * fault. We have to specifically check for these instructions
 * from the abort handlers to treat them properly.
 *
 */

	.macro	do_thumb_abort
	tst	r3, #PSR_T_BIT
	beq	not_thumb
	ldrh	r3, [r2]			@ Read aborted Thumb instruction
	and	r3, r3, # 0xfe00		@ Mask opcode field
	cmp	r3, # 0x5600			@ Is it ldrsb?
	orreq	r3, r3, #1 << 11		@ Set L-bit if yes
	tst	r3, #1 << 11			@ L = 0 -> write
	orreq	r1, r1, #1 << 11		@ yes.
	mov	pc, lr
not_thumb:
	.endm

/*
 * We check for the following insturction encoding for LDRD.
 *
 * [27:25] == 0
 *   [7:4] == 1101
 *    [20] == 0
 */
 	.macro	do_ldrd_abort
 	tst	r3, #0x0e000000			@ [27:25] == 0
	bne	not_ldrd
	and	r2, r3, #0x000000f0		@ [7:4] == 1101
	cmp	r2, #0x000000d0
	bne	not_ldrd
	tst	r3, #1 << 20			@ [20] == 0
	moveq	pc, lr
not_ldrd:
	.endm

