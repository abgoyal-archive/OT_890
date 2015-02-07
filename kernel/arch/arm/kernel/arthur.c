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
 *  linux/arch/arm/kernel/arthur.c
 *
 *  Copyright (C) 1998, 1999, 2000, 2001 Philip Blundell
 *
 * Arthur personality
 */

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/personality.h>
#include <linux/stddef.h>
#include <linux/signal.h>
#include <linux/init.h>
#include <linux/sched.h>

#include <asm/ptrace.h>

/* Arthur doesn't have many signals, and a lot of those that it does
   have don't map easily to any Linux equivalent.  Never mind.  */

#define ARTHUR_SIGABRT		1
#define ARTHUR_SIGFPE		2
#define ARTHUR_SIGILL		3
#define ARTHUR_SIGINT		4
#define ARTHUR_SIGSEGV		5
#define ARTHUR_SIGTERM		6
#define ARTHUR_SIGSTAK		7
#define ARTHUR_SIGUSR1		8
#define ARTHUR_SIGUSR2		9
#define ARTHUR_SIGOSERROR	10

static unsigned long arthur_to_linux_signals[32] = {
	0,	1,	2,	3,	4,	5,	6,	7,
	8,	9,	10,	11,	12,	13,	14,	15,
	16,	17,	18,	19,	20,	21,	22,	23,
	24,	25,	26,	27,	28,	29,	30,	31
};

static unsigned long linux_to_arthur_signals[32] = {
	0,		-1,		ARTHUR_SIGINT,	-1,
       	ARTHUR_SIGILL,	5,		ARTHUR_SIGABRT,	7,
	ARTHUR_SIGFPE,	9,		ARTHUR_SIGUSR1,	ARTHUR_SIGSEGV,	
	ARTHUR_SIGUSR2,	13,		14,		ARTHUR_SIGTERM,
	16,		17,		18,		19,
	20,		21,		22,		23,
	24,		25,		26,		27,
	28,		29,		30,		31
};

static void arthur_lcall7(int nr, struct pt_regs *regs)
{
	struct siginfo info;
	info.si_signo = SIGSWI;
	info.si_errno = nr;
	/* Bounce it to the emulator */
	send_sig_info(SIGSWI, &info, current);
}

static struct exec_domain arthur_exec_domain = {
	.name		= "Arthur",
	.handler	= arthur_lcall7,
	.pers_low	= PER_RISCOS,
	.pers_high	= PER_RISCOS,
	.signal_map	= arthur_to_linux_signals,
	.signal_invmap	= linux_to_arthur_signals,
	.module		= THIS_MODULE,
};

/*
 * We could do with some locking to stop Arthur being removed while
 * processes are using it.
 */

static int __init arthur_init(void)
{
	return register_exec_domain(&arthur_exec_domain);
}

static void __exit arthur_exit(void)
{
	unregister_exec_domain(&arthur_exec_domain);
}

module_init(arthur_init);
module_exit(arthur_exit);

MODULE_LICENSE("GPL");
