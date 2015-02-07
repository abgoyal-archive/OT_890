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

#ifndef _LINUX_MIGRATE_H
#define _LINUX_MIGRATE_H

#include <linux/mm.h>
#include <linux/mempolicy.h>

typedef struct page *new_page_t(struct page *, unsigned long private, int **);

#ifdef CONFIG_MIGRATION
#define PAGE_MIGRATION 1

extern int putback_lru_pages(struct list_head *l);
extern int migrate_page(struct address_space *,
			struct page *, struct page *);
extern int migrate_pages(struct list_head *l, new_page_t x, unsigned long);

extern int fail_migrate_page(struct address_space *,
			struct page *, struct page *);

extern int migrate_prep(void);
extern int migrate_vmas(struct mm_struct *mm,
		const nodemask_t *from, const nodemask_t *to,
		unsigned long flags);
#else
#define PAGE_MIGRATION 0

static inline int putback_lru_pages(struct list_head *l) { return 0; }
static inline int migrate_pages(struct list_head *l, new_page_t x,
		unsigned long private) { return -ENOSYS; }

static inline int migrate_pages_to(struct list_head *pagelist,
			struct vm_area_struct *vma, int dest) { return 0; }

static inline int migrate_prep(void) { return -ENOSYS; }

static inline int migrate_vmas(struct mm_struct *mm,
		const nodemask_t *from, const nodemask_t *to,
		unsigned long flags)
{
	return -ENOSYS;
}

/* Possible settings for the migrate_page() method in address_operations */
#define migrate_page NULL
#define fail_migrate_page NULL

#endif /* CONFIG_MIGRATION */
#endif /* _LINUX_MIGRATE_H */
