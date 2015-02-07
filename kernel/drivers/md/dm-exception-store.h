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
 * Copyright (C) 2001-2002 Sistina Software (UK) Limited.
 * Copyright (C) 2008 Red Hat, Inc. All rights reserved.
 *
 * Device-mapper snapshot exception store.
 *
 * This file is released under the GPL.
 */

#ifndef _LINUX_DM_EXCEPTION_STORE
#define _LINUX_DM_EXCEPTION_STORE

#include <linux/blkdev.h>
#include <linux/device-mapper.h>

/*
 * The snapshot code deals with largish chunks of the disk at a
 * time. Typically 32k - 512k.
 */
typedef sector_t chunk_t;

/*
 * An exception is used where an old chunk of data has been
 * replaced by a new one.
 * If chunk_t is 64 bits in size, the top 8 bits of new_chunk hold the number
 * of chunks that follow contiguously.  Remaining bits hold the number of the
 * chunk within the device.
 */
struct dm_snap_exception {
	struct list_head hash_list;

	chunk_t old_chunk;
	chunk_t new_chunk;
};

/*
 * Abstraction to handle the meta/layout of exception stores (the
 * COW device).
 */
struct dm_exception_store {
	/*
	 * Destroys this object when you've finished with it.
	 */
	void (*destroy) (struct dm_exception_store *store);

	/*
	 * The target shouldn't read the COW device until this is
	 * called.  As exceptions are read from the COW, they are
	 * reported back via the callback.
	 */
	int (*read_metadata) (struct dm_exception_store *store,
			      int (*callback)(void *callback_context,
					      chunk_t old, chunk_t new),
			      void *callback_context);

	/*
	 * Find somewhere to store the next exception.
	 */
	int (*prepare_exception) (struct dm_exception_store *store,
				  struct dm_snap_exception *e);

	/*
	 * Update the metadata with this exception.
	 */
	void (*commit_exception) (struct dm_exception_store *store,
				  struct dm_snap_exception *e,
				  void (*callback) (void *, int success),
				  void *callback_context);

	/*
	 * The snapshot is invalid, note this in the metadata.
	 */
	void (*drop_snapshot) (struct dm_exception_store *store);

	int (*status) (struct dm_exception_store *store, status_type_t status,
		       char *result, unsigned int maxlen);

	/*
	 * Return how full the snapshot is.
	 */
	void (*fraction_full) (struct dm_exception_store *store,
			       sector_t *numerator,
			       sector_t *denominator);

	struct dm_snapshot *snap;
	void *context;
};

/*
 * Funtions to manipulate consecutive chunks
 */
#  if defined(CONFIG_LBD) || (BITS_PER_LONG == 64)
#    define DM_CHUNK_CONSECUTIVE_BITS 8
#    define DM_CHUNK_NUMBER_BITS 56

static inline chunk_t dm_chunk_number(chunk_t chunk)
{
	return chunk & (chunk_t)((1ULL << DM_CHUNK_NUMBER_BITS) - 1ULL);
}

static inline unsigned dm_consecutive_chunk_count(struct dm_snap_exception *e)
{
	return e->new_chunk >> DM_CHUNK_NUMBER_BITS;
}

static inline void dm_consecutive_chunk_count_inc(struct dm_snap_exception *e)
{
	e->new_chunk += (1ULL << DM_CHUNK_NUMBER_BITS);

	BUG_ON(!dm_consecutive_chunk_count(e));
}

#  else
#    define DM_CHUNK_CONSECUTIVE_BITS 0

static inline chunk_t dm_chunk_number(chunk_t chunk)
{
	return chunk;
}

static inline unsigned dm_consecutive_chunk_count(struct dm_snap_exception *e)
{
	return 0;
}

static inline void dm_consecutive_chunk_count_inc(struct dm_snap_exception *e)
{
}

#  endif

int dm_exception_store_init(void);
void dm_exception_store_exit(void);

/*
 * Two exception store implementations.
 */
int dm_persistent_snapshot_init(void);
void dm_persistent_snapshot_exit(void);

int dm_transient_snapshot_init(void);
void dm_transient_snapshot_exit(void);

int dm_create_persistent(struct dm_exception_store *store);

int dm_create_transient(struct dm_exception_store *store);

#endif /* _LINUX_DM_EXCEPTION_STORE */
