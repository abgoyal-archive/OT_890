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
 * include/linux/irqflags.h
 *
 * IRQ flags tracing: follow the state of the hardirq and softirq flags and
 * provide callbacks for transitions between ON and OFF states.
 *
 * This file gets included from lowlevel asm headers too, to provide
 * wrapped versions of the local_irq_*() APIs, based on the
 * raw_local_irq_*() macros from the lowlevel headers.
 */
#ifndef _LINUX_TRACE_IRQFLAGS_H
#define _LINUX_TRACE_IRQFLAGS_H

#include <linux/typecheck.h>

#ifdef CONFIG_TRACE_IRQFLAGS
  extern void trace_softirqs_on(unsigned long ip);
  extern void trace_softirqs_off(unsigned long ip);
  extern void trace_hardirqs_on(void);
  extern void trace_hardirqs_off(void);
# define trace_hardirq_context(p)	((p)->hardirq_context)
# define trace_softirq_context(p)	((p)->softirq_context)
# define trace_hardirqs_enabled(p)	((p)->hardirqs_enabled)
# define trace_softirqs_enabled(p)	((p)->softirqs_enabled)
# define trace_hardirq_enter()	do { current->hardirq_context++; } while (0)
# define trace_hardirq_exit()	do { current->hardirq_context--; } while (0)
# define trace_softirq_enter()	do { current->softirq_context++; } while (0)
# define trace_softirq_exit()	do { current->softirq_context--; } while (0)
# define INIT_TRACE_IRQFLAGS	.softirqs_enabled = 1,
#else
# define trace_hardirqs_on()		do { } while (0)
# define trace_hardirqs_off()		do { } while (0)
# define trace_softirqs_on(ip)		do { } while (0)
# define trace_softirqs_off(ip)		do { } while (0)
# define trace_hardirq_context(p)	0
# define trace_softirq_context(p)	0
# define trace_hardirqs_enabled(p)	0
# define trace_softirqs_enabled(p)	0
# define trace_hardirq_enter()		do { } while (0)
# define trace_hardirq_exit()		do { } while (0)
# define trace_softirq_enter()		do { } while (0)
# define trace_softirq_exit()		do { } while (0)
# define INIT_TRACE_IRQFLAGS
#endif

#if defined(CONFIG_IRQSOFF_TRACER) || \
	defined(CONFIG_PREEMPT_TRACER)
 extern void stop_critical_timings(void);
 extern void start_critical_timings(void);
#else
# define stop_critical_timings() do { } while (0)
# define start_critical_timings() do { } while (0)
#endif

#ifdef CONFIG_TRACE_IRQFLAGS_SUPPORT

#include <asm/irqflags.h>

#define local_irq_enable() \
	do { trace_hardirqs_on(); raw_local_irq_enable(); } while (0)
#define local_irq_disable() \
	do { raw_local_irq_disable(); trace_hardirqs_off(); } while (0)
#define local_irq_save(flags)				\
	do {						\
		typecheck(unsigned long, flags);	\
		raw_local_irq_save(flags);		\
		trace_hardirqs_off();			\
	} while (0)


#define local_irq_restore(flags)			\
	do {						\
		typecheck(unsigned long, flags);	\
		if (raw_irqs_disabled_flags(flags)) {	\
			raw_local_irq_restore(flags);	\
			trace_hardirqs_off();		\
		} else {				\
			trace_hardirqs_on();		\
			raw_local_irq_restore(flags);	\
		}					\
	} while (0)
#else /* !CONFIG_TRACE_IRQFLAGS_SUPPORT */
/*
 * The local_irq_*() APIs are equal to the raw_local_irq*()
 * if !TRACE_IRQFLAGS.
 */
# define raw_local_irq_disable()	local_irq_disable()
# define raw_local_irq_enable()		local_irq_enable()
# define raw_local_irq_save(flags)			\
	do {						\
		typecheck(unsigned long, flags);	\
		local_irq_save(flags);			\
	} while (0)
# define raw_local_irq_restore(flags)			\
	do {						\
		typecheck(unsigned long, flags);	\
		local_irq_restore(flags);		\
	} while (0)
#endif /* CONFIG_TRACE_IRQFLAGS_SUPPORT */

#ifdef CONFIG_TRACE_IRQFLAGS_SUPPORT
#define safe_halt()						\
	do {							\
		trace_hardirqs_on();				\
		raw_safe_halt();				\
	} while (0)

#define local_save_flags(flags)				\
	do {						\
		typecheck(unsigned long, flags);	\
		raw_local_save_flags(flags);		\
	} while (0)

#define irqs_disabled()						\
({								\
	unsigned long _flags;					\
								\
	raw_local_save_flags(_flags);				\
	raw_irqs_disabled_flags(_flags);			\
})

#define irqs_disabled_flags(flags)		\
({						\
	typecheck(unsigned long, flags);	\
	raw_irqs_disabled_flags(flags);		\
})
#endif		/* CONFIG_X86 */

#endif
