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


#if __LINUX_ARM_ARCH__ >= 6 && defined(CONFIG_CPU_32v6K)
	.macro	bitop, instr
	mov	r2, #1
	and	r3, r0, #7		@ Get bit offset
	add	r1, r1, r0, lsr #3	@ Get byte offset
	mov	r3, r2, lsl r3
1:	ldrexb	r2, [r1]
	\instr	r2, r2, r3
	strexb	r0, r2, [r1]
	cmp	r0, #0
	bne	1b
	mov	pc, lr
	.endm

	.macro	testop, instr, store
	and	r3, r0, #7		@ Get bit offset
	mov	r2, #1
	add	r1, r1, r0, lsr #3	@ Get byte offset
	mov	r3, r2, lsl r3		@ create mask
1:	ldrexb	r2, [r1]
	ands	r0, r2, r3		@ save old value of bit
	\instr	r2, r2, r3			@ toggle bit
	strexb	ip, r2, [r1]
	cmp	ip, #0
	bne	1b
	cmp	r0, #0
	movne	r0, #1
2:	mov	pc, lr
	.endm
#else
	.macro	bitop, instr
	and	r2, r0, #7
	mov	r3, #1
	mov	r3, r3, lsl r2
	save_and_disable_irqs ip
	ldrb	r2, [r1, r0, lsr #3]
	\instr	r2, r2, r3
	strb	r2, [r1, r0, lsr #3]
	restore_irqs ip
	mov	pc, lr
	.endm

/**
 * testop - implement a test_and_xxx_bit operation.
 * @instr: operational instruction
 * @store: store instruction
 *
 * Note: we can trivially conditionalise the store instruction
 * to avoid dirtying the data cache.
 */
	.macro	testop, instr, store
	add	r1, r1, r0, lsr #3
	and	r3, r0, #7
	mov	r0, #1
	save_and_disable_irqs ip
	ldrb	r2, [r1]
	tst	r2, r0, lsl r3
	\instr	r2, r2, r0, lsl r3
	\store	r2, [r1]
	restore_irqs ip
	moveq	r0, #0
	mov	pc, lr
	.endm
#endif
