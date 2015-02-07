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

/* -*- mode: c; c-basic-offset: 8; -*-
 * vim: noexpandtab sw=8 ts=8 sts=0:
 *
 * dlmdebug.h
 *
 * Copyright (C) 2008 Oracle.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 *
 */

#ifndef DLMDEBUG_H
#define DLMDEBUG_H

void dlm_print_one_mle(struct dlm_master_list_entry *mle);

#ifdef CONFIG_DEBUG_FS

struct dlm_debug_ctxt {
	struct kref debug_refcnt;
	struct dentry *debug_state_dentry;
	struct dentry *debug_lockres_dentry;
	struct dentry *debug_mle_dentry;
	struct dentry *debug_purgelist_dentry;
};

struct debug_buffer {
	int len;
	char *buf;
};

struct debug_lockres {
	int dl_len;
	char *dl_buf;
	struct dlm_ctxt *dl_ctxt;
	struct dlm_lock_resource *dl_res;
};

int dlm_debug_init(struct dlm_ctxt *dlm);
void dlm_debug_shutdown(struct dlm_ctxt *dlm);

int dlm_create_debugfs_subroot(struct dlm_ctxt *dlm);
void dlm_destroy_debugfs_subroot(struct dlm_ctxt *dlm);

int dlm_create_debugfs_root(void);
void dlm_destroy_debugfs_root(void);

#else

static inline int dlm_debug_init(struct dlm_ctxt *dlm)
{
	return 0;
}
static inline void dlm_debug_shutdown(struct dlm_ctxt *dlm)
{
}
static inline int dlm_create_debugfs_subroot(struct dlm_ctxt *dlm)
{
	return 0;
}
static inline void dlm_destroy_debugfs_subroot(struct dlm_ctxt *dlm)
{
}
static inline int dlm_create_debugfs_root(void)
{
	return 0;
}
static inline void dlm_destroy_debugfs_root(void)
{
}

#endif	/* CONFIG_DEBUG_FS */
#endif	/* DLMDEBUG_H */
