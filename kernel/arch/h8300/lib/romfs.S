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

/* romfs move to __ebss */

#include <asm/linkage.h>

#if defined(__H8300H__) 
	.h8300h
#endif
#if defined(__H8300S__) 
	.h8300s
#endif

#define BLKOFFSET 512

	.text
.globl	__move_romfs
_romfs_sig_len = 8

__move_romfs:	
	mov.l	#__sbss,er0
	mov.l	#_romfs_sig,er1
	mov.b	#_romfs_sig_len,r3l
1:					/* check romfs image */
	mov.b	@er0+,r2l
	mov.b	@er1+,r2h
	cmp.b	r2l,r2h
	bne	2f
	dec.b	r3l
	bne	1b

	/* find romfs image */
	mov.l	@__sbss+8,er0		/* romfs length(be) */
	mov.l	#__sbss,er1
	add.l	er0,er1			/* romfs image end */
	mov.l	#__ebss,er2
	add.l	er0,er2			/* distination address */
#if defined(CONFIG_INTELFLASH)
	add.l	#BLKOFFSET,er2
#endif
	adds	#2,er0
	adds	#1,er0
	shlr	er0
	shlr	er0			/* transfer length */
1:
	mov.l	@er1,er3		/* copy image */
	mov.l	er3,@er2
	subs	#4,er1
	subs	#4,er2
	dec.l	#1,er0
	bpl	1b
2:
	rts

	.section	.rodata
_romfs_sig:	
	.ascii	"-rom1fs-"

	.end
