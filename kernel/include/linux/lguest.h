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

/* Things the lguest guest needs to know.  Note: like all lguest interfaces,
 * this is subject to wild and random change between versions. */
#ifndef _LINUX_LGUEST_H
#define _LINUX_LGUEST_H

#ifndef __ASSEMBLY__
#include <linux/time.h>
#include <asm/irq.h>
#include <asm/lguest_hcall.h>

#define LG_CLOCK_MIN_DELTA	100UL
#define LG_CLOCK_MAX_DELTA	ULONG_MAX

/*G:032 The second method of communicating with the Host is to via "struct
 * lguest_data".  Once the Guest's initialization hypercall tells the Host where
 * this is, the Guest and Host both publish information in it. :*/
struct lguest_data
{
	/* 512 == enabled (same as eflags in normal hardware).  The Guest
	 * changes interrupts so often that a hypercall is too slow. */
	unsigned int irq_enabled;
	/* Fine-grained interrupt disabling by the Guest */
	DECLARE_BITMAP(blocked_interrupts, LGUEST_IRQS);

	/* The Host writes the virtual address of the last page fault here,
	 * which saves the Guest a hypercall.  CR2 is the native register where
	 * this address would normally be found. */
	unsigned long cr2;

	/* Wallclock time set by the Host. */
	struct timespec time;

	/* Async hypercall ring.  Instead of directly making hypercalls, we can
	 * place them in here for processing the next time the Host wants.
	 * This batching can be quite efficient. */

	/* 0xFF == done (set by Host), 0 == pending (set by Guest). */
	u8 hcall_status[LHCALL_RING_SIZE];
	/* The actual registers for the hypercalls. */
	struct hcall_args hcalls[LHCALL_RING_SIZE];

/* Fields initialized by the Host at boot: */
	/* Memory not to try to access */
	unsigned long reserve_mem;
	/* KHz for the TSC clock. */
	u32 tsc_khz;
	/* Page where the top-level pagetable is */
	unsigned long pgdir;

/* Fields initialized by the Guest at boot: */
	/* Instruction range to suppress interrupts even if enabled */
	unsigned long noirq_start, noirq_end;
	/* Address above which page tables are all identical. */
	unsigned long kernel_address;
	/* The vector to try to use for system calls (0x40 or 0x80). */
	unsigned int syscall_vec;
};
extern struct lguest_data lguest_data;
#endif /* __ASSEMBLY__ */
#endif	/* _LINUX_LGUEST_H */
