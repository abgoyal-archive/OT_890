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
  File: linux/mbcache.h

  (C) 2001 by Andreas Gruenbacher, <a.gruenbacher@computer.org>
*/

/* Hardwire the number of additional indexes */
#define MB_CACHE_INDEXES_COUNT 1

struct mb_cache_entry {
	struct list_head		e_lru_list;
	struct mb_cache			*e_cache;
	unsigned short			e_used;
	unsigned short			e_queued;
	struct block_device		*e_bdev;
	sector_t			e_block;
	struct list_head		e_block_list;
	struct {
		struct list_head	o_list;
		unsigned int		o_key;
	} e_indexes[0];
};

struct mb_cache_op {
	int (*free)(struct mb_cache_entry *, gfp_t);
};

/* Functions on caches */

struct mb_cache * mb_cache_create(const char *, struct mb_cache_op *, size_t,
				  int, int);
void mb_cache_shrink(struct block_device *);
void mb_cache_destroy(struct mb_cache *);

/* Functions on cache entries */

struct mb_cache_entry *mb_cache_entry_alloc(struct mb_cache *, gfp_t);
int mb_cache_entry_insert(struct mb_cache_entry *, struct block_device *,
			  sector_t, unsigned int[]);
void mb_cache_entry_release(struct mb_cache_entry *);
void mb_cache_entry_free(struct mb_cache_entry *);
struct mb_cache_entry *mb_cache_entry_get(struct mb_cache *,
					  struct block_device *,
					  sector_t);
#if !defined(MB_CACHE_INDEXES_COUNT) || (MB_CACHE_INDEXES_COUNT > 0)
struct mb_cache_entry *mb_cache_entry_find_first(struct mb_cache *cache, int,
						 struct block_device *, 
						 unsigned int);
struct mb_cache_entry *mb_cache_entry_find_next(struct mb_cache_entry *, int,
						struct block_device *, 
						unsigned int);
#endif
