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

#ifndef _ASM_X86_IPI_H
#define _ASM_X86_IPI_H

/*
 * Copyright 2004 James Cleverdon, IBM.
 * Subject to the GNU Public License, v.2
 *
 * Generic APIC InterProcessor Interrupt code.
 *
 * Moved to include file by James Cleverdon from
 * arch/x86-64/kernel/smp.c
 *
 * Copyrights from kernel/smp.c:
 *
 * (c) 1995 Alan Cox, Building #3 <alan@redhat.com>
 * (c) 1998-99, 2000 Ingo Molnar <mingo@redhat.com>
 * (c) 2002,2003 Andi Kleen, SuSE Labs.
 * Subject to the GNU Public License, v.2
 */

#include <asm/hw_irq.h>
#include <asm/apic.h>
#include <asm/smp.h>

/*
 * the following functions deal with sending IPIs between CPUs.
 *
 * We use 'broadcast', CPU->CPU IPIs and self-IPIs too.
 */

static inline unsigned int __prepare_ICR(unsigned int shortcut, int vector,
					 unsigned int dest)
{
	unsigned int icr = shortcut | dest;

	switch (vector) {
	default:
		icr |= APIC_DM_FIXED | vector;
		break;
	case NMI_VECTOR:
		icr |= APIC_DM_NMI;
		break;
	}
	return icr;
}

static inline int __prepare_ICR2(unsigned int mask)
{
	return SET_APIC_DEST_FIELD(mask);
}

static inline void __xapic_wait_icr_idle(void)
{
	while (native_apic_mem_read(APIC_ICR) & APIC_ICR_BUSY)
		cpu_relax();
}

static inline void __send_IPI_shortcut(unsigned int shortcut, int vector,
				       unsigned int dest)
{
	/*
	 * Subtle. In the case of the 'never do double writes' workaround
	 * we have to lock out interrupts to be safe.  As we don't care
	 * of the value read we use an atomic rmw access to avoid costly
	 * cli/sti.  Otherwise we use an even cheaper single atomic write
	 * to the APIC.
	 */
	unsigned int cfg;

	/*
	 * Wait for idle.
	 */
	__xapic_wait_icr_idle();

	/*
	 * No need to touch the target chip field
	 */
	cfg = __prepare_ICR(shortcut, vector, dest);

	/*
	 * Send the IPI. The write to APIC_ICR fires this off.
	 */
	native_apic_mem_write(APIC_ICR, cfg);
}

/*
 * This is used to send an IPI with no shorthand notation (the destination is
 * specified in bits 56 to 63 of the ICR).
 */
static inline void __send_IPI_dest_field(unsigned int mask, int vector,
					 unsigned int dest)
{
	unsigned long cfg;

	/*
	 * Wait for idle.
	 */
	if (unlikely(vector == NMI_VECTOR))
		safe_apic_wait_icr_idle();
	else
		__xapic_wait_icr_idle();

	/*
	 * prepare target chip field
	 */
	cfg = __prepare_ICR2(mask);
	native_apic_mem_write(APIC_ICR2, cfg);

	/*
	 * program the ICR
	 */
	cfg = __prepare_ICR(0, vector, dest);

	/*
	 * Send the IPI. The write to APIC_ICR fires this off.
	 */
	native_apic_mem_write(APIC_ICR, cfg);
}

static inline void send_IPI_mask_sequence(const struct cpumask *mask,
					  int vector)
{
	unsigned long flags;
	unsigned long query_cpu;

	/*
	 * Hack. The clustered APIC addressing mode doesn't allow us to send
	 * to an arbitrary mask, so I do a unicast to each CPU instead.
	 * - mbligh
	 */
	local_irq_save(flags);
	for_each_cpu(query_cpu, mask) {
		__send_IPI_dest_field(per_cpu(x86_cpu_to_apicid, query_cpu),
				      vector, APIC_DEST_PHYSICAL);
	}
	local_irq_restore(flags);
}

static inline void send_IPI_mask_allbutself(const struct cpumask *mask,
					    int vector)
{
	unsigned long flags;
	unsigned int query_cpu;
	unsigned int this_cpu = smp_processor_id();

	/* See Hack comment above */

	local_irq_save(flags);
	for_each_cpu(query_cpu, mask)
		if (query_cpu != this_cpu)
			__send_IPI_dest_field(
				per_cpu(x86_cpu_to_apicid, query_cpu),
				vector, APIC_DEST_PHYSICAL);
	local_irq_restore(flags);
}

#endif /* _ASM_X86_IPI_H */
