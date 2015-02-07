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

#include <linux/bootmem.h>
#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/mmzone.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include "internal.h"

#define KPMSIZE sizeof(u64)
#define KPMMASK (KPMSIZE - 1)
/* /proc/kpagecount - an array exposing page counts
 *
 * Each entry is a u64 representing the corresponding
 * physical page count.
 */
static ssize_t kpagecount_read(struct file *file, char __user *buf,
			     size_t count, loff_t *ppos)
{
	u64 __user *out = (u64 __user *)buf;
	struct page *ppage;
	unsigned long src = *ppos;
	unsigned long pfn;
	ssize_t ret = 0;
	u64 pcount;

	pfn = src / KPMSIZE;
	count = min_t(size_t, count, (max_pfn * KPMSIZE) - src);
	if (src & KPMMASK || count & KPMMASK)
		return -EINVAL;

	while (count > 0) {
		ppage = NULL;
		if (pfn_valid(pfn))
			ppage = pfn_to_page(pfn);
		pfn++;
		if (!ppage)
			pcount = 0;
		else
			pcount = page_mapcount(ppage);

		if (put_user(pcount, out++)) {
			ret = -EFAULT;
			break;
		}

		count -= KPMSIZE;
	}

	*ppos += (char __user *)out - buf;
	if (!ret)
		ret = (char __user *)out - buf;
	return ret;
}

static const struct file_operations proc_kpagecount_operations = {
	.llseek = mem_lseek,
	.read = kpagecount_read,
};

/* /proc/kpageflags - an array exposing page flags
 *
 * Each entry is a u64 representing the corresponding
 * physical page flags.
 */

/* These macros are used to decouple internal flags from exported ones */

#define KPF_LOCKED     0
#define KPF_ERROR      1
#define KPF_REFERENCED 2
#define KPF_UPTODATE   3
#define KPF_DIRTY      4
#define KPF_LRU        5
#define KPF_ACTIVE     6
#define KPF_SLAB       7
#define KPF_WRITEBACK  8
#define KPF_RECLAIM    9
#define KPF_BUDDY     10

#define kpf_copy_bit(flags, dstpos, srcpos) (((flags >> srcpos) & 1) << dstpos)

static ssize_t kpageflags_read(struct file *file, char __user *buf,
			     size_t count, loff_t *ppos)
{
	u64 __user *out = (u64 __user *)buf;
	struct page *ppage;
	unsigned long src = *ppos;
	unsigned long pfn;
	ssize_t ret = 0;
	u64 kflags, uflags;

	pfn = src / KPMSIZE;
	count = min_t(unsigned long, count, (max_pfn * KPMSIZE) - src);
	if (src & KPMMASK || count & KPMMASK)
		return -EINVAL;

	while (count > 0) {
		ppage = NULL;
		if (pfn_valid(pfn))
			ppage = pfn_to_page(pfn);
		pfn++;
		if (!ppage)
			kflags = 0;
		else
			kflags = ppage->flags;

		uflags = kpf_copy_bit(kflags, KPF_LOCKED, PG_locked) |
			kpf_copy_bit(kflags, KPF_ERROR, PG_error) |
			kpf_copy_bit(kflags, KPF_REFERENCED, PG_referenced) |
			kpf_copy_bit(kflags, KPF_UPTODATE, PG_uptodate) |
			kpf_copy_bit(kflags, KPF_DIRTY, PG_dirty) |
			kpf_copy_bit(kflags, KPF_LRU, PG_lru) |
			kpf_copy_bit(kflags, KPF_ACTIVE, PG_active) |
			kpf_copy_bit(kflags, KPF_SLAB, PG_slab) |
			kpf_copy_bit(kflags, KPF_WRITEBACK, PG_writeback) |
			kpf_copy_bit(kflags, KPF_RECLAIM, PG_reclaim) |
			kpf_copy_bit(kflags, KPF_BUDDY, PG_buddy);

		if (put_user(uflags, out++)) {
			ret = -EFAULT;
			break;
		}

		count -= KPMSIZE;
	}

	*ppos += (char __user *)out - buf;
	if (!ret)
		ret = (char __user *)out - buf;
	return ret;
}

static const struct file_operations proc_kpageflags_operations = {
	.llseek = mem_lseek,
	.read = kpageflags_read,
};

static int __init proc_page_init(void)
{
	proc_create("kpagecount", S_IRUSR, NULL, &proc_kpagecount_operations);
	proc_create("kpageflags", S_IRUSR, NULL, &proc_kpageflags_operations);
	return 0;
}
module_init(proc_page_init);
