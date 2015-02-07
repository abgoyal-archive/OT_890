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
 * Cryptographic API.
 *
 * Cipher operations.
 *
 * Copyright (c) 2002 James Morris <jmorris@intercode.com.au>
 *               2002 Adam J. Richter <adam@yggdrasil.com>
 *               2004 Jean-Luc Cooke <jlcooke@certainkey.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include <crypto/scatterwalk.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/pagemap.h>
#include <linux/highmem.h>
#include <linux/scatterlist.h>

static inline void memcpy_dir(void *buf, void *sgdata, size_t nbytes, int out)
{
	void *src = out ? buf : sgdata;
	void *dst = out ? sgdata : buf;

	memcpy(dst, src, nbytes);
}

void scatterwalk_start(struct scatter_walk *walk, struct scatterlist *sg)
{
	walk->sg = sg;

	BUG_ON(!sg->length);

	walk->offset = sg->offset;
}
EXPORT_SYMBOL_GPL(scatterwalk_start);

void *scatterwalk_map(struct scatter_walk *walk, int out)
{
	return crypto_kmap(scatterwalk_page(walk), out) +
	       offset_in_page(walk->offset);
}
EXPORT_SYMBOL_GPL(scatterwalk_map);

static void scatterwalk_pagedone(struct scatter_walk *walk, int out,
				 unsigned int more)
{
	if (out) {
		struct page *page;

		page = sg_page(walk->sg) + ((walk->offset - 1) >> PAGE_SHIFT);
		if (!PageSlab(page))
			flush_dcache_page(page);
	}

	if (more) {
		walk->offset += PAGE_SIZE - 1;
		walk->offset &= PAGE_MASK;
		if (walk->offset >= walk->sg->offset + walk->sg->length)
			scatterwalk_start(walk, scatterwalk_sg_next(walk->sg));
	}
}

void scatterwalk_done(struct scatter_walk *walk, int out, int more)
{
	if (!offset_in_page(walk->offset) || !more)
		scatterwalk_pagedone(walk, out, more);
}
EXPORT_SYMBOL_GPL(scatterwalk_done);

void scatterwalk_copychunks(void *buf, struct scatter_walk *walk,
			    size_t nbytes, int out)
{
	for (;;) {
		unsigned int len_this_page = scatterwalk_pagelen(walk);
		u8 *vaddr;

		if (len_this_page > nbytes)
			len_this_page = nbytes;

		vaddr = scatterwalk_map(walk, out);
		memcpy_dir(buf, vaddr, len_this_page, out);
		scatterwalk_unmap(vaddr, out);

		scatterwalk_advance(walk, len_this_page);

		if (nbytes == len_this_page)
			break;

		buf += len_this_page;
		nbytes -= len_this_page;

		scatterwalk_pagedone(walk, out, 1);
	}
}
EXPORT_SYMBOL_GPL(scatterwalk_copychunks);

void scatterwalk_map_and_copy(void *buf, struct scatterlist *sg,
			      unsigned int start, unsigned int nbytes, int out)
{
	struct scatter_walk walk;
	unsigned int offset = 0;

	if (!nbytes)
		return;

	for (;;) {
		scatterwalk_start(&walk, sg);

		if (start < offset + sg->length)
			break;

		offset += sg->length;
		sg = scatterwalk_sg_next(sg);
	}

	scatterwalk_advance(&walk, start - offset);
	scatterwalk_copychunks(buf, &walk, nbytes, out);
	scatterwalk_done(&walk, out, 0);
}
EXPORT_SYMBOL_GPL(scatterwalk_map_and_copy);
