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

#ifndef LINUX_MM_INLINE_H
#define LINUX_MM_INLINE_H

/**
 * page_is_file_cache - should the page be on a file LRU or anon LRU?
 * @page: the page to test
 *
 * Returns LRU_FILE if @page is page cache page backed by a regular filesystem,
 * or 0 if @page is anonymous, tmpfs or otherwise ram or swap backed.
 * Used by functions that manipulate the LRU lists, to sort a page
 * onto the right LRU list.
 *
 * We would like to get this info without a page flag, but the state
 * needs to survive until the page is last deleted from the LRU, which
 * could be as far down as __page_cache_release.
 */
static inline int page_is_file_cache(struct page *page)
{
	if (PageSwapBacked(page))
		return 0;

	/* The page is page cache backed by a normal filesystem. */
	return LRU_FILE;
}

static inline void
add_page_to_lru_list(struct zone *zone, struct page *page, enum lru_list l)
{
	list_add(&page->lru, &zone->lru[l].list);
	__inc_zone_state(zone, NR_LRU_BASE + l);
	mem_cgroup_add_lru_list(page, l);
}

static inline void
del_page_from_lru_list(struct zone *zone, struct page *page, enum lru_list l)
{
	list_del(&page->lru);
	__dec_zone_state(zone, NR_LRU_BASE + l);
	mem_cgroup_del_lru_list(page, l);
}

static inline void
del_page_from_lru(struct zone *zone, struct page *page)
{
	enum lru_list l = LRU_BASE;

	list_del(&page->lru);
	if (PageUnevictable(page)) {
		__ClearPageUnevictable(page);
		l = LRU_UNEVICTABLE;
	} else {
		if (PageActive(page)) {
			__ClearPageActive(page);
			l += LRU_ACTIVE;
		}
		l += page_is_file_cache(page);
	}
	__dec_zone_state(zone, NR_LRU_BASE + l);
	mem_cgroup_del_lru_list(page, l);
}

/**
 * page_lru - which LRU list should a page be on?
 * @page: the page to test
 *
 * Returns the LRU list a page should be on, as an index
 * into the array of LRU lists.
 */
static inline enum lru_list page_lru(struct page *page)
{
	enum lru_list lru = LRU_BASE;

	if (PageUnevictable(page))
		lru = LRU_UNEVICTABLE;
	else {
		if (PageActive(page))
			lru += LRU_ACTIVE;
		lru += page_is_file_cache(page);
	}

	return lru;
}

#endif
