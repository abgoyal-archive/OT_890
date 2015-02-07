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
 * Move OProfile dependencies from spufs module to the kernel so it
 * can run on non-cell PPC.
 *
 * Copyright (C) IBM 2005
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

#undef DEBUG

#include <linux/module.h>
#include <asm/spu.h>
#include "spufs/spufs.h"

static BLOCKING_NOTIFIER_HEAD(spu_switch_notifier);

void spu_switch_notify(struct spu *spu, struct spu_context *ctx)
{
	blocking_notifier_call_chain(&spu_switch_notifier,
				     ctx ? ctx->object_id : 0, spu);
}
EXPORT_SYMBOL_GPL(spu_switch_notify);

int spu_switch_event_register(struct notifier_block *n)
{
	int ret;
	ret = blocking_notifier_chain_register(&spu_switch_notifier, n);
	if (!ret)
		notify_spus_active();
	return ret;
}
EXPORT_SYMBOL_GPL(spu_switch_event_register);

int spu_switch_event_unregister(struct notifier_block *n)
{
	return blocking_notifier_chain_unregister(&spu_switch_notifier, n);
}
EXPORT_SYMBOL_GPL(spu_switch_event_unregister);

void spu_set_profile_private_kref(struct spu_context *ctx,
				  struct kref *prof_info_kref,
				  void (* prof_info_release) (struct kref *kref))
{
	ctx->prof_priv_kref = prof_info_kref;
	ctx->prof_priv_release = prof_info_release;
}
EXPORT_SYMBOL_GPL(spu_set_profile_private_kref);

void *spu_get_profile_private_kref(struct spu_context *ctx)
{
	return ctx->prof_priv_kref;
}
EXPORT_SYMBOL_GPL(spu_get_profile_private_kref);

