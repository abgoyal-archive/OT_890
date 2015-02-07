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
 * Generic show_mem() implementation
 *
 * Copyright (C) 2008 Johannes Weiner <hannes@saeurebad.de>
 * All code subject to the GPL version 2.
 */

#include <linux/mm.h>
#include <linux/nmi.h>
#include <linux/quicklist.h>

void show_mem(void)
{
	pg_data_t *pgdat;
	unsigned long total = 0, reserved = 0, shared = 0,
		nonshared = 0, highmem = 0;

	printk(KERN_INFO "Mem-Info:\n");
	show_free_areas();

	for_each_online_pgdat(pgdat) {
		unsigned long i, flags;

		pgdat_resize_lock(pgdat, &flags);
		for (i = 0; i < pgdat->node_spanned_pages; i++) {
			struct page *page;
			unsigned long pfn = pgdat->node_start_pfn + i;

			if (unlikely(!(i % MAX_ORDER_NR_PAGES)))
				touch_nmi_watchdog();

			if (!pfn_valid(pfn))
				continue;

			page = pfn_to_page(pfn);

			if (PageHighMem(page))
				highmem++;

			if (PageReserved(page))
				reserved++;
			else if (page_count(page) == 1)
				nonshared++;
			else if (page_count(page) > 1)
				shared += page_count(page) - 1;

			total++;
		}
		pgdat_resize_unlock(pgdat, &flags);
	}

	printk(KERN_INFO "%lu pages RAM\n", total);
#ifdef CONFIG_HIGHMEM
	printk(KERN_INFO "%lu pages HighMem\n", highmem);
#endif
	printk(KERN_INFO "%lu pages reserved\n", reserved);
	printk(KERN_INFO "%lu pages shared\n", shared);
	printk(KERN_INFO "%lu pages non-shared\n", nonshared);
#ifdef CONFIG_QUICKLIST
	printk(KERN_INFO "%lu pages in pagetable cache\n",
		quicklist_total_size());
#endif
}
