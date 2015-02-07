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

/* NG2copy_to_user.S: Niagara-2 optimized copy to userspace.
 *
 * Copyright (C) 2007 David S. Miller (davem@davemloft.net)
 */

#define EX_ST(x)		\
98:	x;			\
	.section __ex_table,"a";\
	.align 4;		\
	.word 98b, __retl_one_asi;\
	.text;			\
	.align 4;

#ifndef ASI_AIUS
#define ASI_AIUS	0x11
#endif

#ifndef ASI_BLK_AIUS_4V
#define ASI_BLK_AIUS_4V	0x17
#endif

#ifndef ASI_BLK_INIT_QUAD_LDD_AIUS
#define ASI_BLK_INIT_QUAD_LDD_AIUS 0x23
#endif

#define FUNC_NAME		NG2copy_to_user
#define STORE(type,src,addr)	type##a src, [addr] ASI_AIUS
#define STORE_ASI		ASI_BLK_INIT_QUAD_LDD_AIUS
#define STORE_BLK(src,addr)	stda src, [addr] ASI_BLK_AIUS_4V
#define EX_RETVAL(x)		0

#ifdef __KERNEL__
	/* Writing to %asi is _expensive_ so we hardcode it.
	 * Reading %asi to check for KERNEL_DS is comparatively
	 * cheap.
	 */
#define PREAMBLE					\
	rd		%asi, %g1;			\
	cmp		%g1, ASI_AIUS;			\
	bne,pn		%icc, ___copy_in_user;		\
	 nop
#endif

#include "NG2memcpy.S"
