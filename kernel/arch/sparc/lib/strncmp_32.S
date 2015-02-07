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
 * strncmp.S: Hand optimized Sparc assembly of GCC output from GNU libc
 *            generic strncmp routine.
 */

	.text
	.align 4
	.global __strncmp, strncmp
__strncmp:
strncmp:
	mov	%o0, %g3
	mov	0, %o3

	cmp	%o2, 3
	ble	7f
	 mov	0, %g2

	sra	%o2, 2, %o4
	ldub	[%g3], %o3

0:
	ldub	[%o1], %g2
	add	%g3, 1, %g3
	and	%o3, 0xff, %o0

	cmp	%o0, 0
	be	8f
	 add	%o1, 1, %o1

	cmp	%o0, %g2
	be,a	1f
	 ldub	[%g3], %o3

	retl
	 sub	%o0, %g2, %o0

1:
	ldub	[%o1], %g2
	add	%g3,1, %g3
	and	%o3, 0xff, %o0

	cmp	%o0, 0
	be	8f
	 add	%o1, 1, %o1

	cmp	%o0, %g2
	be,a	1f
	 ldub	[%g3], %o3

	retl
	 sub	%o0, %g2, %o0

1:
	ldub	[%o1], %g2
	add	%g3, 1, %g3
	and	%o3, 0xff, %o0

	cmp	%o0, 0
	be	8f
	 add	%o1, 1, %o1

	cmp	%o0, %g2
	be,a	1f
	 ldub	[%g3], %o3

	retl
	 sub	%o0, %g2, %o0

1:
	ldub	[%o1], %g2
	add	%g3, 1, %g3
	and	%o3, 0xff, %o0

	cmp	%o0, 0
	be	8f
	 add	%o1, 1, %o1

	cmp	%o0, %g2
	be	1f
	 add	%o4, -1, %o4

	retl
	 sub	%o0, %g2, %o0

1:

	cmp	%o4, 0
	bg,a	0b
	 ldub	[%g3], %o3

	b	7f
	 and	%o2, 3, %o2

9:
	ldub	[%o1], %g2
	add	%g3, 1, %g3
	and	%o3, 0xff, %o0

	cmp	%o0, 0
	be	8f
	 add	%o1, 1, %o1

	cmp	%o0, %g2
	be	7f
	 add	%o2, -1, %o2

8:
	retl
	 sub	%o0, %g2, %o0

7:
	cmp	%o2, 0
	bg,a	9b
	 ldub	[%g3], %o3

	and	%g2, 0xff, %o0
	retl
	 sub	%o3, %o0, %o0
