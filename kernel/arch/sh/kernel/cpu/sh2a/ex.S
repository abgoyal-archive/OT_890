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
 * arch/sh/kernel/cpu/sh2a/ex.S
 *
 * The SH-2A exception vector table
 *
 * Copyright (C) 2008 Yoshinori Sato
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/linkage.h>

!
! convert Exception Vector to Exception Number
!

! exception no 0 to 255
exception_entry0:
no	=	0
	.rept	256
	mov.l	r1,@-sp
	bra	exception_trampoline0
	mov	#no,r1
no	=	no + 1
	.endr
exception_trampoline0:
	mov.l	r0,@-sp
	mov.l	1f,r0
	extu.b	r1,r1
	jmp	@r0
	  extu.w	r1,r1
	 
	.align	2
1:	.long	exception_handler

! exception no 256 to 511
exception_entry1:
no	=	0
	.rept	256
	mov.l	r1,@-sp
	bra	exception_trampoline1
	mov	#no,r1
no	=	no + 1
	.endr
exception_trampoline1:
	mov.l	r0,@-sp
	extu.b	r1,r1
	movi20	#0x100,r0
	add	r0,r1
	mov.l	1f,r0
	jmp	@r0
	  extu.w	r1,r1
	
	.align	2
1:	.long	exception_handler

	!
! Exception Vector Base
!
	.align	2
ENTRY(vbr_base)
vector	=	0
	.rept	256
	.long	exception_entry0 + vector * 6
vector	=	vector + 1
	.endr
	.rept	256
	.long	exception_entry1 + vector * 6
vector	=	vector + 1
	.endr
