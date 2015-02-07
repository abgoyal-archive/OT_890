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

#ifndef _LINUX_PRCTL_H
#define _LINUX_PRCTL_H

/* Values to pass as first argument to prctl() */

#define PR_SET_PDEATHSIG  1  /* Second arg is a signal */
#define PR_GET_PDEATHSIG  2  /* Second arg is a ptr to return the signal */

/* Get/set current->mm->dumpable */
#define PR_GET_DUMPABLE   3
#define PR_SET_DUMPABLE   4

/* Get/set unaligned access control bits (if meaningful) */
#define PR_GET_UNALIGN	  5
#define PR_SET_UNALIGN	  6
# define PR_UNALIGN_NOPRINT	1	/* silently fix up unaligned user accesses */
# define PR_UNALIGN_SIGBUS	2	/* generate SIGBUS on unaligned user access */

/* Get/set whether or not to drop capabilities on setuid() away from
 * uid 0 (as per security/commoncap.c) */
#define PR_GET_KEEPCAPS   7
#define PR_SET_KEEPCAPS   8

/* Get/set floating-point emulation control bits (if meaningful) */
#define PR_GET_FPEMU  9
#define PR_SET_FPEMU 10
# define PR_FPEMU_NOPRINT	1	/* silently emulate fp operations accesses */
# define PR_FPEMU_SIGFPE	2	/* don't emulate fp operations, send SIGFPE instead */

/* Get/set floating-point exception mode (if meaningful) */
#define PR_GET_FPEXC	11
#define PR_SET_FPEXC	12
# define PR_FP_EXC_SW_ENABLE	0x80	/* Use FPEXC for FP exception enables */
# define PR_FP_EXC_DIV		0x010000	/* floating point divide by zero */
# define PR_FP_EXC_OVF		0x020000	/* floating point overflow */
# define PR_FP_EXC_UND		0x040000	/* floating point underflow */
# define PR_FP_EXC_RES		0x080000	/* floating point inexact result */
# define PR_FP_EXC_INV		0x100000	/* floating point invalid operation */
# define PR_FP_EXC_DISABLED	0	/* FP exceptions disabled */
# define PR_FP_EXC_NONRECOV	1	/* async non-recoverable exc. mode */
# define PR_FP_EXC_ASYNC	2	/* async recoverable exception mode */
# define PR_FP_EXC_PRECISE	3	/* precise exception mode */

/* Get/set whether we use statistical process timing or accurate timestamp
 * based process timing */
#define PR_GET_TIMING   13
#define PR_SET_TIMING   14
# define PR_TIMING_STATISTICAL  0       /* Normal, traditional,
                                                   statistical process timing */
# define PR_TIMING_TIMESTAMP    1       /* Accurate timestamp based
                                                   process timing */

#define PR_SET_NAME    15		/* Set process name */
#define PR_GET_NAME    16		/* Get process name */

/* Get/set process endian */
#define PR_GET_ENDIAN	19
#define PR_SET_ENDIAN	20
# define PR_ENDIAN_BIG		0
# define PR_ENDIAN_LITTLE	1	/* True little endian mode */
# define PR_ENDIAN_PPC_LITTLE	2	/* "PowerPC" pseudo little endian */

/* Get/set process seccomp mode */
#define PR_GET_SECCOMP	21
#define PR_SET_SECCOMP	22

/* Get/set the capability bounding set (as per security/commoncap.c) */
#define PR_CAPBSET_READ 23
#define PR_CAPBSET_DROP 24

/* Get/set the process' ability to use the timestamp counter instruction */
#define PR_GET_TSC 25
#define PR_SET_TSC 26
# define PR_TSC_ENABLE		1	/* allow the use of the timestamp counter */
# define PR_TSC_SIGSEGV		2	/* throw a SIGSEGV instead of reading the TSC */

/* Get/set securebits (as per security/commoncap.c) */
#define PR_GET_SECUREBITS 27
#define PR_SET_SECUREBITS 28

/*
 * Get/set the timerslack as used by poll/select/nanosleep
 * A value of 0 means "use default"
 */
#define PR_SET_TIMERSLACK 29
#define PR_GET_TIMERSLACK 30

#endif /* _LINUX_PRCTL_H */
