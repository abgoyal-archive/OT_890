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

#ifndef __DIR_DOT_H__
#define __DIR_DOT_H__

#include <linux/dcache.h>
#include <linux/crc32.h>

struct inode;
struct gfs2_inode;
struct gfs2_inum;

struct inode *gfs2_dir_search(struct inode *dir, const struct qstr *filename);
int gfs2_dir_check(struct inode *dir, const struct qstr *filename,
		   const struct gfs2_inode *ip);
int gfs2_dir_add(struct inode *inode, const struct qstr *filename,
		 const struct gfs2_inode *ip, unsigned int type);
int gfs2_dir_del(struct gfs2_inode *dip, const struct qstr *filename);
int gfs2_dir_read(struct inode *inode, u64 *offset, void *opaque,
		  filldir_t filldir);
int gfs2_dir_mvino(struct gfs2_inode *dip, const struct qstr *filename,
		   const struct gfs2_inode *nip, unsigned int new_type);

int gfs2_dir_exhash_dealloc(struct gfs2_inode *dip);

int gfs2_diradd_alloc_required(struct inode *dir,
			       const struct qstr *filename);
int gfs2_dir_get_new_buffer(struct gfs2_inode *ip, u64 block,
			    struct buffer_head **bhp);

static inline u32 gfs2_disk_hash(const char *data, int len)
{
        return crc32_le((u32)~0, data, len) ^ (u32)~0;
}


static inline void gfs2_str2qstr(struct qstr *name, const char *fname)
{
	name->name = fname;
	name->len = strlen(fname);
	name->hash = gfs2_disk_hash(name->name, name->len);
}

/* N.B. This probably ought to take inum & type as args as well */
static inline void gfs2_qstr2dirent(const struct qstr *name, u16 reclen, struct gfs2_dirent *dent)
{
	dent->de_inum.no_addr = cpu_to_be64(0);
	dent->de_inum.no_formal_ino = cpu_to_be64(0);
	dent->de_hash = cpu_to_be32(name->hash);
	dent->de_rec_len = cpu_to_be16(reclen);
	dent->de_name_len = cpu_to_be16(name->len);
	dent->de_type = cpu_to_be16(0);
	memset(dent->__pad, 0, sizeof(dent->__pad));
	memcpy(dent + 1, name->name, name->len);
}

#endif /* __DIR_DOT_H__ */
