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
 * Just like strncpy() except that if a fault occurs during copying,
 * -EFAULT is returned.
 *
 * Inputs:
 *	in0:	address of destination buffer
 *	in1:	address of string to be copied
 *	in2:	length of buffer in bytes
 * Outputs:
 *	r8:	-EFAULT in case of fault or number of bytes copied if no fault
 *
 * Copyright (C) 1998-2001 Hewlett-Packard Co
 * Copyright (C) 1998-2001 David Mosberger-Tang <davidm@hpl.hp.com>
 *
 * 00/03/06 D. Mosberger Fixed to return proper return value (bug found by
 *			 by Andreas Schwab <schwab@suse.de>).
 */

#include <asm/asmmacro.h>

GLOBAL_ENTRY(__strncpy_from_user)
	alloc r2=ar.pfs,3,0,0,0
	mov r8=0
	mov r9=in1
	;;
	add r10=in1,in2
	cmp.eq p6,p0=r0,in2
(p6)	br.ret.spnt.many rp

	// XXX braindead copy loop---this needs to be optimized
.Loop1:
	EX(.Lexit, ld1 r8=[in1],1)
	;;
	EX(.Lexit, st1 [in0]=r8,1)
	cmp.ne p6,p7=r8,r0
	;;
(p6)	cmp.ne.unc p8,p0=in1,r10
(p8)	br.cond.dpnt.few .Loop1
	;;
(p6)	mov r8=in2		// buffer filled up---return buffer length
(p7)	sub r8=in1,r9,1		// return string length (excluding NUL character)
[.Lexit:]
	br.ret.sptk.many rp
END(__strncpy_from_user)
