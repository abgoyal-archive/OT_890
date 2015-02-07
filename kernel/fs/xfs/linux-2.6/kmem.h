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
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef __XFS_SUPPORT_KMEM_H__
#define __XFS_SUPPORT_KMEM_H__

#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/mm.h>

/*
 * General memory allocation interfaces
 */

#define KM_SLEEP	0x0001u
#define KM_NOSLEEP	0x0002u
#define KM_NOFS		0x0004u
#define KM_MAYFAIL	0x0008u
#define KM_LARGE	0x0010u

/*
 * We use a special process flag to avoid recursive callbacks into
 * the filesystem during transactions.  We will also issue our own
 * warnings, so we explicitly skip any generic ones (silly of us).
 */
static inline gfp_t
kmem_flags_convert(unsigned int __nocast flags)
{
	gfp_t	lflags;

	BUG_ON(flags & ~(KM_SLEEP|KM_NOSLEEP|KM_NOFS|KM_MAYFAIL|KM_LARGE));

	if (flags & KM_NOSLEEP) {
		lflags = GFP_ATOMIC | __GFP_NOWARN;
	} else {
		lflags = GFP_KERNEL | __GFP_NOWARN;
		if ((current->flags & PF_FSTRANS) || (flags & KM_NOFS))
			lflags &= ~__GFP_FS;
	}
	return lflags;
}

extern void *kmem_alloc(size_t, unsigned int __nocast);
extern void *kmem_zalloc(size_t, unsigned int __nocast);
extern void *kmem_zalloc_greedy(size_t *, size_t, size_t, unsigned int __nocast);
extern void *kmem_realloc(const void *, size_t, size_t, unsigned int __nocast);
extern void  kmem_free(const void *);

/*
 * Zone interfaces
 */

#define KM_ZONE_HWALIGN	SLAB_HWCACHE_ALIGN
#define KM_ZONE_RECLAIM	SLAB_RECLAIM_ACCOUNT
#define KM_ZONE_SPREAD	SLAB_MEM_SPREAD

#define kmem_zone	kmem_cache
#define kmem_zone_t	struct kmem_cache

static inline kmem_zone_t *
kmem_zone_init(int size, char *zone_name)
{
	return kmem_cache_create(zone_name, size, 0, 0, NULL);
}

static inline kmem_zone_t *
kmem_zone_init_flags(int size, char *zone_name, unsigned long flags,
		     void (*construct)(void *))
{
	return kmem_cache_create(zone_name, size, 0, flags, construct);
}

static inline void
kmem_zone_free(kmem_zone_t *zone, void *ptr)
{
	kmem_cache_free(zone, ptr);
}

static inline void
kmem_zone_destroy(kmem_zone_t *zone)
{
	if (zone)
		kmem_cache_destroy(zone);
}

extern void *kmem_zone_alloc(kmem_zone_t *, unsigned int __nocast);
extern void *kmem_zone_zalloc(kmem_zone_t *, unsigned int __nocast);

static inline int
kmem_shake_allow(gfp_t gfp_mask)
{
	return (gfp_mask & __GFP_WAIT) != 0;
}

#endif /* __XFS_SUPPORT_KMEM_H__ */
