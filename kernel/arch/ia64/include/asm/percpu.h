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

#ifndef _ASM_IA64_PERCPU_H
#define _ASM_IA64_PERCPU_H

/*
 * Copyright (C) 2002-2003 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 */

#define PERCPU_ENOUGH_ROOM PERCPU_PAGE_SIZE

#ifdef __ASSEMBLY__
# define THIS_CPU(var)	(per_cpu__##var)  /* use this to mark accesses to per-CPU variables... */
#else /* !__ASSEMBLY__ */


#include <linux/threads.h>

#ifdef CONFIG_SMP

#ifdef HAVE_MODEL_SMALL_ATTRIBUTE
# define PER_CPU_ATTRIBUTES	__attribute__((__model__ (__small__)))
#endif

#define __my_cpu_offset	__ia64_per_cpu_var(local_per_cpu_offset)

extern void *per_cpu_init(void);

#else /* ! SMP */

#define PER_CPU_ATTRIBUTES	__attribute__((__section__(".data.percpu")))

#define per_cpu_init()				(__phys_per_cpu_start)

#endif	/* SMP */

/*
 * Be extremely careful when taking the address of this variable!  Due to virtual
 * remapping, it is different from the canonical address returned by __get_cpu_var(var)!
 * On the positive side, using __ia64_per_cpu_var() instead of __get_cpu_var() is slightly
 * more efficient.
 */
#define __ia64_per_cpu_var(var)	per_cpu__##var

#include <asm-generic/percpu.h>

/* Equal to __per_cpu_offset[smp_processor_id()], but faster to access: */
DECLARE_PER_CPU(unsigned long, local_per_cpu_offset);

#endif /* !__ASSEMBLY__ */

#endif /* _ASM_IA64_PERCPU_H */
