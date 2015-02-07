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
 * Setting up the clock on MSP SOCs.  No RTC typically.
 *
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 1999,2000 MIPS Technologies, Inc.  All rights reserved.
 *
 * ########################################################################
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * ########################################################################
 */

#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/ptrace.h>

#include <asm/mipsregs.h>
#include <asm/time.h>

#include <msp_prom.h>
#include <msp_int.h>
#include <msp_regs.h>

void __init plat_time_init(void)
{
	char    *endp, *s;
	unsigned long cpu_rate = 0;

	if (cpu_rate == 0) {
		s = prom_getenv("clkfreqhz");
		cpu_rate = simple_strtoul(s, &endp, 10);
		if (endp != NULL && *endp != 0) {
			printk(KERN_ERR
				"Clock rate in Hz parse error: %s\n", s);
			cpu_rate = 0;
		}
	}

	if (cpu_rate == 0) {
		s = prom_getenv("clkfreq");
		cpu_rate = 1000 * simple_strtoul(s, &endp, 10);
		if (endp != NULL && *endp != 0) {
			printk(KERN_ERR
				"Clock rate in MHz parse error: %s\n", s);
			cpu_rate = 0;
		}
	}

	if (cpu_rate == 0) {
#if defined(CONFIG_PMC_MSP7120_EVAL) \
 || defined(CONFIG_PMC_MSP7120_GW)
		cpu_rate = 400000000;
#elif defined(CONFIG_PMC_MSP7120_FPGA)
		cpu_rate = 25000000;
#else
		cpu_rate = 150000000;
#endif
		printk(KERN_ERR
			"Failed to determine CPU clock rate, "
			"assuming %ld hz ...\n", cpu_rate);
	}

	printk(KERN_WARNING "Clock rate set to %ld\n", cpu_rate);

	/* timer frequency is 1/2 clock rate */
	mips_hpt_frequency = cpu_rate/2;
}

void __init plat_timer_setup(struct irqaction *irq)
{
#ifdef CONFIG_IRQ_MSP_CIC
	/* we are using the vpe0 counter for timer interrupts */
	setup_irq(MSP_INT_VPE0_TIMER, irq);
#endif
}
