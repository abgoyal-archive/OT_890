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
 *  arch/arm/mach-footbridge/include/mach/system.h
 *
 *  Copyright (C) 1996-1999 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/io.h>
#include <asm/hardware/dec21285.h>
#include <mach/hardware.h>
#include <asm/leds.h>
#include <asm/mach-types.h>

static inline void arch_idle(void)
{
	cpu_do_idle();
}

static inline void arch_reset(char mode)
{
	if (mode == 's') {
		/*
		 * Jump into the ROM
		 */
		cpu_reset(0x41000000);
	} else {
		if (machine_is_netwinder()) {
			/* open up the SuperIO chip
			 */
			outb(0x87, 0x370);
			outb(0x87, 0x370);

			/* aux function group 1 (logical device 7)
			 */
			outb(0x07, 0x370);
			outb(0x07, 0x371);

			/* set GP16 for WD-TIMER output
			 */
			outb(0xe6, 0x370);
			outb(0x00, 0x371);

			/* set a RED LED and toggle WD_TIMER for rebooting
			 */
			outb(0xc4, 0x338);
		} else {
			/* 
			 * Force the watchdog to do a CPU reset.
			 *
			 * After making sure that the watchdog is disabled
			 * (so we can change the timer registers) we first
			 * enable the timer to autoreload itself.  Next, the
			 * timer interval is set really short and any
			 * current interrupt request is cleared (so we can
			 * see an edge transition).  Finally, TIMER4 is
			 * enabled as the watchdog.
			 */
			*CSR_SA110_CNTL &= ~(1 << 13);
			*CSR_TIMER4_CNTL = TIMER_CNTL_ENABLE |
					   TIMER_CNTL_AUTORELOAD |
					   TIMER_CNTL_DIV16;
			*CSR_TIMER4_LOAD = 0x2;
			*CSR_TIMER4_CLR  = 0;
			*CSR_SA110_CNTL |= (1 << 13);
		}
	}
}
