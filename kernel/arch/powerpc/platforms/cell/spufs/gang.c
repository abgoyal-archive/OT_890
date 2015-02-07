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
 * SPU file system
 *
 * (C) Copyright IBM Deutschland Entwicklung GmbH 2005
 *
 * Author: Arnd Bergmann <arndb@de.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/list.h>
#include <linux/slab.h>

#include "spufs.h"

struct spu_gang *alloc_spu_gang(void)
{
	struct spu_gang *gang;

	gang = kzalloc(sizeof *gang, GFP_KERNEL);
	if (!gang)
		goto out;

	kref_init(&gang->kref);
	mutex_init(&gang->mutex);
	mutex_init(&gang->aff_mutex);
	INIT_LIST_HEAD(&gang->list);
	INIT_LIST_HEAD(&gang->aff_list_head);

out:
	return gang;
}

static void destroy_spu_gang(struct kref *kref)
{
	struct spu_gang *gang;
	gang = container_of(kref, struct spu_gang, kref);
	WARN_ON(gang->contexts || !list_empty(&gang->list));
	kfree(gang);
}

struct spu_gang *get_spu_gang(struct spu_gang *gang)
{
	kref_get(&gang->kref);
	return gang;
}

int put_spu_gang(struct spu_gang *gang)
{
	return kref_put(&gang->kref, &destroy_spu_gang);
}

void spu_gang_add_ctx(struct spu_gang *gang, struct spu_context *ctx)
{
	mutex_lock(&gang->mutex);
	ctx->gang = get_spu_gang(gang);
	list_add(&ctx->gang_list, &gang->list);
	gang->contexts++;
	mutex_unlock(&gang->mutex);
}

void spu_gang_remove_ctx(struct spu_gang *gang, struct spu_context *ctx)
{
	mutex_lock(&gang->mutex);
	WARN_ON(ctx->gang != gang);
	if (!list_empty(&ctx->aff_list)) {
		list_del_init(&ctx->aff_list);
		gang->aff_flags &= ~AFF_OFFSETS_SET;
	}
	list_del_init(&ctx->gang_list);
	gang->contexts--;
	mutex_unlock(&gang->mutex);

	put_spu_gang(gang);
}
