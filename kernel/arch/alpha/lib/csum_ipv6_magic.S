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
 * arch/alpha/lib/csum_ipv6_magic.S
 * Contributed by Richard Henderson <rth@tamu.edu>
 *
 * unsigned short csum_ipv6_magic(struct in6_addr *saddr,
 *                                struct in6_addr *daddr,
 *                                __u32 len,
 *                                unsigned short proto,
 *                                unsigned int csum);
 *
 * Misalignment handling (which costs 16 instructions / 8 cycles)
 * added by Ivan Kokshaysky <ink@jurassic.park.msu.ru>
 */

	.globl csum_ipv6_magic
	.align 4
	.ent csum_ipv6_magic
	.frame $30,0,$26,0
csum_ipv6_magic:
	.prologue 0

	ldq_u	$0,0($16)	# e0    : load src & dst addr words
	zapnot	$20,15,$20	# .. e1 : zero extend incoming csum
	extqh	$18,1,$4	# e0    : byte swap len & proto while we wait
	ldq_u	$21,7($16)	# .. e1 : handle misalignment

	extbl	$18,1,$5	# e0	:
	ldq_u	$1,8($16)	# .. e1 :
	extbl	$18,2,$6	# e0 	:
	ldq_u	$22,15($16)	# .. e1 :

	extbl	$18,3,$18	# e0	:
	ldq_u	$2,0($17)	# .. e1 :
	sra	$4,32,$4	# e0	:
	ldq_u	$23,7($17)	# .. e1 :

	extql	$0,$16,$0	# e0	:
	ldq_u	$3,8($17)	# .. e1 :
	extqh	$21,$16,$21	# e0	:
	ldq_u	$24,15($17)	# .. e1 :

	sll	$5,16,$5	# e0	:
	or	$0,$21,$0	# .. e1 : 1st src word complete
	extql	$1,$16,$1	# e0	:
	addq	$20,$0,$20	# .. e1 : begin summing the words

	extqh	$22,$16,$22	# e0	:
	cmpult	$20,$0,$0	# .. e1 :
	sll	$6,8,$6		# e0	:
	or	$1,$22,$1	# .. e1 : 2nd src word complete

	extql	$2,$17,$2	# e0	:
	or	$4,$18,$18	# .. e1 :
	extqh	$23,$17,$23	# e0	:
	or	$5,$6,$5	# .. e1 :

	extql	$3,$17,$3	# e0	:
	or	$2,$23,$2	# .. e1 : 1st dst word complete
	extqh	$24,$17,$24	# e0	:
	or	$18,$5,$18	# .. e1 : len complete

	extwh	$19,7,$7	# e0    :
	or	$3,$24,$3	# .. e1 : 2nd dst word complete
	extbl	$19,1,$19	# e0    :
	addq	$20,$1,$20	# .. e1 :

	or	$19,$7,$19	# e0    :
	cmpult	$20,$1,$1	# .. e1 :
	sll	$19,48,$19	# e0    :
	nop			# .. e0 :

	sra	$19,32,$19	# e0    : proto complete
	addq	$20,$2,$20	# .. e1 :
	cmpult	$20,$2,$2	# e0    :
	addq	$20,$3,$20	# .. e1 :

	cmpult	$20,$3,$3	# e0    :
	addq	$20,$18,$20	# .. e1 :
	cmpult	$20,$18,$18	# e0    :
	addq	$20,$19,$20	# .. e1 :

	cmpult	$20,$19,$19	# e0    :
	addq	$0,$1,$0	# .. e1 : merge the carries back into the csum
	addq	$2,$3,$2	# e0    :
	addq	$18,$19,$18	# .. e1 :

	addq	$0,$2,$0	# e0    :
	addq	$20,$18,$20	# .. e1 :
	addq	$0,$20,$0	# e0    :
	unop			#       :

	extwl	$0,2,$2		# e0    : begin folding the 64-bit value
	zapnot	$0,3,$3		# .. e1 :
	extwl	$0,4,$1		# e0    :
	addq	$2,$3,$3	# .. e1 :

	extwl	$0,6,$0		# e0    :
	addq	$3,$1,$3	# .. e1 :
	addq	$0,$3,$0	# e0    :
	unop			#       :

	extwl	$0,2,$1		# e0    : fold 18-bit value
	zapnot	$0,3,$0		# .. e1 :
	addq	$0,$1,$0	# e0    :
	unop			#       :

	extwl	$0,2,$1		# e0    : fold 17-bit value
	zapnot	$0,3,$0		# .. e1 :
	addq	$0,$1,$0	# e0    :
	not	$0,$0		# .. e1 : and complement.

	zapnot	$0,3,$0		# e0    :
	ret			# .. e1 :

	.end csum_ipv6_magic
