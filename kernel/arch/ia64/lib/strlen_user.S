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
 * Optimized version of the strlen_user() function
 *
 * Inputs:
 *	in0	address of buffer
 *
 * Outputs:
 *	ret0	0 in case of fault, strlen(buffer)+1 otherwise
 *
 * Copyright (C) 1998, 1999, 2001 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 *	Stephane Eranian <eranian@hpl.hp.com>
 *
 * 01/19/99 S.Eranian heavily enhanced version (see details below)
 * 09/24/99 S.Eranian added speculation recovery code
 */

#include <asm/asmmacro.h>

//
// int strlen_user(char *)
// ------------------------
// Returns:
//	- length of string + 1
//	- 0 in case an exception is raised
//
// This is an enhanced version of the basic strlen_user. it includes a
// combination of compute zero index (czx), parallel comparisons, speculative
// loads and loop unroll using rotating registers.
//
// General Ideas about the algorithm:
//	  The goal is to look at the string in chunks of 8 bytes.
//	  so we need to do a few extra checks at the beginning because the
//	  string may not be 8-byte aligned. In this case we load the 8byte
//	  quantity which includes the start of the string and mask the unused
//	  bytes with 0xff to avoid confusing czx.
//	  We use speculative loads and software pipelining to hide memory
//	  latency and do read ahead safely. This way we defer any exception.
//
//	  Because we don't want the kernel to be relying on particular
//	  settings of the DCR register, we provide recovery code in case
//	  speculation fails. The recovery code is going to "redo" the work using
//	  only normal loads. If we still get a fault then we return an
//	  error (ret0=0). Otherwise we return the strlen+1 as usual.
//	  The fact that speculation may fail can be caused, for instance, by
//	  the DCR.dm bit being set. In this case TLB misses are deferred, i.e.,
//	  a NaT bit will be set if the translation is not present. The normal
//	  load, on the other hand, will cause the translation to be inserted
//	  if the mapping exists.
//
//	  It should be noted that we execute recovery code only when we need
//	  to use the data that has been speculatively loaded: we don't execute
//	  recovery code on pure read ahead data.
//
// Remarks:
//	- the cmp r0,r0 is used as a fast way to initialize a predicate
//	  register to 1. This is required to make sure that we get the parallel
//	  compare correct.
//
//	- we don't use the epilogue counter to exit the loop but we need to set
//	  it to zero beforehand.
//
//	- after the loop we must test for Nat values because neither the
//	  czx nor cmp instruction raise a NaT consumption fault. We must be
//	  careful not to look too far for a Nat for which we don't care.
//	  For instance we don't need to look at a NaT in val2 if the zero byte
//	  was in val1.
//
//	- Clearly performance tuning is required.
//

#define saved_pfs	r11
#define	tmp		r10
#define base		r16
#define orig		r17
#define saved_pr	r18
#define src		r19
#define mask		r20
#define val		r21
#define val1		r22
#define val2		r23

GLOBAL_ENTRY(__strlen_user)
	.prologue
	.save ar.pfs, saved_pfs
	alloc saved_pfs=ar.pfs,11,0,0,8

	.rotr v[2], w[2]	// declares our 4 aliases

	extr.u tmp=in0,0,3	// tmp=least significant 3 bits
	mov orig=in0		// keep trackof initial byte address
	dep src=0,in0,0,3	// src=8byte-aligned in0 address
	.save pr, saved_pr
	mov saved_pr=pr		// preserve predicates (rotation)
	;;

	.body

	ld8.s v[1]=[src],8	// load the initial 8bytes (must speculate)
	shl tmp=tmp,3		// multiply by 8bits/byte
	mov mask=-1		// our mask
	;;
	ld8.s w[1]=[src],8	// load next 8 bytes in 2nd pipeline
	cmp.eq p6,p0=r0,r0	// sets p6 (required because of // cmp.and)
	sub tmp=64,tmp		// how many bits to shift our mask on the right
	;;
	shr.u	mask=mask,tmp	// zero enough bits to hold v[1] valuable part
	mov ar.ec=r0		// clear epilogue counter (saved in ar.pfs)
	;;
	add base=-16,src	// keep track of aligned base
	chk.s v[1], .recover	// if already NaT, then directly skip to recover
	or v[1]=v[1],mask	// now we have a safe initial byte pattern
	;;
