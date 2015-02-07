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
 * relocate_kernel.S - put the kernel image in place to boot
 * 2005.9.17 kogiidena@eggplant.ddo.jp
 *
 * LANDISK/sh4 is supported. Maybe, SH archtecture works well.
 *
 * This source code is licensed under the GNU General Public License,
 * Version 2.  See the file COPYING for more details.
 */
#include <linux/linkage.h>
#include <asm/addrspace.h>
#include <asm/page.h>

		.globl relocate_new_kernel
relocate_new_kernel:
	/* r4 = indirection_page   */
	/* r5 = reboot_code_buffer */
	/* r6 = start_address      */
	/* r7 = vbr_reg            */

	mov.l	10f,r8	  /* PAGE_SIZE */
	mov.l	11f,r9    /* P2SEG */

	/*  stack setting */
	add	r8,r5
	mov	r5,r15

	bra	1f
	mov	r4,r0	  /* cmd = indirection_page */
0:
	mov.l	@r4+,r0	  /* cmd = *ind++ */

1:	/* addr = (cmd | P2SEG) & 0xfffffff0 */
	mov	r0,r2
	or	r9,r2
	mov	#-16,r1
	and	r1,r2

	/* if(cmd & IND_DESTINATION) dst = addr  */
	tst	#1,r0
	bt	2f
	bra	0b
	mov	r2,r5

2:	/* else if(cmd & IND_INDIRECTION) ind = addr  */
	tst	#2,r0
	bt	3f
	bra	0b
	mov	r2,r4

3:	/* else if(cmd & IND_DONE) goto 6  */
	tst	#4,r0
	bt	4f
	bra	6f
	nop

4:	/* else if(cmd & IND_SOURCE) memcpy(dst,addr,PAGE_SIZE) */
	tst	#8,r0
	bt	0b

	mov	r8,r3
	shlr2	r3
	shlr2	r3
5:
	dt	r3
	mov.l	@r2+,r1   /*  16n+0 */
	mov.l	r1,@r5
	add	#4,r5
	mov.l	@r2+,r1	  /*  16n+4 */
	mov.l	r1,@r5
	add	#4,r5
	mov.l	@r2+,r1   /*  16n+8 */
	mov.l	r1,@r5
	add	#4,r5
	mov.l	@r2+,r1   /*  16n+12 */
	mov.l	r1,@r5
	add	#4,r5
	bf	5b

	bra	0b
	nop
6:
#ifdef CONFIG_SH_STANDARD_BIOS
	ldc   r7, vbr
#endif
	jmp @r6
	nop

	.align 2
10:
	.long	PAGE_SIZE
11:
	.long	P2SEG

relocate_new_kernel_end:

	.globl relocate_new_kernel_size
relocate_new_kernel_size:
	.long relocate_new_kernel_end - relocate_new_kernel
