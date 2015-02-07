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
 * include/linux/pagevec.h
 *
 * In many places it is efficient to batch an operation up against multiple
 * pages.  A pagevec is a multipage container which is used for that.
 */

#ifndef _LINUX_PAGEVEC_H
#define _LINUX_PAGEVEC_H

/* 14 pointers + two long's align the pagevec structure to a power of two */
#define PAGEVEC_SIZE	14

struct page;
struct address_space;

struct pagevec {
	unsigned long nr;
	unsigned long cold;
	struct page *pages[PAGEVEC_SIZE];
};

void __pagevec_release(struct pagevec *pvec);
void __pagevec_free(struct pagevec *pvec);
void ____pagevec_lru_add(struct pagevec *pvec, enum lru_list lru);
void pagevec_strip(struct pagevec *pvec);
void pagevec_swap_free(struct pagevec *pvec);
unsigned pagevec_lookup(struct pagevec *pvec, struct address_space *mapping,
		pgoff_t start, unsigned nr_pages);
unsigned pagevec_lookup_tag(struct pagevec *pvec,
		struct address_space *mapping, pgoff_t *index, int tag,
		unsigned nr_pages);

static inline void pagevec_init(struct pagevec *pvec, int cold)
{
	pvec->nr = 0;
	pvec->cold = cold;
}

static inline void pagevec_reinit(struct pagevec *pvec)
{
	pvec->nr = 0;
}

static inline unsigned pagevec_count(struct pagevec *pvec)
{
	return pvec->nr;
}

static inline unsigned pagevec_space(struct pagevec *pvec)
{
	return PAGEVEC_SIZE - pvec->nr;
}

/*
 * Add a page to a pagevec.  Returns the number of slots still available.
 */
static inline unsigned pagevec_add(struct pagevec *pvec, struct page *page)
{
	pvec->pages[pvec->nr++] = page;
	return pagevec_space(pvec);
}


static inline void pagevec_release(struct pagevec *pvec)
{
	if (pagevec_count(pvec))
		__pagevec_release(pvec);
}

static inline void pagevec_free(struct pagevec *pvec)
{
	if (pagevec_count(pvec))
		__pagevec_free(pvec);
}

static inline void __pagevec_lru_add_anon(struct pagevec *pvec)
{
	____pagevec_lru_add(pvec, LRU_INACTIVE_ANON);
}

static inline void __pagevec_lru_add_active_anon(struct pagevec *pvec)
{
	____pagevec_lru_add(pvec, LRU_ACTIVE_ANON);
}

static inline void __pagevec_lru_add_file(struct pagevec *pvec)
{
	____pagevec_lru_add(pvec, LRU_INACTIVE_FILE);
}

static inline void __pagevec_lru_add_active_file(struct pagevec *pvec)
{
	____pagevec_lru_add(pvec, LRU_ACTIVE_FILE);
}

static inline void pagevec_lru_add_file(struct pagevec *pvec)
{
	if (pagevec_count(pvec))
		__pagevec_lru_add_file(pvec);
}

static inline void pagevec_lru_add_anon(struct pagevec *pvec)
{
	if (pagevec_count(pvec))
		__pagevec_lru_add_anon(pvec);
}

#endif /* _LINUX_PAGEVEC_H */
