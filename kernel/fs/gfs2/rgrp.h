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

#ifndef __RGRP_DOT_H__
#define __RGRP_DOT_H__

struct gfs2_rgrpd;
struct gfs2_sbd;
struct gfs2_holder;

void gfs2_rgrp_verify(struct gfs2_rgrpd *rgd);

struct gfs2_rgrpd *gfs2_blk2rgrpd(struct gfs2_sbd *sdp, u64 blk);
struct gfs2_rgrpd *gfs2_rgrpd_get_first(struct gfs2_sbd *sdp);
struct gfs2_rgrpd *gfs2_rgrpd_get_next(struct gfs2_rgrpd *rgd);

void gfs2_clear_rgrpd(struct gfs2_sbd *sdp);
int gfs2_rindex_hold(struct gfs2_sbd *sdp, struct gfs2_holder *ri_gh);

int gfs2_rgrp_bh_get(struct gfs2_rgrpd *rgd);
void gfs2_rgrp_bh_hold(struct gfs2_rgrpd *rgd);
void gfs2_rgrp_bh_put(struct gfs2_rgrpd *rgd);

void gfs2_rgrp_repolish_clones(struct gfs2_rgrpd *rgd);

struct gfs2_alloc *gfs2_alloc_get(struct gfs2_inode *ip);
static inline void gfs2_alloc_put(struct gfs2_inode *ip)
{
	BUG_ON(ip->i_alloc == NULL);
	kfree(ip->i_alloc);
	ip->i_alloc = NULL;
}

int gfs2_inplace_reserve_i(struct gfs2_inode *ip,
			 char *file, unsigned int line);
#define gfs2_inplace_reserve(ip) \
gfs2_inplace_reserve_i((ip), __FILE__, __LINE__)

void gfs2_inplace_release(struct gfs2_inode *ip);

unsigned char gfs2_get_block_type(struct gfs2_rgrpd *rgd, u64 block);

u64 gfs2_alloc_block(struct gfs2_inode *ip, unsigned int *n);
u64 gfs2_alloc_di(struct gfs2_inode *ip, u64 *generation);

void gfs2_free_data(struct gfs2_inode *ip, u64 bstart, u32 blen);
void gfs2_free_meta(struct gfs2_inode *ip, u64 bstart, u32 blen);
void gfs2_free_di(struct gfs2_rgrpd *rgd, struct gfs2_inode *ip);
void gfs2_unlink_di(struct inode *inode);

struct gfs2_rgrp_list {
	unsigned int rl_rgrps;
	unsigned int rl_space;
	struct gfs2_rgrpd **rl_rgd;
	struct gfs2_holder *rl_ghs;
};

void gfs2_rlist_add(struct gfs2_sbd *sdp, struct gfs2_rgrp_list *rlist,
		    u64 block);
void gfs2_rlist_alloc(struct gfs2_rgrp_list *rlist, unsigned int state);
void gfs2_rlist_free(struct gfs2_rgrp_list *rlist);
u64 gfs2_ri_total(struct gfs2_sbd *sdp);

#endif /* __RGRP_DOT_H__ */
