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

#ifndef _ALPHA_IRQ_H
#define _ALPHA_IRQ_H

/*
 *	linux/include/alpha/irq.h
 *
 *	(C) 1994 Linus Torvalds
 */

#include <linux/linkage.h>

#if   defined(CONFIG_ALPHA_GENERIC)

/* Here NR_IRQS is not exact, but rather an upper bound.  This is used
   many places throughout the kernel to size static arrays.  That's ok,
   we'll use alpha_mv.nr_irqs when we want the real thing.  */

/* When LEGACY_START_ADDRESS is selected, we leave out:
     TITAN
     WILDFIRE
     MARVEL

   This helps keep the kernel object size reasonable for the majority
   of machines.
*/

# if defined(CONFIG_ALPHA_LEGACY_START_ADDRESS)
#  define NR_IRQS      (128)           /* max is RAWHIDE/TAKARA */
# else
#  define NR_IRQS      (32768 + 16)    /* marvel - 32 pids */
# endif

#elif defined(CONFIG_ALPHA_CABRIOLET) || \
      defined(CONFIG_ALPHA_EB66P)     || \
      defined(CONFIG_ALPHA_EB164)     || \
      defined(CONFIG_ALPHA_PC164)     || \
      defined(CONFIG_ALPHA_LX164)
# define NR_IRQS	35

#elif defined(CONFIG_ALPHA_EB66)      || \
      defined(CONFIG_ALPHA_EB64P)     || \
      defined(CONFIG_ALPHA_MIKASA)
# define NR_IRQS	32

#elif defined(CONFIG_ALPHA_ALCOR)     || \
      defined(CONFIG_ALPHA_MIATA)     || \
      defined(CONFIG_ALPHA_RUFFIAN)   || \
      defined(CONFIG_ALPHA_RX164)     || \
      defined(CONFIG_ALPHA_NORITAKE)
# define NR_IRQS	48

#elif defined(CONFIG_ALPHA_SABLE)     || \
      defined(CONFIG_ALPHA_SX164)
# define NR_IRQS	40

#elif defined(CONFIG_ALPHA_DP264) || \
      defined(CONFIG_ALPHA_LYNX)  || \
      defined(CONFIG_ALPHA_SHARK) || \
      defined(CONFIG_ALPHA_EIGER)
# define NR_IRQS	64

#elif defined(CONFIG_ALPHA_TITAN)
#define NR_IRQS		80

#elif defined(CONFIG_ALPHA_RAWHIDE) || \
	defined(CONFIG_ALPHA_TAKARA)
# define NR_IRQS	128

#elif defined(CONFIG_ALPHA_WILDFIRE)
# define NR_IRQS	2048 /* enuff for 8 QBBs */

#elif defined(CONFIG_ALPHA_MARVEL)
# define NR_IRQS	(32768 + 16) 	/* marvel - 32 pids*/

#else /* everyone else */
# define NR_IRQS	16
#endif

static __inline__ int irq_canonicalize(int irq)
{
	/*
	 * XXX is this true for all Alpha's?  The old serial driver
	 * did it this way for years without any complaints, so....
	 */
	return ((irq == 2) ? 9 : irq);
}

struct pt_regs;
extern void (*perf_irq)(unsigned long, struct pt_regs *);

#endif /* _ALPHA_IRQ_H */
