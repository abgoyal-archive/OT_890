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
 * FR-V MB93093 Power Management Routines
 *
 * Copyright (c) 2004 Red Hat, Inc.
 *
 * Written by: msalter@redhat.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License.
 *
 */

#include <linux/init.h>
#include <linux/pm.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/sysctl.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <asm/uaccess.h>

#include <asm/mb86943a.h>

#include "local.h"

static unsigned long imask;
/*
 * Setup interrupt masks, etc to enable wakeup by power switch
 */
static void mb93093_power_switch_setup(void)
{
	/* mask all but FPGA interrupt sources. */
	imask = *(volatile unsigned long *)0xfeff9820;
	*(volatile unsigned long *)0xfeff9820 = ~(1 << (IRQ_XIRQ2_LEVEL + 16)) & 0xfffe0000;
}

/*
 * Cleanup interrupt masks, etc after wakeup by power switch
 */
static void mb93093_power_switch_cleanup(void)
{
	*(volatile unsigned long *)0xfeff9820 = imask;
}

/*
 * Return non-zero if wakeup irq was caused by power switch
 */
static int mb93093_power_switch_check(void)
{
	return 1;
}

/*
 * Initialize power interface
 */
static int __init mb93093_pm_init(void)
{
	__power_switch_wake_setup = mb93093_power_switch_setup;
	__power_switch_wake_check = mb93093_power_switch_check;
	__power_switch_wake_cleanup = mb93093_power_switch_cleanup;
	return 0;
}

__initcall(mb93093_pm_init);

