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
 * MTD primitives for XIP support. Architecture specific functions.
 *
 * Do not include this file directly. It's included from linux/mtd/xip.h
 *
 * Author: Vladimir Barinov <vbarinov@embeddedalley.com>
 *
 * (c) 2005 MontaVista Software, Inc.  This file is licensed under the
 * terms of the GNU General Public License version 2.  This program is
 * licensed "as is" without any warranty of any kind, whether express or
 * implied.
 */

#ifndef __ARCH_OMAP_MTD_XIP_H__
#define __ARCH_OMAP_MTD_XIP_H__

#include <mach/hardware.h>
#define OMAP_MPU_TIMER_BASE	(0xfffec500)
#define OMAP_MPU_TIMER_OFFSET	0x100

typedef struct {
	u32 cntl;			/* CNTL_TIMER, R/W */
	u32 load_tim;			/* LOAD_TIM,   W */
	u32 read_tim;			/* READ_TIM,   R */
} xip_omap_mpu_timer_regs_t;

#define xip_omap_mpu_timer_base(n)					\
((volatile xip_omap_mpu_timer_regs_t*)IO_ADDRESS(OMAP_MPU_TIMER_BASE +	\
	(n)*OMAP_MPU_TIMER_OFFSET))

static inline unsigned long xip_omap_mpu_timer_read(int nr)
{
	volatile xip_omap_mpu_timer_regs_t* timer = xip_omap_mpu_timer_base(nr);
	return timer->read_tim;
}

#define xip_irqpending()	\
	(omap_readl(OMAP_IH1_ITR) & ~omap_readl(OMAP_IH1_MIR))
#define xip_currtime()		(~xip_omap_mpu_timer_read(0))

/*
 * It's permitted to do approxmation for xip_elapsed_since macro
 * (see linux/mtd/xip.h)
 */

#ifdef CONFIG_MACH_OMAP_PERSEUS2
#define xip_elapsed_since(x)	(signed)((~xip_omap_mpu_timer_read(0) - (x)) / 7)
#else
#define xip_elapsed_since(x)	(signed)((~xip_omap_mpu_timer_read(0) - (x)) / 6)
#endif

/*
 * xip_cpu_idle() is used when waiting for a delay equal or larger than
 * the system timer tick period.  This should put the CPU into idle mode
 * to save power and to be woken up only when some interrupts are pending.
 * As above, this should not rely upon standard kernel code.
 */

#define xip_cpu_idle()  asm volatile ("mcr p15, 0, %0, c7, c0, 4" :: "r" (1))

#endif /* __ARCH_OMAP_MTD_XIP_H__ */
