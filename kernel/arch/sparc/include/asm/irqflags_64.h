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
 * include/asm/irqflags.h
 *
 * IRQ flags handling
 *
 * This file gets included from lowlevel asm headers too, to provide
 * wrapped versions of the local_irq_*() APIs, based on the
 * raw_local_irq_*() functions from the lowlevel headers.
 */
#ifndef _ASM_IRQFLAGS_H
#define _ASM_IRQFLAGS_H

#include <asm/pil.h>

#ifndef __ASSEMBLY__

static inline unsigned long __raw_local_save_flags(void)
{
	unsigned long flags;

	__asm__ __volatile__(
		"rdpr	%%pil, %0"
		: "=r" (flags)
	);

	return flags;
}

#define raw_local_save_flags(flags) \
		do { (flags) = __raw_local_save_flags(); } while (0)

static inline void raw_local_irq_restore(unsigned long flags)
{
	__asm__ __volatile__(
		"wrpr	%0, %%pil"
		: /* no output */
		: "r" (flags)
		: "memory"
	);
}

static inline void raw_local_irq_disable(void)
{
	__asm__ __volatile__(
		"wrpr	%0, %%pil"
		: /* no outputs */
		: "i" (PIL_NORMAL_MAX)
		: "memory"
	);
}

static inline void raw_local_irq_enable(void)
{
	__asm__ __volatile__(
		"wrpr	0, %%pil"
		: /* no outputs */
		: /* no inputs */
		: "memory"
	);
}

static inline int raw_irqs_disabled_flags(unsigned long flags)
{
	return (flags > 0);
}

static inline int raw_irqs_disabled(void)
{
	unsigned long flags = __raw_local_save_flags();

	return raw_irqs_disabled_flags(flags);
}

/*
 * For spinlocks, etc:
 */
static inline unsigned long __raw_local_irq_save(void)
{
	unsigned long flags = __raw_local_save_flags();

	raw_local_irq_disable();

	return flags;
}

#define raw_local_irq_save(flags) \
		do { (flags) = __raw_local_irq_save(); } while (0)

#endif /* (__ASSEMBLY__) */

#endif /* !(_ASM_IRQFLAGS_H) */
