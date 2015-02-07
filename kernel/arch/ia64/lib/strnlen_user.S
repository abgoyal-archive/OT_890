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
 * Returns 0 if exception before NUL or reaching the supplied limit (N),
 * a value greater than N if the string is longer than the limit, else
 * strlen.
 *
 * Inputs:
 *	in0:	address of buffer
 *	in1:	string length limit N
 * Outputs:
 *	r8:	0 in case of fault, strlen(buffer)+1 otherwise
 *
 * Copyright (C) 1999, 2001 David Mosberger-Tang <davidm@hpl.hp.com>
 */

#include <asm/asmmacro.h>

GLOBAL_ENTRY(__strnlen_user)
	.prologue
	alloc r2=ar.pfs,2,0,0,0
	.save ar.lc, r16
	mov r16=ar.lc			// preserve ar.lc

	.body

	add r3=-1,in1
	;;
	mov ar.lc=r3
	mov r9=0
	;;
	// XXX braindead strlen loop---this needs to be optimized
.Loop1:
	EXCLR(.Lexit, ld1 r8=[in0],1)
	add r9=1,r9
	;;
	cmp.eq p6,p0=r8,r0
(p6)	br.cond.dpnt .Lexit
	br.cloop.dptk.few .Loop1

	add r9=1,in1			// NUL not found---return N+1
	;;
.Lexit:
	mov r8=r9
	mov ar.lc=r16			// restore ar.lc
	br.ret.sptk.many rp
END(__strnlen_user)
