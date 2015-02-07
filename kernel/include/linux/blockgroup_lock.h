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

#ifndef _LINUX_BLOCKGROUP_LOCK_H
#define _LINUX_BLOCKGROUP_LOCK_H
/*
 * Per-blockgroup locking for ext2 and ext3.
 *
 * Simple hashed spinlocking.
 */

#include <linux/spinlock.h>
#include <linux/cache.h>

#ifdef CONFIG_SMP

/*
 * We want a power-of-two.  Is there a better way than this?
 */

#if NR_CPUS >= 32
#define NR_BG_LOCKS	128
#elif NR_CPUS >= 16
#define NR_BG_LOCKS	64
#elif NR_CPUS >= 8
#define NR_BG_LOCKS	32
#elif NR_CPUS >= 4
#define NR_BG_LOCKS	16
#elif NR_CPUS >= 2
#define NR_BG_LOCKS	8
#else
#define NR_BG_LOCKS	4
#endif

#else	/* CONFIG_SMP */
#define NR_BG_LOCKS	1
#endif	/* CONFIG_SMP */

struct bgl_lock {
	spinlock_t lock;
} ____cacheline_aligned_in_smp;

struct blockgroup_lock {
	struct bgl_lock locks[NR_BG_LOCKS];
};

static inline void bgl_lock_init(struct blockgroup_lock *bgl)
{
	int i;

	for (i = 0; i < NR_BG_LOCKS; i++)
		spin_lock_init(&bgl->locks[i].lock);
}

/*
 * The accessor is a macro so we can embed a blockgroup_lock into different
 * superblock types
 */
static inline spinlock_t *
bgl_lock_ptr(struct blockgroup_lock *bgl, unsigned int block_group)
{
	return &bgl->locks[(block_group) & (NR_BG_LOCKS-1)].lock;
}

#endif
