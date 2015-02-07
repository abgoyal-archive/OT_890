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

#ifndef _SPARC64_VISASM_H
#define _SPARC64_VISASM_H

/* visasm.h:  FPU saving macros for VIS routines
 *
 * Copyright (C) 1998 Jakub Jelinek (jj@ultra.linux.cz)
 */

#include <asm/pstate.h>
#include <asm/ptrace.h>

/* Clobbers %o5, %g1, %g2, %g3, %g7, %icc, %xcc */

#define VISEntry					\
	rd		%fprs, %o5;			\
	andcc		%o5, (FPRS_FEF|FPRS_DU), %g0;	\
	be,pt		%icc, 297f;			\
	 sethi		%hi(297f), %g7;			\
	sethi		%hi(VISenter), %g1;		\
	jmpl		%g1 + %lo(VISenter), %g0;	\
	 or		%g7, %lo(297f), %g7;		\
297:	wr		%g0, FPRS_FEF, %fprs;		\

#define VISExit						\
	wr		%g0, 0, %fprs;

/* Clobbers %o5, %g1, %g2, %g3, %g7, %icc, %xcc.
 * Must preserve %o5 between VISEntryHalf and VISExitHalf */

#define VISEntryHalf					\
	rd		%fprs, %o5;			\
	andcc		%o5, FPRS_FEF, %g0;		\
	be,pt		%icc, 297f;			\
	 sethi		%hi(298f), %g7;			\
	sethi		%hi(VISenterhalf), %g1;		\
	jmpl		%g1 + %lo(VISenterhalf), %g0;	\
	 or		%g7, %lo(298f), %g7;		\
	clr		%o5;				\
297:	wr		%o5, FPRS_FEF, %fprs;		\
298:

#define VISExitHalf					\
	wr		%o5, 0, %fprs;

#ifndef __ASSEMBLY__
static inline void save_and_clear_fpu(void) {
	__asm__ __volatile__ (
"		rd %%fprs, %%o5\n"
"		andcc %%o5, %0, %%g0\n"
"		be,pt %%icc, 299f\n"
"		 sethi %%hi(298f), %%g7\n"
"		sethi %%hi(VISenter), %%g1\n"
"		jmpl %%g1 + %%lo(VISenter), %%g0\n"
"		 or %%g7, %%lo(298f), %%g7\n"
"	298:	wr %%g0, 0, %%fprs\n"
"	299:\n"
"		" : : "i" (FPRS_FEF|FPRS_DU) :
		"o5", "g1", "g2", "g3", "g7", "cc");
}
extern int vis_emul(struct pt_regs *, unsigned int);
#endif

#endif /* _SPARC64_ASI_H */
