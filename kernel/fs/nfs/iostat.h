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
 *  linux/fs/nfs/iostat.h
 *
 *  Declarations for NFS client per-mount statistics
 *
 *  Copyright (C) 2005, 2006 Chuck Lever <cel@netapp.com>
 *
 */

#ifndef _NFS_IOSTAT
#define _NFS_IOSTAT

#include <linux/percpu.h>
#include <linux/cache.h>
#include <linux/nfs_iostat.h>

struct nfs_iostats {
	unsigned long long	bytes[__NFSIOS_BYTESMAX];
	unsigned long		events[__NFSIOS_COUNTSMAX];
} ____cacheline_aligned;

static inline void nfs_inc_server_stats(const struct nfs_server *server,
					enum nfs_stat_eventcounters stat)
{
	struct nfs_iostats *iostats;
	int cpu;

	cpu = get_cpu();
	iostats = per_cpu_ptr(server->io_stats, cpu);
	iostats->events[stat]++;
	put_cpu_no_resched();
}

static inline void nfs_inc_stats(const struct inode *inode,
				 enum nfs_stat_eventcounters stat)
{
	nfs_inc_server_stats(NFS_SERVER(inode), stat);
}

static inline void nfs_add_server_stats(const struct nfs_server *server,
					enum nfs_stat_bytecounters stat,
					unsigned long addend)
{
	struct nfs_iostats *iostats;
	int cpu;

	cpu = get_cpu();
	iostats = per_cpu_ptr(server->io_stats, cpu);
	iostats->bytes[stat] += addend;
	put_cpu_no_resched();
}

static inline void nfs_add_stats(const struct inode *inode,
				 enum nfs_stat_bytecounters stat,
				 unsigned long addend)
{
	nfs_add_server_stats(NFS_SERVER(inode), stat, addend);
}

static inline struct nfs_iostats *nfs_alloc_iostats(void)
{
	return alloc_percpu(struct nfs_iostats);
}

static inline void nfs_free_iostats(struct nfs_iostats *stats)
{
	if (stats != NULL)
		free_percpu(stats);
}

#endif /* _NFS_IOSTAT */
