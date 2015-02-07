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
 *    Precise Delay Loops for S390
 *
 *    Copyright IBM Corp. 1999,2008
 *    Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *		 Heiko Carstens <heiko.carstens@de.ibm.com>,
 */

#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/timex.h>
#include <linux/irqflags.h>
#include <linux/interrupt.h>

void __delay(unsigned long loops)
{
        /*
         * To end the bloody studid and useless discussion about the
         * BogoMips number I took the liberty to define the __delay
         * function in a way that that resulting BogoMips number will
         * yield the megahertz number of the cpu. The important function
         * is udelay and that is done using the tod clock. -- martin.
         */
	asm volatile("0: brct %0,0b" : : "d" ((loops/2) + 1));
}

static void __udelay_disabled(unsigned long usecs)
{
	unsigned long mask, cr0, cr0_saved;
	u64 clock_saved;

	clock_saved = local_tick_disable();
	set_clock_comparator(get_clock() + ((u64) usecs << 12));
	__ctl_store(cr0_saved, 0, 0);
	cr0 = (cr0_saved & 0xffff00e0) | 0x00000800;
	__ctl_load(cr0 , 0, 0);
	mask = psw_kernel_bits | PSW_MASK_WAIT | PSW_MASK_EXT;
	trace_hardirqs_on();
	__load_psw_mask(mask);
	local_irq_disable();
	__ctl_load(cr0_saved, 0, 0);
	local_tick_enable(clock_saved);
	set_clock_comparator(S390_lowcore.clock_comparator);
}

static void __udelay_enabled(unsigned long usecs)
{
	unsigned long mask;
	u64 end, time;

	mask = psw_kernel_bits | PSW_MASK_WAIT | PSW_MASK_EXT | PSW_MASK_IO;
	end = get_clock() + ((u64) usecs << 12);
	do {
		time = end < S390_lowcore.clock_comparator ?
			end : S390_lowcore.clock_comparator;
		set_clock_comparator(time);
		trace_hardirqs_on();
		__load_psw_mask(mask);
		local_irq_disable();
	} while (get_clock() < end);
	set_clock_comparator(S390_lowcore.clock_comparator);
}

/*
 * Waits for 'usecs' microseconds using the TOD clock comparator.
 */
void __udelay(unsigned long usecs)
{
	unsigned long flags;

	preempt_disable();
	local_irq_save(flags);
	if (in_irq()) {
		__udelay_disabled(usecs);
		goto out;
	}
	if (in_softirq()) {
		if (raw_irqs_disabled_flags(flags))
			__udelay_disabled(usecs);
		else
			__udelay_enabled(usecs);
		goto out;
	}
	if (raw_irqs_disabled_flags(flags)) {
		local_bh_disable();
		__udelay_disabled(usecs);
		_local_bh_enable();
		goto out;
	}
	__udelay_enabled(usecs);
out:
	local_irq_restore(flags);
	preempt_enable();
}

/*
 * Simple udelay variant. To be used on startup and reboot
 * when the interrupt handler isn't working.
 */
void udelay_simple(unsigned long usecs)
{
	u64 end;

	end = get_clock() + ((u64) usecs << 12);
	while (get_clock() < end)
		cpu_relax();
}
