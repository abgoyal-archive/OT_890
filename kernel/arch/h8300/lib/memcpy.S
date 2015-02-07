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

;;; memcpy.S

#include <asm/linkage.h>

#if defined(__H8300H__) 
	.h8300h
#endif
#if defined(__H8300S__) 
	.h8300s
#endif

	.text
.global SYMBOL_NAME(memcpy)

;;; void *memcpy(void *to, void *from, size_t n)
SYMBOL_NAME_LABEL(memcpy)
	mov.l	er2,er2
	bne	1f
	rts	
1:	
	;; address check
	bld	#0,r0l
	bxor	#0,r1l
	bcs	4f
	mov.l	er4,@-sp
	mov.l	er0,@-sp
	btst	#0,r0l
	beq	1f
	;; (aligned even) odd address
	mov.b	@er1,r3l
	mov.b	r3l,@er0
	adds	#1,er1
	adds	#1,er0
	dec.l	#1,er2
	beq	3f
1:	
	;; n < sizeof(unsigned long) check
	sub.l	er4,er4
	adds	#4,er4		; loop count check value
	cmp.l	er4,er2
	blo	2f
	;; unsigned long copy
1:	
	mov.l	@er1,er3
	mov.l	er3,@er0
	adds	#4,er0
	adds	#4,er1
	subs	#4,er2
	cmp.l	er4,er2
	bcc	1b	
	;; rest
2:	
	mov.l	er2,er2
	beq	3f
1:	
	mov.b	@er1,r3l
	mov.b	r3l,@er0
	adds	#1,er1
	adds	#1,er0
	dec.l	#1,er2
	bne	1b
3:
	mov.l	@sp+,er0
	mov.l	@sp+,er4
	rts

	;; odd <- even / even <- odd
4:	
	mov.l	er4,er3
	mov.l	er2,er4
	mov.l	er5,er2
	mov.l	er1,er5
	mov.l	er6,er1
	mov.l	er0,er6
1:
	eepmov.w
	mov.w	r4,r4
	bne	1b
	dec.w	#1,e4
	bpl	1b
	mov.l	er1,er6
	mov.l	er2,er5
	mov.l	er3,er4
	rts
