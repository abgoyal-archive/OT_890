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
 * struct flchip definition
 *
 * Contains information about the location and state of a given flash device
 *
 * (C) 2000 Red Hat. GPLd.
 */

#ifndef __MTD_FLASHCHIP_H__
#define __MTD_FLASHCHIP_H__

/* For spinlocks. sched.h includes spinlock.h from whichever directory it
 * happens to be in - so we don't have to care whether we're on 2.2, which
 * has asm/spinlock.h, or 2.4, which has linux/spinlock.h
 */
#include <linux/sched.h>

typedef enum {
	FL_READY,
	FL_STATUS,
	FL_CFI_QUERY,
	FL_JEDEC_QUERY,
	FL_ERASING,
	FL_ERASE_SUSPENDING,
	FL_ERASE_SUSPENDED,
	FL_WRITING,
	FL_WRITING_TO_BUFFER,
	FL_OTP_WRITE,
	FL_WRITE_SUSPENDING,
	FL_WRITE_SUSPENDED,
	FL_PM_SUSPENDED,
	FL_SYNCING,
	FL_UNLOADING,
	FL_LOCKING,
	FL_UNLOCKING,
	FL_POINT,
	FL_XIP_WHILE_ERASING,
	FL_XIP_WHILE_WRITING,
	FL_SHUTDOWN,
	FL_UNKNOWN
} flstate_t;



/* NOTE: confusingly, this can be used to refer to more than one chip at a time,
   if they're interleaved.  This can even refer to individual partitions on
   the same physical chip when present. */

struct flchip {
	unsigned long start; /* Offset within the map */
	//	unsigned long len;
	/* We omit len for now, because when we group them together
	   we insist that they're all of the same size, and the chip size
	   is held in the next level up. If we get more versatile later,
	   it'll make it a damn sight harder to find which chip we want from
	   a given offset, and we'll want to add the per-chip length field
	   back in.
	*/
	int ref_point_counter;
	flstate_t state;
	flstate_t oldstate;

	unsigned int write_suspended:1;
	unsigned int erase_suspended:1;
	unsigned long in_progress_block_addr;

	spinlock_t *mutex;
	spinlock_t _spinlock; /* We do it like this because sometimes they'll be shared. */
	wait_queue_head_t wq; /* Wait on here when we're waiting for the chip
			     to be ready */
	int word_write_time;
	int buffer_write_time;
	int erase_time;

	int word_write_time_max;
	int buffer_write_time_max;
	int erase_time_max;

	void *priv;
};

/* This is used to handle contention on write/erase operations
   between partitions of the same physical chip. */
struct flchip_shared {
	spinlock_t lock;
	struct flchip *writing;
	struct flchip *erasing;
};


#endif /* __MTD_FLASHCHIP_H__ */
