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
 * arch/sh/boards/dreamcast/irq.c
 *
 * Holly IRQ support for the Sega Dreamcast.
 *
 * Copyright (c) 2001, 2002 M. R. Brown <mrbrown@0xd6.org>
 *
 * This file is part of the LinuxDC project (www.linuxdc.org)
 * Released under the terms of the GNU GPL v2.0
 */

#include <linux/irq.h>
#include <linux/io.h>
#include <asm/irq.h>
#include <mach/sysasic.h>

/*
 * Dreamcast System ASIC Hardware Events -
 *
 * The Dreamcast's System ASIC (a.k.a. Holly) is responsible for receiving
 * hardware events from system peripherals and triggering an SH7750 IRQ.
 * Hardware events can trigger IRQs 13, 11, or 9 depending on which bits are
 * set in the Event Mask Registers (EMRs).  When a hardware event is
 * triggered, its corresponding bit in the Event Status Registers (ESRs)
 * is set, and that bit should be rewritten to the ESR to acknowledge that
 * event.
 *
 * There are three 32-bit ESRs located at 0xa05f6900 - 0xa05f6908.  Event
 * types can be found in arch/sh/include/mach-dreamcast/mach/sysasic.h.
 * There are three groups of EMRs that parallel the ESRs.  Each EMR group
 * corresponds to an IRQ, so 0xa05f6910 - 0xa05f6918 triggers IRQ 13,
 * 0xa05f6920 - 0xa05f6928 triggers IRQ 11, and 0xa05f6930 - 0xa05f6938
 * triggers IRQ 9.
 *
 * In the kernel, these events are mapped to virtual IRQs so that drivers can
 * respond to them as they would a normal interrupt.  In order to keep this
 * mapping simple, the events are mapped as:
 *
 * 6900/6910 - Events  0-31, IRQ 13
 * 6904/6924 - Events 32-63, IRQ 11
 * 6908/6938 - Events 64-95, IRQ  9
 *
 */

#define ESR_BASE 0x005f6900    /* Base event status register */
#define EMR_BASE 0x005f6910    /* Base event mask register */

/*
 * Helps us determine the EMR group that this event belongs to: 0 = 0x6910,
 * 1 = 0x6920, 2 = 0x6930; also determine the event offset.
 */
#define LEVEL(event) (((event) - HW_EVENT_IRQ_BASE) / 32)

/* Return the hardware event's bit positon within the EMR/ESR */
#define EVENT_BIT(event) (((event) - HW_EVENT_IRQ_BASE) & 31)

/*
 * For each of these *_irq routines, the IRQ passed in is the virtual IRQ
 * (logically mapped to the corresponding bit for the hardware event).
 */

/* Disable the hardware event by masking its bit in its EMR */
static inline void disable_systemasic_irq(unsigned int irq)
{
	__u32 emr = EMR_BASE + (LEVEL(irq) << 4) + (LEVEL(irq) << 2);
	__u32 mask;

	mask = inl(emr);
	mask &= ~(1 << EVENT_BIT(irq));
	outl(mask, emr);
}

/* Enable the hardware event by setting its bit in its EMR */
static inline void enable_systemasic_irq(unsigned int irq)
{
	__u32 emr = EMR_BASE + (LEVEL(irq) << 4) + (LEVEL(irq) << 2);
	__u32 mask;

	mask = inl(emr);
	mask |= (1 << EVENT_BIT(irq));
	outl(mask, emr);
}

/* Acknowledge a hardware event by writing its bit back to its ESR */
static void mask_ack_systemasic_irq(unsigned int irq)
{
	__u32 esr = ESR_BASE + (LEVEL(irq) << 2);
	disable_systemasic_irq(irq);
	outl((1 << EVENT_BIT(irq)), esr);
}

struct irq_chip systemasic_int = {
	.name		= "System ASIC",
	.mask		= disable_systemasic_irq,
	.mask_ack	= mask_ack_systemasic_irq,
	.unmask		= enable_systemasic_irq,
};

/*
 * Map the hardware event indicated by the processor IRQ to a virtual IRQ.
 */
int systemasic_irq_demux(int irq)
{
	__u32 emr, esr, status, level;
	__u32 j, bit;

	switch (irq) {
	case 13:
		level = 0;
		break;
	case 11:
		level = 1;
		break;
	case  9:
		level = 2;
		break;
	default:
		return irq;
	}
	emr = EMR_BASE + (level << 4) + (level << 2);
	esr = ESR_BASE + (level << 2);

	/* Mask the ESR to filter any spurious, unwanted interrupts */
	status = inl(esr);
	status &= inl(emr);

	/* Now scan and find the first set bit as the event to map */
	for (bit = 1, j = 0; j < 32; bit <<= 1, j++) {
		if (status & bit) {
			irq = HW_EVENT_IRQ_BASE + j + (level << 5);
			return irq;
		}
	}

	/* Not reached */
	return irq;
}
