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
 * Copyright (C) 2007 Oracle.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 */

#ifndef __BTRFS_ASYNC_THREAD_
#define __BTRFS_ASYNC_THREAD_

struct btrfs_worker_thread;

/*
 * This is similar to a workqueue, but it is meant to spread the operations
 * across all available cpus instead of just the CPU that was used to
 * queue the work.  There is also some batching introduced to try and
 * cut down on context switches.
 *
 * By default threads are added on demand up to 2 * the number of cpus.
 * Changing struct btrfs_workers->max_workers is one way to prevent
 * demand creation of kthreads.
 *
 * the basic model of these worker threads is to embed a btrfs_work
 * structure in your own data struct, and use container_of in a
 * work function to get back to your data struct.
 */
struct btrfs_work {
	/*
	 * func should be set to the function you want called
	 * your work struct is passed as the only arg
	 *
	 * ordered_func must be set for work sent to an ordered work queue,
	 * and it is called to complete a given work item in the same
	 * order they were sent to the queue.
	 */
	void (*func)(struct btrfs_work *work);
	void (*ordered_func)(struct btrfs_work *work);
	void (*ordered_free)(struct btrfs_work *work);

	/*
	 * flags should be set to zero.  It is used to make sure the
	 * struct is only inserted once into the list.
	 */
	unsigned long flags;

	/* don't touch these */
	struct btrfs_worker_thread *worker;
	struct list_head list;
	struct list_head order_list;
};

struct btrfs_workers {
	/* current number of running workers */
	int num_workers;

	/* max number of workers allowed.  changed by btrfs_start_workers */
	int max_workers;

	/* once a worker has this many requests or fewer, it is idle */
	int idle_thresh;

	/* force completions in the order they were queued */
	int ordered;

	/* list with all the work threads.  The workers on the idle thread
	 * may be actively servicing jobs, but they haven't yet hit the
	 * idle thresh limit above.
	 */
	struct list_head worker_list;
	struct list_head idle_list;

	/*
	 * when operating in ordered mode, this maintains the list
	 * of work items waiting for completion
	 */
	struct list_head order_list;

	/* lock for finding the next worker thread to queue on */
	spinlock_t lock;

	/* extra name for this worker, used for current->name */
	char *name;
};

int btrfs_queue_worker(struct btrfs_workers *workers, struct btrfs_work *work);
int btrfs_start_workers(struct btrfs_workers *workers, int num_workers);
int btrfs_stop_workers(struct btrfs_workers *workers);
void btrfs_init_workers(struct btrfs_workers *workers, char *name, int max);
int btrfs_requeue_work(struct btrfs_work *work);
#endif
