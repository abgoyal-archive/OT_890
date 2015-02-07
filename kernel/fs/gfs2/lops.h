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
 * Copyright (C) 2004-2008 Red Hat, Inc.  All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License version 2.
 */

#ifndef __LOPS_DOT_H__
#define __LOPS_DOT_H__

#include <linux/list.h>
#include "incore.h"

#define BUF_OFFSET \
	((sizeof(struct gfs2_log_descriptor) + sizeof(__be64) - 1) & \
	 ~(sizeof(__be64) - 1))
#define DATABUF_OFFSET \
	((sizeof(struct gfs2_log_descriptor) + (2 * sizeof(__be64) - 1)) & \
	 ~(2 * sizeof(__be64) - 1))

extern const struct gfs2_log_operations gfs2_glock_lops;
extern const struct gfs2_log_operations gfs2_buf_lops;
extern const struct gfs2_log_operations gfs2_revoke_lops;
extern const struct gfs2_log_operations gfs2_rg_lops;
extern const struct gfs2_log_operations gfs2_databuf_lops;

extern const struct gfs2_log_operations *gfs2_log_ops[];

static inline unsigned int buf_limit(struct gfs2_sbd *sdp)
{
	unsigned int limit;

	limit = (sdp->sd_sb.sb_bsize - BUF_OFFSET) / sizeof(__be64);
	return limit;
}

static inline unsigned int databuf_limit(struct gfs2_sbd *sdp)
{
	unsigned int limit;

	limit = (sdp->sd_sb.sb_bsize - DATABUF_OFFSET) / (2 * sizeof(__be64));
	return limit;
}

static inline void lops_init_le(struct gfs2_log_element *le,
				const struct gfs2_log_operations *lops)
{
	INIT_LIST_HEAD(&le->le_list);
	le->le_ops = lops;
}

static inline void lops_add(struct gfs2_sbd *sdp, struct gfs2_log_element *le)
{
	if (le->le_ops->lo_add)
		le->le_ops->lo_add(sdp, le);
}

static inline void lops_before_commit(struct gfs2_sbd *sdp)
{
	int x;
	for (x = 0; gfs2_log_ops[x]; x++)
		if (gfs2_log_ops[x]->lo_before_commit)
			gfs2_log_ops[x]->lo_before_commit(sdp);
}

static inline void lops_after_commit(struct gfs2_sbd *sdp, struct gfs2_ail *ai)
{
	int x;
	for (x = 0; gfs2_log_ops[x]; x++)
		if (gfs2_log_ops[x]->lo_after_commit)
			gfs2_log_ops[x]->lo_after_commit(sdp, ai);
}

static inline void lops_before_scan(struct gfs2_jdesc *jd,
				    struct gfs2_log_header_host *head,
				    unsigned int pass)
{
	int x;
	for (x = 0; gfs2_log_ops[x]; x++)
		if (gfs2_log_ops[x]->lo_before_scan)
			gfs2_log_ops[x]->lo_before_scan(jd, head, pass);
}

static inline int lops_scan_elements(struct gfs2_jdesc *jd, unsigned int start,
				     struct gfs2_log_descriptor *ld,
				     __be64 *ptr,
				     unsigned int pass)
{
	int x, error;
	for (x = 0; gfs2_log_ops[x]; x++)
		if (gfs2_log_ops[x]->lo_scan_elements) {
			error = gfs2_log_ops[x]->lo_scan_elements(jd, start,
								  ld, ptr, pass);
			if (error)
				return error;
		}

	return 0;
}

static inline void lops_after_scan(struct gfs2_jdesc *jd, int error,
				   unsigned int pass)
{
	int x;
	for (x = 0; gfs2_log_ops[x]; x++)
		if (gfs2_log_ops[x]->lo_before_scan)
			gfs2_log_ops[x]->lo_after_scan(jd, error, pass);
}

#endif /* __LOPS_DOT_H__ */

