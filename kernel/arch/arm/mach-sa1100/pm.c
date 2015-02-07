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
 * SA1100 Power Management Routines
 *
 * Copyright (c) 2001 Cliff Brake <cbrake@accelent.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License.
 *
 * History:
 *
 * 2001-02-06:	Cliff Brake         Initial code
 *
 * 2001-02-25:	Sukjae Cho <sjcho@east.isi.edu> &
 * 		Chester Kuo <chester@linux.org.tw>
 * 			Save more value for the resume function! Support
 * 			Bitsy/Assabet/Freebird board
 *
 * 2001-08-29:	Nicolas Pitre <nico@cam.org>
 * 			Cleaned up, pushed platform dependent stuff
 * 			in the platform specific files.
 *
 * 2002-05-27:	Nicolas Pitre	Killed sleep.h and the kmalloced save array.
 * 				Storage is local on the stack now.
 */
#include <linux/init.h>
#include <linux/suspend.h>
#include <linux/errno.h>
#include <linux/time.h>

#include <mach/hardware.h>
#include <asm/memory.h>
#include <asm/system.h>
#include <asm/mach/time.h>

extern void sa1100_cpu_suspend(void);
extern void sa1100_cpu_resume(void);

#define SAVE(x)		sleep_save[SLEEP_SAVE_##x] = x
#define RESTORE(x)	x = sleep_save[SLEEP_SAVE_##x]

/*
 * List of global SA11x0 peripheral registers to preserve.
 * More ones like CP and general purpose register values are preserved
 * on the stack and then the stack pointer is stored last in sleep.S.
 */
enum {	SLEEP_SAVE_GPDR, SLEEP_SAVE_GAFR,
	SLEEP_SAVE_PPDR, SLEEP_SAVE_PPSR, SLEEP_SAVE_PPAR, SLEEP_SAVE_PSDR,

	SLEEP_SAVE_Ser1SDCR0,

	SLEEP_SAVE_COUNT
};


static int sa11x0_pm_enter(suspend_state_t state)
{
	unsigned long gpio, sleep_save[SLEEP_SAVE_COUNT];

	gpio = GPLR;

	/* save vital registers */
	SAVE(GPDR);
	SAVE(GAFR);

	SAVE(PPDR);
	SAVE(PPSR);
	SAVE(PPAR);
	SAVE(PSDR);

	SAVE(Ser1SDCR0);

	/* Clear previous reset status */
	RCSR = RCSR_HWR | RCSR_SWR | RCSR_WDR | RCSR_SMR;

	/* set resume return address */
	PSPR = virt_to_phys(sa1100_cpu_resume);

	/* go zzz */
	sa1100_cpu_suspend();

	cpu_init();

	/*
	 * Ensure not to come back here if it wasn't intended
	 */
	PSPR = 0;

	/*
	 * Ensure interrupt sources are disabled; we will re-init
	 * the interrupt subsystem via the device manager.
	 */
	ICLR = 0;
	ICCR = 1;
	ICMR = 0;

	/* restore registers */
	RESTORE(GPDR);
	RESTORE(GAFR);

	RESTORE(PPDR);
	RESTORE(PPSR);
	RESTORE(PPAR);
	RESTORE(PSDR);

	RESTORE(Ser1SDCR0);

	GPSR = gpio;
	GPCR = ~gpio;

	/*
	 * Clear the peripheral sleep-hold bit.
	 */
	PSSR = PSSR_PH;

	return 0;
}

unsigned long sleep_phys_sp(void *sp)
{
	return virt_to_phys(sp);
}

static struct platform_suspend_ops sa11x0_pm_ops = {
	.enter		= sa11x0_pm_enter,
	.valid		= suspend_valid_only_mem,
};

static int __init sa11x0_pm_init(void)
{
	suspend_set_ops(&sa11x0_pm_ops);
	return 0;
}

late_initcall(sa11x0_pm_init);
