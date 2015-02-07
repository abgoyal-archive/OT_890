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
 * arch/arm/mach-sa1100/include/mach/hardware.h
 *
 * Copyright (C) 1998 Nicolas Pitre <nico@cam.org>
 *
 * This file contains the hardware definitions for SA1100 architecture
 *
 * 2000/05/23 John Dorsey <john+@cs.cmu.edu>
 *      Definitions for SA1111 added.
 */

#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H


#define UNCACHEABLE_ADDR	0xfa050000


/*
 * SA1100 internal I/O mappings
 *
 * We have the following mapping:
 *      phys            virt
 *      80000000        f8000000
 *      90000000        fa000000
 *      a0000000        fc000000
 *      b0000000        fe000000
 */

#define VIO_BASE        0xf8000000	/* virtual start of IO space */
#define VIO_SHIFT       3		/* x = IO space shrink power */
#define PIO_START       0x80000000	/* physical start of IO space */

#define io_p2v( x )             \
   ( (((x)&0x00ffffff) | (((x)&0x30000000)>>VIO_SHIFT)) + VIO_BASE )
#define io_v2p( x )             \
   ( (((x)&0x00ffffff) | (((x)&(0x30000000>>VIO_SHIFT))<<VIO_SHIFT)) + PIO_START )

#define CPU_SA1110_A0	(0)
#define CPU_SA1110_B0	(4)
#define CPU_SA1110_B1	(5)
#define CPU_SA1110_B2	(6)
#define CPU_SA1110_B4	(8)

#define CPU_SA1100_ID	(0x4401a110)
#define CPU_SA1100_MASK	(0xfffffff0)
#define CPU_SA1110_ID	(0x6901b110)
#define CPU_SA1110_MASK	(0xfffffff0)

#ifndef __ASSEMBLY__

#include <asm/cputype.h>

#define CPU_REVISION	(read_cpuid_id() & 15)

#define cpu_is_sa1100()	((read_cpuid_id() & CPU_SA1100_MASK) == CPU_SA1100_ID)
#define cpu_is_sa1110()	((read_cpuid_id() & CPU_SA1110_MASK) == CPU_SA1110_ID)

# define __REG(x)	(*((volatile unsigned long *)io_p2v(x)))
# define __PREG(x)	(io_v2p((unsigned long)&(x)))

static inline unsigned long get_clock_tick_rate(void)
{
	return 3686400;
}
#else

# define __REG(x)	io_p2v(x)
# define __PREG(x)	io_v2p(x)

#endif

#include "SA-1100.h"

#ifdef CONFIG_SA1101
#include "SA-1101.h"
#endif

#endif  /* _ASM_ARCH_HARDWARE_H */
