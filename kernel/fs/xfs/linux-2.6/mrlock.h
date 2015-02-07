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
 * Copyright (c) 2000-2006 Silicon Graphics, Inc.
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
#ifndef __XFS_SUPPORT_MRLOCK_H__
#define __XFS_SUPPORT_MRLOCK_H__

#include <linux/rwsem.h>

typedef struct {
	struct rw_semaphore	mr_lock;
#ifdef DEBUG
	int			mr_writer;
#endif
} mrlock_t;

#ifdef DEBUG
#define mrinit(mrp, name)	\
	do { (mrp)->mr_writer = 0; init_rwsem(&(mrp)->mr_lock); } while (0)
#else
#define mrinit(mrp, name)	\
	do { init_rwsem(&(mrp)->mr_lock); } while (0)
#endif

#define mrlock_init(mrp, t,n,s)	mrinit(mrp, n)
#define mrfree(mrp)		do { } while (0)

static inline void mraccess_nested(mrlock_t *mrp, int subclass)
{
	down_read_nested(&mrp->mr_lock, subclass);
}

static inline void mrupdate_nested(mrlock_t *mrp, int subclass)
{
	down_write_nested(&mrp->mr_lock, subclass);
#ifdef DEBUG
	mrp->mr_writer = 1;
#endif
}

static inline int mrtryaccess(mrlock_t *mrp)
{
	return down_read_trylock(&mrp->mr_lock);
}

static inline int mrtryupdate(mrlock_t *mrp)
{
	if (!down_write_trylock(&mrp->mr_lock))
		return 0;
#ifdef DEBUG
	mrp->mr_writer = 1;
#endif
	return 1;
}

static inline void mrunlock_excl(mrlock_t *mrp)
{
#ifdef DEBUG
	mrp->mr_writer = 0;
#endif
	up_write(&mrp->mr_lock);
}

static inline void mrunlock_shared(mrlock_t *mrp)
{
	up_read(&mrp->mr_lock);
}

static inline void mrdemote(mrlock_t *mrp)
{
#ifdef DEBUG
	mrp->mr_writer = 0;
#endif
	downgrade_write(&mrp->mr_lock);
}

#endif /* __XFS_SUPPORT_MRLOCK_H__ */
