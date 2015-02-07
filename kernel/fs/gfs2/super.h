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
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2006 Red Hat, Inc.  All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License version 2.
 */

#ifndef __SUPER_DOT_H__
#define __SUPER_DOT_H__

#include <linux/fs.h>
#include <linux/dcache.h>
#include "incore.h"

void gfs2_lm_unmount(struct gfs2_sbd *sdp);

static inline unsigned int gfs2_jindex_size(struct gfs2_sbd *sdp)
{
	unsigned int x;
	spin_lock(&sdp->sd_jindex_spin);
	x = sdp->sd_journals;
	spin_unlock(&sdp->sd_jindex_spin);
	return x;
}

void gfs2_jindex_free(struct gfs2_sbd *sdp);

struct gfs2_jdesc *gfs2_jdesc_find(struct gfs2_sbd *sdp, unsigned int jid);
int gfs2_jdesc_check(struct gfs2_jdesc *jd);

int gfs2_lookup_in_master_dir(struct gfs2_sbd *sdp, char *filename,
			      struct gfs2_inode **ipp);

int gfs2_make_fs_rw(struct gfs2_sbd *sdp);

int gfs2_statfs_init(struct gfs2_sbd *sdp);
void gfs2_statfs_change(struct gfs2_sbd *sdp,
			s64 total, s64 free, s64 dinodes);
int gfs2_statfs_sync(struct gfs2_sbd *sdp);

int gfs2_freeze_fs(struct gfs2_sbd *sdp);
void gfs2_unfreeze_fs(struct gfs2_sbd *sdp);

extern struct file_system_type gfs2_fs_type;
extern struct file_system_type gfs2meta_fs_type;
extern const struct export_operations gfs2_export_ops;
extern const struct super_operations gfs2_super_ops;
extern struct dentry_operations gfs2_dops;

#endif /* __SUPER_DOT_H__ */

