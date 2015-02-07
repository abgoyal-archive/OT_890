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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 *
 * Changed by HuTao Apr18, 2003
 *
 * Copyright was missing when I got the code so took from MIPS arch ...MaTed---
 * Copyright (C) 1994 by Waldorf GMBH, written by Ralf Baechle
 * Copyright (C) 1995, 96, 97, 98, 99, 2000, 2001 by Ralf Baechle
 *
 * Adapted for BlackFin (ADI) by Ted Ma <mated@sympatico.ca>
 * Copyright (c) 2002 Arcturus Networks Inc. (www.arcturusnetworks.com)
 * Copyright (c) 2002 Lineo, Inc. <mattw@lineo.com>
 */

#ifndef _BFIN_IRQ_H_
#define _BFIN_IRQ_H_

/* SYS_IRQS and NR_IRQS are defined in <mach-bf5xx/irq.h>*/
#include <mach/irq.h>
#include <asm/pda.h>
#include <asm/processor.h>

#ifdef CONFIG_SMP
/* Forward decl needed due to cdef inter dependencies */
static inline uint32_t __pure bfin_dspid(void);
# define blackfin_core_id() (bfin_dspid() & 0xff)
# define bfin_irq_flags cpu_pda[blackfin_core_id()].imask
#else
extern unsigned long bfin_irq_flags;
#endif

#ifdef CONFIG_IPIPE

#include <linux/ipipe_trace.h>

void __ipipe_unstall_root(void);

void __ipipe_restore_root(unsigned long flags);

#ifdef CONFIG_DEBUG_HWERR
# define __all_masked_irq_flags 0x3f
# define __save_and_cli_hw(x) \
	__asm__ __volatile__( \
		"cli %0;" \
		"sti %1;" \
		: "=&d"(x) \
		: "d" (0x3F) \
	)
#else
# define __all_masked_irq_flags 0x1f
# define __save_and_cli_hw(x) \
	__asm__ __volatile__( \
		"cli %0;" \
		: "=&d"(x) \
	)
#endif

#define irqs_enabled_from_flags_hw(x)	((x) != __all_masked_irq_flags)
#define raw_irqs_disabled_flags(flags)	(!irqs_enabled_from_flags_hw(flags))
#define local_test_iflag_hw(x)		irqs_enabled_from_flags_hw(x)

#define local_save_flags(x)					 \
	do {							 \
		(x) = __ipipe_test_root() ?			 \
			__all_masked_irq_flags : bfin_irq_flags; \
		barrier();					 \
	} while (0)

#define local_irq_save(x)					 \
	do {						 	 \
		(x) = __ipipe_test_and_stall_root() ?		 \
			__all_masked_irq_flags : bfin_irq_flags; \
		barrier();					 \
	} while (0)

static inline void local_irq_restore(unsigned long x)
{
	barrier();
	__ipipe_restore_root(x == __all_masked_irq_flags);
}

#define local_irq_disable()			\
	do {					\
		__ipipe_stall_root();		\
		barrier();			\
	} while (0)

static inline void local_irq_enable(void)
{
	barrier();
	__ipipe_unstall_root();
}

#define irqs_disabled()		__ipipe_test_root()

#define local_save_flags_hw(x) \
	__asm__ __volatile__( \
		"cli %0;" \
		"sti %0;" \
		: "=d"(x) \
	)

#define	irqs_disabled_hw()				\
	({						\
		unsigned long flags;			\
		local_save_flags_hw(flags);		\
		!irqs_enabled_from_flags_hw(flags);	\
	})

static inline unsigned long raw_mangle_irq_bits(int virt, unsigned long real)
{
	/* Merge virtual and real interrupt mask bits into a single
	   32bit word. */
	return (real & ~(1 << 31)) | ((virt != 0) << 31);
}

static inline int raw_demangle_irq_bits(unsigned long *x)
{
	int virt = (*x & (1 << 31)) != 0;
	*x &= ~(1L << 31);
	return virt;
}

#ifdef CONFIG_IPIPE_TRACE_IRQSOFF

#define local_irq_disable_hw()						\
	do {								\
		int _tmp_dummy;						\
		if (!irqs_disabled_hw())				\
			ipipe_trace_begin(0x80000000);			\
		__asm__ __volatile__ ("cli %0;" : "=d" (_tmp_dummy) : );	\
	} while (0)

#define local_irq_enable_hw()						\
	do {								\
		if (irqs_disabled_hw())					\
			ipipe_trace_end(0x80000000);			\
		__asm__ __volatile__ ("sti %0;" : : "d"(bfin_irq_flags));	\
	} while (0)

#define local_irq_save_hw(x)				\
	do {						\
		__save_and_cli_hw(x);			\
		if (local_test_iflag_hw(x))		\
			ipipe_trace_begin(0x80000001);	\
	} while (0)

