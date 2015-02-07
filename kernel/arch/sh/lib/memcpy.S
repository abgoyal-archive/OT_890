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

/* $Id: memcpy.S,v 1.3 2001/07/27 11:50:52 gniibe Exp $
 *
 * "memcpy" implementation of SuperH
 *
 * Copyright (C) 1999  Niibe Yutaka
 *
 */

/*
 * void *memcpy(void *dst, const void *src, size_t n);
 * No overlap between the memory of DST and of SRC are assumed.
 */

#include <linux/linkage.h>
ENTRY(memcpy)
	tst	r6,r6
	bt/s	9f		! if n=0, do nothing
	 mov	r4,r0
	sub	r4,r5		! From here, r5 has the distance to r0
	add	r6,r0		! From here, r0 points the end of copying point
	mov	#12,r1
	cmp/gt	r6,r1
	bt/s	7f		! if it's too small, copy a byte at once
	 add	#-1,r5
	add	#1,r5
	!			From here, r6 is free
	!
	!      r4   -->  [ ...  ] DST             [ ...  ] SRC
	!	         [ ...  ]                 [ ...  ]
	!	           :                        :
	!      r0   -->  [ ...  ]       r0+r5 --> [ ...  ]
	!
	!
	mov	r5,r1
	mov	#3,r2
	and	r2,r1
	shll2	r1
	mov	r0,r3		! Save the value on R0 to R3
	mova	jmptable,r0
	add	r1,r0
	mov.l	@r0,r1
	jmp	@r1
	 mov	r3,r0		! and back to R0
	.balign	4
jmptable:
	.long	case0
	.long	case1
	.long	case2
	.long	case3

	! copy a byte at once
7:	mov	r4,r2
	add	#1,r2
8:
	cmp/hi	r2,r0
	mov.b	@(r0,r5),r1
	bt/s	8b			! while (r0>r2)
	 mov.b	r1,@-r0
9:
	rts
	 nop

case0:
	!
	!	GHIJ KLMN OPQR -->  GHIJ KLMN OPQR
	!
	! First, align to long word boundary
	mov	r0,r3
	and	r2,r3
	tst	r3,r3
	bt/s	2f
	 add	#-4,r5
	add	#3,r5
1:	dt	r3
	mov.b	@(r0,r5),r1
	bf/s	1b
	 mov.b	r1,@-r0
	!
	add	#-3,r5
2:	! Second, copy a long word at once
	mov	r4,r2
	add	#7,r2
3:	mov.l	@(r0,r5),r1
	cmp/hi	r2,r0
	bt/s	3b
	 mov.l	r1,@-r0
	!
	! Third, copy a byte at once, if necessary
	cmp/eq	r4,r0
	bt/s	9b
	 add	#3,r5
	bra	8b
	 add	#-6,r2

case1:
	!
	!	GHIJ KLMN OPQR -->  ...G HIJK LMNO PQR.
	!
	! First, align to long word boundary
	mov	r0,r3
	and	r2,r3
	tst	r3,r3
	bt/s	2f
	 add	#-1,r5
1:	dt	r3
	mov.b	@(r0,r5),r1
	bf/s	1b
	 mov.b	r1,@-r0
	!
2:	! Second, read a long word and write a long word at once
	mov.l	@(r0,r5),r1
	add	#-4,r5
	mov	r4,r2
	add	#7,r2
	!
#ifdef __LITTLE_ENDIAN__
3:	mov	r1,r3		! RQPO
	shll16	r3
	shll8	r3		! Oxxx
	mov.l	@(r0,r5),r1	! NMLK
	mov	r1,r6
	shlr8	r6		! xNML
	or	r6,r3		! ONML
	cmp/hi	r2,r0
	bt/s	3b
	 mov.l	r3,@-r0
#else
3:	mov	r1,r3		! OPQR
	shlr16	r3
	shlr8	r3		! xxxO
	mov.l	@(r0,r5),r1	! KLMN
	mov	r1,r6
	shll8	r6		! LMNx
	or	r6,r3		! LMNO
	cmp/hi	r2,r0
	bt/s	3b
	 mov.l	r3,@-r0
#endif
	!
	! Third, copy a byte at once, if necessary
	cmp/eq	r4,r0
	bt/s	9b
	 add	#4,r5
	bra	8b
	 add	#-6,r2

case2:
	!
	!	GHIJ KLMN OPQR -->  ..GH IJKL MNOP QR..
	!
	! First, align to word boundary
	tst	#1,r0
	bt/s	2f
	 add	#-1,r5
	mov.b	@(r0,r5),r1
	mov.b	r1,@-r0
	!
2:	! Second, read a word and write a word at once
	add	#-1,r5
	mov	r4,r2
	add	#3,r2
	!
3:	mov.w	@(r0,r5),r1
	cmp/hi	r2,r0
	bt/s	3b
	 mov.w	r1,@-r0
	!
	! Third, copy a byte at once, if necessary
	cmp/eq	r4,r0
	bt/s	9b
	 add	#1,r5
	mov.b	@(r0,r5),r1
	rts
	 mov.b	r1,@-r0

case3:
	!
	!	GHIJ KLMN OPQR -->  .GHI JKLM NOPQ R...
	!
	! First, align to long word boundary
	mov	r0,r3
	and	r2,r3
	tst	r3,r3
	bt/s	2f
	 add	#-1,r5
1:	dt	r3
	mov.b	@(r0,r5),r1
	bf/s	1b
	 mov.b	r1,@-r0
	!
2:	! Second, read a long word and write a long word at once
	add	#-2,r5
	mov.l	@(r0,r5),r1
	add	#-4,r5
	mov	r4,r2
	add	#7,r2
	!
#ifdef __LITTLE_ENDIAN__
3:	mov	r1,r3		! RQPO
	shll8	r3		! QPOx
	mov.l	@(r0,r5),r1	! NMLK
	mov	r1,r6
	shlr16	r6
	shlr8	r6		! xxxN
	or	r6,r3		! QPON
	cmp/hi	r2,r0
	bt/s	3b
	 mov.l	r3,@-r0
#else
3:	mov	r1,r3		! OPQR
	shlr8	r3		! xOPQ
	mov.l	@(r0,r5),r1	! KLMN
	mov	r1,r6
	shll16	r6
	shll8	r6		! Nxxx
	or	r6,r3		! NOPQ
	cmp/hi	r2,r0
	bt/s	3b
	 mov.l	r3,@-r0
#endif
	!
	! Third, copy a byte at once, if necessary
	cmp/eq	r4,r0
	bt/s	9b
	 add	#6,r5
	bra	8b
	 add	#-6,r2