1:
	ld8.s v[0]=[src],8	// speculatively load next
	czx1.r val1=v[1]	// search 0 byte from right
	czx1.r val2=w[1]	// search 0 byte from right following 8bytes
	;;
	ld8.s w[0]=[src],8	// speculatively load next to next
	cmp.eq.and p6,p0=8,val1	// p6 = p6 and val1==8
	cmp.eq.and p6,p0=8,val2	// p6 = p6 and mask==8
(p6)	br.wtop.dptk.few 1b	// loop until p6 == 0
	;;
	//
	// We must return try the recovery code iff
	// val1_is_nat || (val1==8 && val2_is_nat)
	//
	// XXX Fixme
	//	- there must be a better way of doing the test
	//
	cmp.eq  p8,p9=8,val1	// p6 = val1 had zero (disambiguate)
	tnat.nz p6,p7=val1	// test NaT on val1
(p6)	br.cond.spnt .recover	// jump to recovery if val1 is NaT
	;;
	//
	// if we come here p7 is true, i.e., initialized for // cmp
	//
	cmp.eq.and  p7,p0=8,val1// val1==8?
	tnat.nz.and p7,p0=val2	// test NaT if val2
(p7)	br.cond.spnt .recover	// jump to recovery if val2 is NaT
	;;
(p8)	mov val1=val2		// val2 contains the value
(p8)	adds src=-16,src	// correct position when 3 ahead
(p9)	adds src=-24,src	// correct position when 4 ahead
	;;
	sub ret0=src,orig	// distance from origin
	sub tmp=7,val1		// 7=8-1 because this strlen returns strlen+1
	mov pr=saved_pr,0xffffffffffff0000
	;;
	sub ret0=ret0,tmp	// length=now - back -1
	mov ar.pfs=saved_pfs	// because of ar.ec, restore no matter what
	br.ret.sptk.many rp	// end of normal execution

	//
	// Outlined recovery code when speculation failed
	//
	// This time we don't use speculation and rely on the normal exception
	// mechanism. that's why the loop is not as good as the previous one
	// because read ahead is not possible
	//
	// XXX Fixme
	//	- today we restart from the beginning of the string instead
	//	  of trying to continue where we left off.
	//
.recover:
	EX(.Lexit1, ld8 val=[base],8)	// load the initial bytes
	;;
	or val=val,mask			// remask first bytes
	cmp.eq p0,p6=r0,r0		// nullify first ld8 in loop
	;;
	//
	// ar.ec is still zero here
	//
2:
	EX(.Lexit1, (p6) ld8 val=[base],8)
	;;
	czx1.r val1=val		// search 0 byte from right
	;;
	cmp.eq p6,p0=8,val1	// val1==8 ?
(p6)	br.wtop.dptk.few 2b	// loop until p6 == 0
	;;
	sub ret0=base,orig	// distance from base
	sub tmp=7,val1		// 7=8-1 because this strlen returns strlen+1
	mov pr=saved_pr,0xffffffffffff0000
	;;
	sub ret0=ret0,tmp	// length=now - back -1
	mov ar.pfs=saved_pfs	// because of ar.ec, restore no matter what
	br.ret.sptk.many rp	// end of successful recovery code

	//
	// We failed even on the normal load (called from exception handler)
	//
.Lexit1:
	mov ret0=0
	mov pr=saved_pr,0xffffffffffff0000
	mov ar.pfs=saved_pfs	// because of ar.ec, restore no matter what
	br.ret.sptk.many rp
END(__strlen_user)
