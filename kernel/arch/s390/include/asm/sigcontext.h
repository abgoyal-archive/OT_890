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
 *  include/asm-s390/sigcontext.h
 *
 *  S390 version
 *    Copyright (C) 1999,2000 IBM Deutschland Entwicklung GmbH, IBM Corporation
 */

#ifndef _ASM_S390_SIGCONTEXT_H
#define _ASM_S390_SIGCONTEXT_H

#include <linux/compiler.h>

#define __NUM_GPRS 16
#define __NUM_FPRS 16
#define __NUM_ACRS 16

#ifndef __s390x__

/* Has to be at least _NSIG_WORDS from asm/signal.h */
#define _SIGCONTEXT_NSIG	64
#define _SIGCONTEXT_NSIG_BPW	32
/* Size of stack frame allocated when calling signal handler. */
#define __SIGNAL_FRAMESIZE	96

#else /* __s390x__ */

/* Has to be at least _NSIG_WORDS from asm/signal.h */
#define _SIGCONTEXT_NSIG	64
#define _SIGCONTEXT_NSIG_BPW	64 
/* Size of stack frame allocated when calling signal handler. */
#define __SIGNAL_FRAMESIZE	160

#endif /* __s390x__ */

#define _SIGCONTEXT_NSIG_WORDS	(_SIGCONTEXT_NSIG / _SIGCONTEXT_NSIG_BPW)
#define _SIGMASK_COPY_SIZE	(sizeof(unsigned long)*_SIGCONTEXT_NSIG_WORDS)

typedef struct 
{
        unsigned long mask;
        unsigned long addr;
} __attribute__ ((aligned(8))) _psw_t;

typedef struct
{
	_psw_t psw;
	unsigned long gprs[__NUM_GPRS];
	unsigned int  acrs[__NUM_ACRS];
} _s390_regs_common;

typedef struct
{
	unsigned int fpc;
	double   fprs[__NUM_FPRS];
} _s390_fp_regs;

typedef struct
{
	_s390_regs_common regs;
	_s390_fp_regs     fpregs;
} _sigregs;

struct sigcontext
{
	unsigned long	oldmask[_SIGCONTEXT_NSIG_WORDS];
	_sigregs        __user *sregs;
};


#endif

