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
 * Copyright (C) 2003 Sistina Software Limited.
 * Copyright (C) 2004-2008 Red Hat, Inc. All rights reserved.
 *
 * Device-Mapper dirty region hash interface.
 *
 * This file is released under the GPL.
 */

#ifndef DM_REGION_HASH_H
#define DM_REGION_HASH_H

#include <linux/dm-dirty-log.h>

/*-----------------------------------------------------------------
 * Region hash
 *----------------------------------------------------------------*/
struct dm_region_hash;
struct dm_region;

/*
 * States a region can have.
 */
enum dm_rh_region_states {
	DM_RH_CLEAN	 = 0x01,	/* No writes in flight. */
	DM_RH_DIRTY	 = 0x02,	/* Writes in flight. */
	DM_RH_NOSYNC	 = 0x04,	/* Out of sync. */
	DM_RH_RECOVERING = 0x08,	/* Under resynchronization. */
};

/*
 * Region hash create/destroy.
 */
struct bio_list;
struct dm_region_hash *dm_region_hash_create(
		void *context, void (*dispatch_bios)(void *context,
						     struct bio_list *bios),
		void (*wakeup_workers)(void *context),
		void (*wakeup_all_recovery_waiters)(void *context),
		sector_t target_begin, unsigned max_recovery,
		struct dm_dirty_log *log, uint32_t region_size,
		region_t nr_regions);
void dm_region_hash_destroy(struct dm_region_hash *rh);

struct dm_dirty_log *dm_rh_dirty_log(struct dm_region_hash *rh);

/*
 * Conversion functions.
 */
region_t dm_rh_bio_to_region(struct dm_region_hash *rh, struct bio *bio);
sector_t dm_rh_region_to_sector(struct dm_region_hash *rh, region_t region);
void *dm_rh_region_context(struct dm_region *reg);

/*
 * Get region size and key (ie. number of the region).
 */
sector_t dm_rh_get_region_size(struct dm_region_hash *rh);
region_t dm_rh_get_region_key(struct dm_region *reg);

/*
 * Get/set/update region state (and dirty log).
 *
 */
int dm_rh_get_state(struct dm_region_hash *rh, region_t region, int may_block);
void dm_rh_set_state(struct dm_region_hash *rh, region_t region,
		     enum dm_rh_region_states state, int may_block);

/* Non-zero errors_handled leaves the state of the region NOSYNC */
void dm_rh_update_states(struct dm_region_hash *rh, int errors_handled);

/* Flush the region hash and dirty log. */
int dm_rh_flush(struct dm_region_hash *rh);

/* Inc/dec pending count on regions. */
void dm_rh_inc_pending(struct dm_region_hash *rh, struct bio_list *bios);
void dm_rh_dec(struct dm_region_hash *rh, region_t region);

/* Delay bios on regions. */
void dm_rh_delay(struct dm_region_hash *rh, struct bio *bio);

void dm_rh_mark_nosync(struct dm_region_hash *rh,
		       struct bio *bio, unsigned done, int error);

/*
 * Region recovery control.
 */

/* Prepare some regions for recovery by starting to quiesce them. */
void dm_rh_recovery_prepare(struct dm_region_hash *rh);

/* Try fetching a quiesced region for recovery. */
struct dm_region *dm_rh_recovery_start(struct dm_region_hash *rh);

/* Report recovery end on a region. */
void dm_rh_recovery_end(struct dm_region *reg, int error);

/* Returns number of regions with recovery work outstanding. */
int dm_rh_recovery_in_flight(struct dm_region_hash *rh);

/* Start/stop recovery. */
void dm_rh_start_recovery(struct dm_region_hash *rh);
void dm_rh_stop_recovery(struct dm_region_hash *rh);

#endif /* DM_REGION_HASH_H */