#define local_irq_restore_hw(x)				\
	do {						\
		if (local_test_iflag_hw(x)) {		\
			ipipe_trace_end(0x80000001);	\
			local_irq_enable_hw_notrace();	\
		}					\
	} while (0)

#define local_irq_disable_hw_notrace()					\
	do {								\
		int _tmp_dummy;						\
		__asm__ __volatile__ ("cli %0;" : "=d" (_tmp_dummy) : );	\
	} while (0)

#define local_irq_enable_hw_notrace() \
	__asm__ __volatile__( \
		"sti %0;" \
		: \
		: "d"(bfin_irq_flags) \
	)

#define local_irq_save_hw_notrace(x) __save_and_cli_hw(x)

#define local_irq_restore_hw_notrace(x)			\
	do {						\
		if (local_test_iflag_hw(x))		\
			local_irq_enable_hw_notrace();	\
	} while (0)

#else /* CONFIG_IPIPE_TRACE_IRQSOFF */

#define local_irq_enable_hw() \
	__asm__ __volatile__( \
		"sti %0;" \
		: \
		: "d"(bfin_irq_flags) \
	)

#define local_irq_disable_hw()			\
	do {					\
		int _tmp_dummy;			\
		__asm__ __volatile__ (		\
			"cli %0;"		\
			: "=d" (_tmp_dummy));	\
	} while (0)

#define local_irq_restore_hw(x) \
	do { \
		if (irqs_enabled_from_flags_hw(x)) \
			local_irq_enable_hw(); \
	} while (0)

#define local_irq_save_hw(x)		__save_and_cli_hw(x)

#define local_irq_disable_hw_notrace()	local_irq_disable_hw()
#define local_irq_enable_hw_notrace()	local_irq_enable_hw()
#define local_irq_save_hw_notrace(x)	local_irq_save_hw(x)
#define local_irq_restore_hw_notrace(x)	local_irq_restore_hw(x)

#endif  /* CONFIG_IPIPE_TRACE_IRQSOFF */

#else /* !CONFIG_IPIPE */

/*
 * Interrupt configuring macros.
 */
#define local_irq_disable() \
	do { \
		int __tmp_dummy; \
		__asm__ __volatile__( \
			"cli %0;" \
			: "=d" (__tmp_dummy) \
		); \
	} while (0)

#define local_irq_enable() \
	__asm__ __volatile__( \
		"sti %0;" \
		: \
		: "d" (bfin_irq_flags) \
	)

#ifdef CONFIG_DEBUG_HWERR
# define __save_and_cli(x) \
	__asm__ __volatile__( \
		"cli %0;" \
		"sti %1;" \
		: "=&d" (x) \
		: "d" (0x3F) \
	)
#else
# define __save_and_cli(x) \
	__asm__ __volatile__( \
		"cli %0;" \
		: "=&d" (x) \
	)
#endif

#define local_save_flags(x) \
	__asm__ __volatile__( \
		"cli %0;" \
		"sti %0;" \
		: "=d" (x) \
	)

#ifdef CONFIG_DEBUG_HWERR
#define irqs_enabled_from_flags(x) (((x) & ~0x3f) != 0)
#else
#define irqs_enabled_from_flags(x) ((x) != 0x1f)
#endif

#define local_irq_restore(x) \
	do { \
		if (irqs_enabled_from_flags(x)) \
			local_irq_enable(); \
	} while (0)

/* For spinlocks etc */
#define local_irq_save(x) __save_and_cli(x)

#define irqs_disabled()				\
({						\
	unsigned long flags;			\
	local_save_flags(flags);		\
	!irqs_enabled_from_flags(flags);	\
})

#define local_irq_save_hw(x)		local_irq_save(x)
#define local_irq_restore_hw(x)		local_irq_restore(x)
#define local_irq_enable_hw()		local_irq_enable()
#define local_irq_disable_hw()		local_irq_disable()
#define irqs_disabled_hw()		irqs_disabled()

#endif /* !CONFIG_IPIPE */

#if ANOMALY_05000244 && defined(CONFIG_BFIN_ICACHE)
# define NOP_PAD_ANOMALY_05000244 "nop; nop;"
#else
# define NOP_PAD_ANOMALY_05000244
#endif

#define idle_with_irq_disabled() \
	__asm__ __volatile__( \
		NOP_PAD_ANOMALY_05000244 \
		".align 8;" \
		"sti %0;" \
		"idle;" \
		: \
		: "d" (bfin_irq_flags) \
	)

static inline int irq_canonicalize(int irq)
{
	return irq;
}

#endif				/* _BFIN_IRQ_H_ */
