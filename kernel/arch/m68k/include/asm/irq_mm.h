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

#ifndef _M68K_IRQ_H_
#define _M68K_IRQ_H_

#include <linux/linkage.h>
#include <linux/hardirq.h>
#include <linux/spinlock_types.h>

/*
 * This should be the same as the max(NUM_X_SOURCES) for all the
 * different m68k hosts compiled into the kernel.
 * Currently the Atari has 72 and the Amiga 24, but if both are
 * supported in the kernel it is better to make room for 72.
 */
#if defined(CONFIG_VME) || defined(CONFIG_SUN3) || defined(CONFIG_SUN3X)
#define NR_IRQS 200
#elif defined(CONFIG_ATARI) || defined(CONFIG_MAC)
#define NR_IRQS 72
#elif defined(CONFIG_Q40)
#define NR_IRQS	43
#elif defined(CONFIG_AMIGA)
#define NR_IRQS	32
#elif defined(CONFIG_APOLLO)
#define NR_IRQS	24
#elif defined(CONFIG_HP300)
#define NR_IRQS	8
#else
#define NR_IRQS	0
#endif

/*
 * The hardirq mask has to be large enough to have
 * space for potentially all IRQ sources in the system
 * nesting on a single CPU:
 */
#if (1 << HARDIRQ_BITS) < NR_IRQS
# error HARDIRQ_BITS is too low!
#endif

/*
 * Interrupt source definitions
 * General interrupt sources are the level 1-7.
 * Adding an interrupt service routine for one of these sources
 * results in the addition of that routine to a chain of routines.
 * Each one is called in succession.  Each individual interrupt
 * service routine should determine if the device associated with
 * that routine requires service.
 */

#define IRQ_SPURIOUS	0

#define IRQ_AUTO_1	1	/* level 1 interrupt */
#define IRQ_AUTO_2	2	/* level 2 interrupt */
#define IRQ_AUTO_3	3	/* level 3 interrupt */
#define IRQ_AUTO_4	4	/* level 4 interrupt */
#define IRQ_AUTO_5	5	/* level 5 interrupt */
#define IRQ_AUTO_6	6	/* level 6 interrupt */
#define IRQ_AUTO_7	7	/* level 7 interrupt (non-maskable) */

#define IRQ_USER	8

extern unsigned int irq_canonicalize(unsigned int irq);

struct pt_regs;

/*
 * various flags for request_irq() - the Amiga now uses the standard
 * mechanism like all other architectures - IRQF_DISABLED and
 * IRQF_SHARED are your friends.
 */
#ifndef MACH_AMIGA_ONLY
#define IRQ_FLG_LOCK	(0x0001)	/* handler is not replaceable	*/
#define IRQ_FLG_REPLACE	(0x0002)	/* replace existing handler	*/
#define IRQ_FLG_FAST	(0x0004)
#define IRQ_FLG_SLOW	(0x0008)
#define IRQ_FLG_STD	(0x8000)	/* internally used		*/
#endif

/*
 * This structure is used to chain together the ISRs for a particular
 * interrupt source (if it supports chaining).
 */
typedef struct irq_node {
	int		(*handler)(int, void *);
	void		*dev_id;
	struct irq_node *next;
	unsigned long	flags;
	const char	*devname;
} irq_node_t;

/*
 * This structure has only 4 elements for speed reasons
 */
struct irq_handler {
	int		(*handler)(int, void *);
	unsigned long	flags;
	void		*dev_id;
	const char	*devname;
};

struct irq_controller {
	const char *name;
	spinlock_t lock;
	int (*startup)(unsigned int irq);
	void (*shutdown)(unsigned int irq);
	void (*enable)(unsigned int irq);
	void (*disable)(unsigned int irq);
};

extern int m68k_irq_startup(unsigned int);
extern void m68k_irq_shutdown(unsigned int);

/*
 * This function returns a new irq_node_t
 */
extern irq_node_t *new_irq_node(void);

extern void m68k_setup_auto_interrupt(void (*handler)(unsigned int, struct pt_regs *));
extern void m68k_setup_user_interrupt(unsigned int vec, unsigned int cnt,
				      void (*handler)(unsigned int, struct pt_regs *));
extern void m68k_setup_irq_controller(struct irq_controller *, unsigned int, unsigned int);

asmlinkage void m68k_handle_int(unsigned int);
asmlinkage void __m68k_handle_int(unsigned int, struct pt_regs *);

#endif /* _M68K_IRQ_H_ */
