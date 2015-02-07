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

/* Ported over from i386 by AK, original copyright was:
 *
 * (C) Dominik Brodowski <linux@brodo.de> 2003
 *
 * Driver to use the Power Management Timer (PMTMR) available in some
 * southbridges as primary timing source for the Linux kernel.
 *
 * Based on parts of linux/drivers/acpi/hardware/hwtimer.c, timer_pit.c,
 * timer_hpet.c, and on Arjan van de Ven's implementation for 2.4.
 *
 * This file is licensed under the GPL v2.
 *
 * Dropped all the hardware bug workarounds for now. Hopefully they
 * are not needed on 64bit chipsets.
 */

#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/cpumask.h>
#include <linux/acpi_pmtmr.h>

#include <asm/io.h>
#include <asm/proto.h>
#include <asm/msr.h>
#include <asm/vsyscall.h>

static inline u32 cyc2us(u32 cycles)
{
	/* The Power Management Timer ticks at 3.579545 ticks per microsecond.
	 * 1 / PM_TIMER_FREQUENCY == 0.27936511 =~ 286/1024 [error: 0.024%]
	 *
	 * Even with HZ = 100, delta is at maximum 35796 ticks, so it can
	 * easily be multiplied with 286 (=0x11E) without having to fear
	 * u32 overflows.
	 */
	cycles *= 286;
	return (cycles >> 10);
}

static unsigned pmtimer_wait_tick(void)
{
	u32 a, b;
	for (a = b = inl(pmtmr_ioport) & ACPI_PM_MASK;
	     a == b;
	     b = inl(pmtmr_ioport) & ACPI_PM_MASK)
		cpu_relax();
	return b;
}

/* note: wait time is rounded up to one tick */
void pmtimer_wait(unsigned us)
{
	u32 a, b;
	a = pmtimer_wait_tick();
	do {
		b = inl(pmtmr_ioport);
		cpu_relax();
	} while (cyc2us(b - a) < us);
}

static int __init nopmtimer_setup(char *s)
{
	pmtmr_ioport = 0;
	return 1;
}

__setup("nopmtimer", nopmtimer_setup);
