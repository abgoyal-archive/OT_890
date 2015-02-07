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
 * Copyright (C) 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_AVR32_IRQFLAGS_H
#define __ASM_AVR32_IRQFLAGS_H

#include <asm/sysreg.h>

static inline unsigned long __raw_local_save_flags(void)
{
	return sysreg_read(SR);
}

#define raw_local_save_flags(x)					\
	do { (x) = __raw_local_save_flags(); } while (0)

/*
 * This will restore ALL status register flags, not only the interrupt
 * mask flag.
 *
 * The empty asm statement informs the compiler of this fact while
 * also serving as a barrier.
 */
static inline void raw_local_irq_restore(unsigned long flags)
{
	sysreg_write(SR, flags);
	asm volatile("" : : : "memory", "cc");
}

static inline void raw_local_irq_disable(void)
{
	asm volatile("ssrf %0" : : "n"(SYSREG_GM_OFFSET) : "memory");
}

static inline void raw_local_irq_enable(void)
{
	asm volatile("csrf %0" : : "n"(SYSREG_GM_OFFSET) : "memory");
}

static inline int raw_irqs_disabled_flags(unsigned long flags)
{
	return (flags & SYSREG_BIT(GM)) != 0;
}

static inline int raw_irqs_disabled(void)
{
	unsigned long flags = __raw_local_save_flags();

	return raw_irqs_disabled_flags(flags);
}

static inline unsigned long __raw_local_irq_save(void)
{
	unsigned long flags = __raw_local_save_flags();

	raw_local_irq_disable();

	return flags;
}

#define raw_local_irq_save(flags)				\
	do { (flags) = __raw_local_irq_save(); } while (0)

#endif /* __ASM_AVR32_IRQFLAGS_H */
