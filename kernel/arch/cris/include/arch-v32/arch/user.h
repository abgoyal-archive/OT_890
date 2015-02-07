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

#ifndef _ASM_CRIS_ARCH_USER_H
#define _ASM_CRIS_ARCH_USER_H

/* User-mode register used for core dumps. */

struct user_regs_struct {
	unsigned long r0;	/* General registers. */
	unsigned long r1;
	unsigned long r2;
	unsigned long r3;
	unsigned long r4;
	unsigned long r5;
	unsigned long r6;
	unsigned long r7;
	unsigned long r8;
	unsigned long r9;
	unsigned long r10;
	unsigned long r11;
	unsigned long r12;
	unsigned long r13;
	unsigned long sp;	/* R14, Stack pointer. */
	unsigned long acr;	/* R15, Address calculation register. */
	unsigned long bz;	/* P0, Constant zero (8-bits). */
	unsigned long vr;	/* P1, Version register (8-bits). */
	unsigned long pid;	/* P2, Process ID (8-bits). */
	unsigned long srs;	/* P3, Support register select (8-bits). */
	unsigned long wz;	/* P4, Constant zero (16-bits). */
	unsigned long exs;	/* P5, Exception status. */
	unsigned long eda;	/* P6, Exception data address. */
	unsigned long mof;	/* P7, Multiply overflow regiter. */
	unsigned long dz;	/* P8, Constant zero (32-bits). */
	unsigned long ebp;	/* P9, Exception base pointer. */
	unsigned long erp;	/* P10, Exception return pointer. */
	unsigned long srp;	/* P11, Subroutine return pointer. */
	unsigned long nrp;	/* P12, NMI return pointer. */
	unsigned long ccs;	/* P13, Condition code stack. */
	unsigned long usp;	/* P14, User mode stack pointer. */
	unsigned long spc;	/* P15, Single step PC. */
};

#endif /* _ASM_CRIS_ARCH_USER_H */
