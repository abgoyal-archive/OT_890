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

#ifndef _ASM_M32R_SIGCONTEXT_H
#define _ASM_M32R_SIGCONTEXT_H

struct sigcontext {
	/* CPU registers */
	/* Saved main processor registers. */
	unsigned long sc_r4;
	unsigned long sc_r5;
	unsigned long sc_r6;
	struct pt_regs *sc_pt_regs;
	unsigned long sc_r0;
	unsigned long sc_r1;
	unsigned long sc_r2;
	unsigned long sc_r3;
	unsigned long sc_r7;
	unsigned long sc_r8;
	unsigned long sc_r9;
	unsigned long sc_r10;
	unsigned long sc_r11;
	unsigned long sc_r12;

	/* Saved main processor status and miscellaneous context registers. */
	unsigned long sc_acc0h;
	unsigned long sc_acc0l;
	unsigned long sc_acc1h;	/* ISA_DSP_LEVEL2 only */
	unsigned long sc_acc1l;	/* ISA_DSP_LEVEL2 only */
	unsigned long sc_psw;
	unsigned long sc_bpc;		/* saved PC for TRAP syscalls */
	unsigned long sc_bbpsw;
	unsigned long sc_bbpc;
	unsigned long sc_spu;		/* saved user stack */
	unsigned long sc_fp;
	unsigned long sc_lr;		/* saved PC for JL syscalls */
	unsigned long sc_spi;		/* saved kernel stack */

	unsigned long	oldmask;
};

#endif  /* _ASM_M32R_SIGCONTEXT_H */
