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
 * arch/arm/include/asm/mach/time.h
 *
 * Copyright (C) 2004 MontaVista Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_ARM_MACH_TIME_H
#define __ASM_ARM_MACH_TIME_H

#include <linux/sysdev.h>

/*
 * This is our kernel timer structure.
 *
 * - init
 *   Initialise the kernels jiffy timer source, claim interrupt
 *   using setup_irq.  This is called early on during initialisation
 *   while interrupts are still disabled on the local CPU.
 * - suspend
 *   Suspend the kernel jiffy timer source, if necessary.  This
 *   is called with interrupts disabled, after all normal devices
 *   have been suspended.  If no action is required, set this to
 *   NULL.
 * - resume
 *   Resume the kernel jiffy timer source, if necessary.  This
 *   is called with interrupts disabled before any normal devices
 *   are resumed.  If no action is required, set this to NULL.
 * - offset
 *   Return the timer offset in microseconds since the last timer
 *   interrupt.  Note: this must take account of any unprocessed
 *   timer interrupt which may be pending.
 */
struct sys_timer {
	struct sys_device	dev;
	void			(*init)(void);
	void			(*suspend)(void);
	void			(*resume)(void);
#ifndef CONFIG_GENERIC_TIME
	unsigned long		(*offset)(void);
#endif
};

extern struct sys_timer *system_timer;
extern void timer_tick(void);

/*
 * Kernel time keeping support.
 */
struct timespec;
extern int (*set_rtc)(void);
extern void save_time_delta(struct timespec *delta, struct timespec *rtc);
extern void restore_time_delta(struct timespec *delta, struct timespec *rtc);

#endif
