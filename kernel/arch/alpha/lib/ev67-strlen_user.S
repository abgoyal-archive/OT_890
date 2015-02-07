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
 * arch/alpha/lib/ev67-strlen_user.S
 * 21264 version contributed by Rick Gorton <rick.gorton@api-networks.com>
 *
 * Return the length of the string including the NULL terminator
 * (strlen+1) or zero if an error occurred.
 *
 * In places where it is critical to limit the processing time,
 * and the data is not trusted, strnlen_user() should be used.
 * It will return a value greater than its second argument if
 * that limit would be exceeded. This implementation is allowed
 * to access memory beyond the limit, but will not cross a page
 * boundary when doing so.
 *
 * Much of the information about 21264 scheduling/coding comes from:
 *      Compiler Writer's Guide for the Alpha 21264
 *      abbreviated as 'CWG' in other comments here
 *      ftp.digital.com/pub/Digital/info/semiconductor/literature/dsc-library.html
 * Scheduling notation:
 *      E       - either cluster
 *      U       - upper subcluster; U0 - subcluster U0; U1 - subcluster U1
 *      L       - lower subcluster; L0 - subcluster L0; L1 - subcluster L1
 * Try not to change the actual algorithm if possible for consistency.
 */

#include <asm/regdef.h>


/* Allow an exception for an insn; exit if we get one.  */
#define EX(x,y...)			\
	99: x,##y;			\
	.section __ex_table,"a";	\
	.long 99b - .;			\
	lda v0, $exception-99b(zero);	\
	.previous


	.set noreorder
	.set noat
	.text

	.globl __strlen_user
	.ent __strlen_user
	.frame sp, 0, ra

	.align 4
__strlen_user:
	ldah	a1, 32767(zero)	# do not use plain strlen_user() for strings
				# that might be almost 2 GB long; you should
				# be using strnlen_user() instead
	nop
	nop
	nop

	.globl __strnlen_user

	.align 4
__strnlen_user:
	.prologue 0
	EX( ldq_u t0, 0(a0) )	# L : load first quadword (a0 may be misaligned)
	lda     t1, -1(zero)	# E :

	insqh   t1, a0, t1	# U :
	andnot  a0, 7, v0	# E :
	or      t1, t0, t0	# E :
	subq	a0, 1, a0	# E : get our +1 for the return 

	cmpbge  zero, t0, t1	# E : t1 <- bitmask: bit i == 1 <==> i-th byte == 0
	subq	a1, 7, t2	# E :
	subq	a0, v0, t0	# E :
	bne     t1, $found	# U :

	addq	t2, t0, t2	# E :
	addq	a1, 1, a1	# E :
	nop			# E :
	nop			# E :

	.align 4
$loop:	ble	t2, $limit	# U :
	EX( ldq t0, 8(v0) )	# L :
	nop			# E :
	nop			# E :

	cmpbge  zero, t0, t1	# E :
	subq	t2, 8, t2	# E :
	addq    v0, 8, v0	# E : addr += 8
	beq     t1, $loop	# U :

$found: cttz	t1, t2		# U0 :
	addq	v0, t2, v0	# E :
	subq    v0, a0, v0	# E :
	ret			# L0 :

$exception:
	nop
	nop
	nop
	ret

	.align 4		# currently redundant
$limit:
	nop
	nop
	subq	a1, t2, v0
	ret

	.end __strlen_user
