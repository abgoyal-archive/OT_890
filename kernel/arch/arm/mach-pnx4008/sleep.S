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
 * linux/arch/arm/mach-pnx4008/sleep.S
 *
 * PNX4008 support for STOP mode and SDRAM self-refresh
 *
 * Authors: Dmitry Chigirev, Vitaly Wool <source@mvista.com>
 *
 * 2005 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#include <linux/linkage.h>
#include <asm/assembler.h>
#include <mach/hardware.h>

#define PWRMAN_VA_BASE IO_ADDRESS(PNX4008_PWRMAN_BASE)
#define PWR_CTRL_REG_OFFS 0x44

#define SDRAM_CFG_VA_BASE IO_ADDRESS(PNX4008_SDRAM_CFG_BASE)
#define MPMC_STATUS_REG_OFFS 0x4

		.text

ENTRY(pnx4008_cpu_suspend)
	@this function should be entered in Direct run mode.

	@ save registers on stack
	stmfd	sp!, {r0 - r6, lr}

	@ setup Power Manager base address in r4
	@ and put it's value in r5
	mov	r4, #(PWRMAN_VA_BASE & 0xff000000)
	orr	r4, r4, #(PWRMAN_VA_BASE & 0x00ff0000)
	orr	r4, r4, #(PWRMAN_VA_BASE & 0x0000ff00)
	orr	r4, r4, #(PWRMAN_VA_BASE & 0x000000ff)
	ldr	r5, [r4, #PWR_CTRL_REG_OFFS]

	@ setup SDRAM controller base address in r2
	@ and put it's value in r3
	mov	r2, #(SDRAM_CFG_VA_BASE & 0xff000000)
	orr	r2, r2, #(SDRAM_CFG_VA_BASE & 0x00ff0000)
	orr	r2, r2, #(SDRAM_CFG_VA_BASE & 0x0000ff00)
	orr	r2, r2, #(SDRAM_CFG_VA_BASE & 0x000000ff)
	ldr	r3, [r2, #MPMC_STATUS_REG_OFFS] @extra read - HW bug workaround

	@ clear SDRAM self-refresh bit latch
	and	r5, r5, #(~(1 << 8))
	@ clear SDRAM self-refresh bit
	and	r5, r5, #(~(1 << 9))
	str	r5, [r4, #PWR_CTRL_REG_OFFS]

	@ do save current bit settings in r1
	mov	r1, r5

	@ set SDRAM self-refresh bit
	orr	r5, r5, #(1 << 9)
	str	r5, [r4, #PWR_CTRL_REG_OFFS]

	@ set SDRAM self-refresh bit latch
	orr	r5, r5, #(1 << 8)
	str	r5, [r4, #PWR_CTRL_REG_OFFS]

	@ clear SDRAM self-refresh bit latch
	and	r5, r5, #(~(1 << 8))
	str	r5, [r4, #PWR_CTRL_REG_OFFS]

	@ clear SDRAM self-refresh bit
	and	r5, r5, #(~(1 << 9))
	str	r5, [r4, #PWR_CTRL_REG_OFFS]

	@ wait for SDRAM to get into self-refresh mode
2:	ldr	r3, [r2, #MPMC_STATUS_REG_OFFS]
	tst	r3, #(1 << 2)
	beq	2b

	@ to prepare SDRAM to get out of self-refresh mode after wakeup
	orr	r5, r5, #(1 << 7)
	str	r5, [r4, #PWR_CTRL_REG_OFFS]

	@ do enter stop mode
	orr	r5, r5, #(1 << 0)
	str	r5, [r4, #PWR_CTRL_REG_OFFS]
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	@ sleeping now...

	@ coming out of STOP mode into Direct Run mode
	@ clear STOP mode and SDRAM self-refresh bits
	str	r1, [r4, #PWR_CTRL_REG_OFFS]

	@ wait for SDRAM to get out self-refresh mode
3:	ldr	r3, [r2, #MPMC_STATUS_REG_OFFS]
	tst	r3, #5
	bne	3b

	@ restore regs and return
	ldmfd   sp!, {r0 - r6, pc}

ENTRY(pnx4008_cpu_suspend_sz)
	.word	. - pnx4008_cpu_suspend

ENTRY(pnx4008_cpu_standby)
	@ save registers on stack
	stmfd	sp!, {r0 - r6, lr}

	@ setup Power Manager base address in r4
	@ and put it's value in r5
	mov	r4, #(PWRMAN_VA_BASE & 0xff000000)
	orr	r4, r4, #(PWRMAN_VA_BASE & 0x00ff0000)
	orr	r4, r4, #(PWRMAN_VA_BASE & 0x0000ff00)
	orr	r4, r4, #(PWRMAN_VA_BASE & 0x000000ff)
	ldr	r5, [r4, #PWR_CTRL_REG_OFFS]

	@ setup SDRAM controller base address in r2
	@ and put it's value in r3
	mov	r2, #(SDRAM_CFG_VA_BASE & 0xff000000)
	orr	r2, r2, #(SDRAM_CFG_VA_BASE & 0x00ff0000)
	orr	r2, r2, #(SDRAM_CFG_VA_BASE & 0x0000ff00)
	orr	r2, r2, #(SDRAM_CFG_VA_BASE & 0x000000ff)
	ldr	r3, [r2, #MPMC_STATUS_REG_OFFS] @extra read - HW bug workaround

	@ clear SDRAM self-refresh bit latch
	and	r5, r5, #(~(1 << 8))
	@ clear SDRAM self-refresh bit
	and	r5, r5, #(~(1 << 9))
	str	r5, [r4, #PWR_CTRL_REG_OFFS]

	@ do save current bit settings in r1
	mov	r1, r5

	@ set SDRAM self-refresh bit
	orr	r5, r5, #(1 << 9)
	str	r5, [r4, #PWR_CTRL_REG_OFFS]

	@ set SDRAM self-refresh bit latch
	orr	r5, r5, #(1 << 8)
	str	r5, [r4, #PWR_CTRL_REG_OFFS]

	@ clear SDRAM self-refresh bit latch
	and	r5, r5, #(~(1 << 8))
	str	r5, [r4, #PWR_CTRL_REG_OFFS]

	@ clear SDRAM self-refresh bit
	and	r5, r5, #(~(1 << 9))
	str	r5, [r4, #PWR_CTRL_REG_OFFS]

	@ wait for SDRAM to get into self-refresh mode
2:	ldr	r3, [r2, #MPMC_STATUS_REG_OFFS]
	tst	r3, #(1 << 2)
	beq	2b

	@ set 'get out of self-refresh mode after wakeup' bit
	orr	r5, r5, #(1 << 7)
	str	r5, [r4, #PWR_CTRL_REG_OFFS]

	mcr     p15, 0, r0, c7, c0, 4	@ kinda sleeping now...

	@ set SDRAM self-refresh bit latch
	orr	r5, r5, #(1 << 8)
	str	r5, [r4, #PWR_CTRL_REG_OFFS]

	@ clear SDRAM self-refresh bit latch
	and	r5, r5, #(~(1 << 8))
	str	r5, [r4, #PWR_CTRL_REG_OFFS]

	@ wait for SDRAM to get out self-refresh mode
3:	ldr	r3, [r2, #MPMC_STATUS_REG_OFFS]
	tst	r3, #5
	bne	3b

	@ restore regs and return
	ldmfd   sp!, {r0 - r6, pc}

ENTRY(pnx4008_cpu_standby_sz)
	.word	. - pnx4008_cpu_standby

ENTRY(pnx4008_cache_clean_invalidate)
	stmfd	sp!, {r0 - r6, lr}
#ifdef CONFIG_CPU_DCACHE_WRITETHROUGH
	mcr	p15, 0, ip, c7, c6, 0		@ invalidate D cache
#else
1:	mrc	p15, 0, r15, c7, c14, 3		@ test,clean,invalidate
	bne     1b
#endif
	ldmfd   sp!, {r0 - r6, pc}
