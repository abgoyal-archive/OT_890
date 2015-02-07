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

/******************************************************************************
 *
 * Name: aclinux.h - OS specific defines, etc.
 *
 *****************************************************************************/

/*
 * Copyright (C) 2000 - 2008, Intel Corp.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    substantially similar to the "NO WARRANTY" disclaimer below
 *    ("Disclaimer") and any redistribution must be conditioned upon
 *    including a substantially similar Disclaimer requirement for further
 *    binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 */

#ifndef __ACLINUX_H__
#define __ACLINUX_H__

#define ACPI_USE_SYSTEM_CLIBRARY
#define ACPI_USE_DO_WHILE_0
#define ACPI_MUTEX_TYPE             ACPI_BINARY_SEMAPHORE

#ifdef __KERNEL__

#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ctype.h>
#include <linux/sched.h>
#include <asm/system.h>
#include <asm/atomic.h>
#include <asm/div64.h>
#include <asm/acpi.h>
#include <linux/slab.h>
#include <linux/spinlock_types.h>
#include <asm/current.h>

/* Host-dependent types and defines */

#define ACPI_MACHINE_WIDTH          BITS_PER_LONG
#define acpi_cache_t                        struct kmem_cache
#define acpi_spinlock                   spinlock_t *
#define ACPI_EXPORT_SYMBOL(symbol)  EXPORT_SYMBOL(symbol);
#define strtoul                     simple_strtoul

#else				/* !__KERNEL__ */

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#if defined(__ia64__) || defined(__x86_64__)
#define ACPI_MACHINE_WIDTH          64
#define COMPILER_DEPENDENT_INT64    long
#define COMPILER_DEPENDENT_UINT64   unsigned long
#else
#define ACPI_MACHINE_WIDTH          32
#define COMPILER_DEPENDENT_INT64    long long
#define COMPILER_DEPENDENT_UINT64   unsigned long long
#define ACPI_USE_NATIVE_DIVIDE
#endif

#ifndef __cdecl
#define __cdecl
#endif

#define ACPI_FLUSH_CPU_CACHE()
#endif				/* __KERNEL__ */

/* Linux uses GCC */

#include "acgcc.h"

#define acpi_cpu_flags unsigned long

#define acpi_thread_id struct task_struct *

static inline acpi_thread_id acpi_os_get_thread_id(void)
{
	return current;
}

/*
 * The irqs_disabled() check is for resume from RAM.
 * Interrupts are off during resume, just like they are for boot.
 * However, boot has  (system_state != SYSTEM_RUNNING)
 * to quiet __might_sleep() in kmalloc() and resume does not.
 */
#include <acpi/actypes.h>
static inline void *acpi_os_allocate(acpi_size size)
{
	return kmalloc(size, irqs_disabled()? GFP_ATOMIC : GFP_KERNEL);
}
static inline void *acpi_os_allocate_zeroed(acpi_size size)
{
	return kzalloc(size, irqs_disabled()? GFP_ATOMIC : GFP_KERNEL);
}

static inline void *acpi_os_acquire_object(acpi_cache_t * cache)
{
	return kmem_cache_zalloc(cache,
				 irqs_disabled()? GFP_ATOMIC : GFP_KERNEL);
}

#define ACPI_ALLOCATE(a)	acpi_os_allocate(a)
#define ACPI_ALLOCATE_ZEROED(a)	acpi_os_allocate_zeroed(a)
#define ACPI_FREE(a)		kfree(a)

/*
 * We need to show where it is safe to preempt execution of ACPICA
 */
#define ACPI_PREEMPTION_POINT()		\
	do {				\
		if (!irqs_disabled())	\
			cond_resched();	\
	} while (0)

#endif				/* __ACLINUX_H__ */
