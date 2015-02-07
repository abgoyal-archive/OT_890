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
 * Copyright (C) 2001  Mike Corrigan IBM Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */
#ifndef _ASM_POWERPC_ISERIES_IT_LP_QUEUE_H
#define _ASM_POWERPC_ISERIES_IT_LP_QUEUE_H

/*
 *	This control block defines the simple LP queue structure that is
 *	shared between the hypervisor (PLIC) and the OS in order to send
 *	events to an LP.
 */

#include <asm/types.h>
#include <asm/ptrace.h>

#define IT_LP_MAX_QUEUES	8

#define IT_LP_NOT_USED		0	/* Queue will not be used by PLIC */
#define IT_LP_DEDICATED_IO	1	/* Queue dedicated to IO processor specified */
#define IT_LP_DEDICATED_LP	2	/* Queue dedicated to LP specified */
#define IT_LP_SHARED		3	/* Queue shared for both IO and LP */

#define IT_LP_EVENT_STACK_SIZE	4096
#define IT_LP_EVENT_MAX_SIZE	256
#define IT_LP_EVENT_ALIGN	64

struct hvlpevent_queue {
/*
 * The hq_current_event is the pointer to the next event stack entry
 * that will become valid.  The OS must peek at this entry to determine
 * if it is valid.  PLIC will set the valid indicator as the very last
 * store into that entry.
 *
 * When the OS has completed processing of the event then it will mark
 * the event as invalid so that PLIC knows it can store into that event
 * location again.
 *
 * If the event stack fills and there are overflow events, then PLIC
 * will set the hq_overflow_pending flag in which case the OS will
 * have to fetch the additional LP events once they have drained the
 * event stack.
 *
 * The first 16-bytes are known by both the OS and PLIC.  The remainder
 * of the cache line is for use by the OS.
 */
	u8		hq_overflow_pending;	/* 0x00 Overflow events are pending */
	u8		hq_status;		/* 0x01 DedicatedIo or DedicatedLp or NotUsed */
	u16		hq_proc_index;		/* 0x02 Logical Proc Index for correlation */
	u8		hq_reserved1[12];	/* 0x04 */
	char		*hq_current_event;	/* 0x10 */
	char		*hq_last_event;		/* 0x18 */
	char		*hq_event_stack;	/* 0x20 */
	u8		hq_index;		/* 0x28 unique sequential index. */
	u8		hq_reserved2[3];	/* 0x29-2b */
	spinlock_t	hq_lock;
};

extern struct hvlpevent_queue hvlpevent_queue;

extern int hvlpevent_is_pending(void);
extern void process_hvlpevents(void);
extern void setup_hvlpevent_queue(void);

#endif /* _ASM_POWERPC_ISERIES_IT_LP_QUEUE_H */
