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

#ifndef __QUOTA_DOT_H__
#define __QUOTA_DOT_H__

struct gfs2_inode;
struct gfs2_sbd;

#define NO_QUOTA_CHANGE ((u32)-1)

extern int gfs2_quota_hold(struct gfs2_inode *ip, u32 uid, u32 gid);
extern void gfs2_quota_unhold(struct gfs2_inode *ip);

extern int gfs2_quota_lock(struct gfs2_inode *ip, u32 uid, u32 gid);
extern void gfs2_quota_unlock(struct gfs2_inode *ip);

extern int gfs2_quota_check(struct gfs2_inode *ip, u32 uid, u32 gid);
extern void gfs2_quota_change(struct gfs2_inode *ip, s64 change,
			      u32 uid, u32 gid);

extern int gfs2_quota_sync(struct gfs2_sbd *sdp);
extern int gfs2_quota_refresh(struct gfs2_sbd *sdp, int user, u32 id);

extern int gfs2_quota_init(struct gfs2_sbd *sdp);
extern void gfs2_quota_cleanup(struct gfs2_sbd *sdp);
extern int gfs2_quotad(void *data);

static inline int gfs2_quota_lock_check(struct gfs2_inode *ip)
{
	struct gfs2_sbd *sdp = GFS2_SB(&ip->i_inode);
	int ret;
	if (sdp->sd_args.ar_quota == GFS2_QUOTA_OFF)
		return 0;
	ret = gfs2_quota_lock(ip, NO_QUOTA_CHANGE, NO_QUOTA_CHANGE);
	if (ret)
		return ret;
	if (sdp->sd_args.ar_quota != GFS2_QUOTA_ON)
		return 0;
	ret = gfs2_quota_check(ip, ip->i_inode.i_uid, ip->i_inode.i_gid);
	if (ret)
		gfs2_quota_unlock(ip);
	return ret;
}

#endif /* __QUOTA_DOT_H__ */
