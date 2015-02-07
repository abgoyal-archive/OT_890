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
 * Copyright (C) 2003 Sistina Software
 * Copyright (C) 2004 - 2008 Red Hat, Inc. All rights reserved.
 *
 * Device-Mapper low-level I/O.
 *
 * This file is released under the GPL.
 */

#ifndef _LINUX_DM_IO_H
#define _LINUX_DM_IO_H

#ifdef __KERNEL__

#include <linux/types.h>

struct dm_io_region {
	struct block_device *bdev;
	sector_t sector;
	sector_t count;		/* If this is zero the region is ignored. */
};

struct page_list {
	struct page_list *next;
	struct page *page;
};

typedef void (*io_notify_fn)(unsigned long error, void *context);

enum dm_io_mem_type {
	DM_IO_PAGE_LIST,/* Page list */
	DM_IO_BVEC,	/* Bio vector */
	DM_IO_VMA,	/* Virtual memory area */
	DM_IO_KMEM,	/* Kernel memory */
};

struct dm_io_memory {
	enum dm_io_mem_type type;

	union {
		struct page_list *pl;
		struct bio_vec *bvec;
		void *vma;
		void *addr;
	} ptr;

	unsigned offset;
};

struct dm_io_notify {
	io_notify_fn fn;	/* Callback for asynchronous requests */
	void *context;		/* Passed to callback */
};

/*
 * IO request structure
 */
struct dm_io_client;
struct dm_io_request {
	int bi_rw;			/* READ|WRITE - not READA */
	struct dm_io_memory mem;	/* Memory to use for io */
	struct dm_io_notify notify;	/* Synchronous if notify.fn is NULL */
	struct dm_io_client *client;	/* Client memory handler */
};

/*
 * For async io calls, users can alternatively use the dm_io() function below
 * and dm_io_client_create() to create private mempools for the client.
 *
 * Create/destroy may block.
 */
struct dm_io_client *dm_io_client_create(unsigned num_pages);
int dm_io_client_resize(unsigned num_pages, struct dm_io_client *client);
void dm_io_client_destroy(struct dm_io_client *client);

/*
 * IO interface using private per-client pools.
 * Each bit in the optional 'sync_error_bits' bitset indicates whether an
 * error occurred doing io to the corresponding region.
 */
int dm_io(struct dm_io_request *io_req, unsigned num_regions,
	  struct dm_io_region *region, unsigned long *sync_error_bits);

#endif	/* __KERNEL__ */
#endif	/* _LINUX_DM_IO_H */
