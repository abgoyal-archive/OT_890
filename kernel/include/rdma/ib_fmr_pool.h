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
 * Copyright (c) 2004 Topspin Corporation.  All rights reserved.
 * Copyright (c) 2005 Sun Microsystems, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#if !defined(IB_FMR_POOL_H)
#define IB_FMR_POOL_H

#include <rdma/ib_verbs.h>

struct ib_fmr_pool;

/**
 * struct ib_fmr_pool_param - Parameters for creating FMR pool
 * @max_pages_per_fmr:Maximum number of pages per map request.
 * @page_shift: Log2 of sizeof "pages" mapped by this fmr
 * @access:Access flags for FMRs in pool.
 * @pool_size:Number of FMRs to allocate for pool.
 * @dirty_watermark:Flush is triggered when @dirty_watermark dirty
 *     FMRs are present.
 * @flush_function:Callback called when unmapped FMRs are flushed and
 *     more FMRs are possibly available for mapping
 * @flush_arg:Context passed to user's flush function.
 * @cache:If set, FMRs may be reused after unmapping for identical map
 *     requests.
 */
struct ib_fmr_pool_param {
	int                     max_pages_per_fmr;
	int                     page_shift;
	enum ib_access_flags    access;
	int                     pool_size;
	int                     dirty_watermark;
	void                  (*flush_function)(struct ib_fmr_pool *pool,
						void               *arg);
	void                   *flush_arg;
	unsigned                cache:1;
};

struct ib_pool_fmr {
	struct ib_fmr      *fmr;
	struct ib_fmr_pool *pool;
	struct list_head    list;
	struct hlist_node   cache_node;
	int                 ref_count;
	int                 remap_count;
	u64                 io_virtual_address;
	int                 page_list_len;
	u64                 page_list[0];
};

struct ib_fmr_pool *ib_create_fmr_pool(struct ib_pd             *pd,
				       struct ib_fmr_pool_param *params);

void ib_destroy_fmr_pool(struct ib_fmr_pool *pool);

int ib_flush_fmr_pool(struct ib_fmr_pool *pool);

struct ib_pool_fmr *ib_fmr_pool_map_phys(struct ib_fmr_pool *pool_handle,
					 u64                *page_list,
					 int                 list_len,
					 u64                 io_virtual_address);

int ib_fmr_pool_unmap(struct ib_pool_fmr *fmr);

#endif /* IB_FMR_POOL_H */
