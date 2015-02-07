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
 *  linux/arch/m32r/mm/page.S
 *
 *  Clear/Copy page with CPU
 *
 *  Copyright (C) 2004  The Free Software Initiative of Japan
 *
 *  Written by Niibe Yutaka
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 */
	.text
	.global	copy_page
	/*
	 * copy_page (to, from)
	 *
	 * PAGE_SIZE = 4096-byte
	 * Cache line = 16-byte
	 * 16 * 256
	 */
	.align	4
copy_page:
	ldi	r2, #255
	ld	r3, @r0		/* cache line allocate */
	ld	r4, @r1+
	ld	r5, @r1+
	ld	r6, @r1+
	ld	r7, @r1+
	.fillinsn
0:
	st	r4, @r0
	st	r5, @+r0
	st	r6, @+r0
	st	r7, @+r0
	ld	r4, @r1+
	addi	r0, #4
	ld	r5, @r1+
	ld	r6, @r1+
	ld	r7, @r1+
	ld	r3, @r0		/* cache line allocate */
	addi	r2, #-1
	bnez	r2, 0b

	st	r4, @r0
	st	r5, @+r0
	st	r6, @+r0
	st	r7, @+r0
	jmp	r14

	.text
	.global	clear_page
	/*
	 * clear_page (to)
	 *
	 * PAGE_SIZE = 4096-byte
	 * Cache line = 16-byte
	 * 16 * 256
	 */
	.align	4
clear_page:
	ldi	r2, #255
	ldi	r4, #0
	ld	r3, @r0		/* cache line allocate */
	.fillinsn
0:
	st	r4, @r0
	st	r4, @+r0
	st	r4, @+r0
	st	r4, @+r0
	addi	r0, #4
	ld	r3, @r0		/* cache line allocate */
	addi	r2, #-1
	bnez	r2, 0b

	st	r4, @r0
	st	r4, @+r0
	st	r4, @+r0
	st	r4, @+r0
	jmp	r14
