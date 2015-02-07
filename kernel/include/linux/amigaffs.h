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

#ifndef AMIGAFFS_H
#define AMIGAFFS_H

#include <linux/types.h>
#include <asm/byteorder.h>

#define FS_OFS		0x444F5300
#define FS_FFS		0x444F5301
#define FS_INTLOFS	0x444F5302
#define FS_INTLFFS	0x444F5303
#define FS_DCOFS	0x444F5304
#define FS_DCFFS	0x444F5305
#define MUFS_FS		0x6d754653   /* 'muFS' */
#define MUFS_OFS	0x6d754600   /* 'muF\0' */
#define MUFS_FFS	0x6d754601   /* 'muF\1' */
#define MUFS_INTLOFS	0x6d754602   /* 'muF\2' */
#define MUFS_INTLFFS	0x6d754603   /* 'muF\3' */
#define MUFS_DCOFS	0x6d754604   /* 'muF\4' */
#define MUFS_DCFFS	0x6d754605   /* 'muF\5' */

#define T_SHORT		2
#define T_LIST		16
#define T_DATA		8

#define ST_LINKFILE	-4
#define ST_FILE		-3
#define ST_ROOT		1
#define ST_USERDIR	2
#define ST_SOFTLINK	3
#define ST_LINKDIR	4

#define AFFS_ROOT_BMAPS		25

struct affs_date {
	__be32 days;
	__be32 mins;
	__be32 ticks;
};

struct affs_short_date {
	__be16 days;
	__be16 mins;
	__be16 ticks;
};

struct affs_root_head {
	__be32 ptype;
	__be32 spare1;
	__be32 spare2;
	__be32 hash_size;
	__be32 spare3;
	__be32 checksum;
	__be32 hashtable[1];
};

struct affs_root_tail {
	__be32 bm_flag;
	__be32 bm_blk[AFFS_ROOT_BMAPS];
	__be32 bm_ext;
	struct affs_date root_change;
	u8 disk_name[32];
	__be32 spare1;
	__be32 spare2;
	struct affs_date disk_change;
	struct affs_date disk_create;
	__be32 spare3;
	__be32 spare4;
	__be32 dcache;
	__be32 stype;
};

struct affs_head {
	__be32 ptype;
	__be32 key;
	__be32 block_count;
	__be32 spare1;
	__be32 first_data;
	__be32 checksum;
	__be32 table[1];
};

struct affs_tail {
	__be32 spare1;
	__be16 uid;
	__be16 gid;
	__be32 protect;
	__be32 size;
	u8 comment[92];
	struct affs_date change;
	u8 name[32];
	__be32 spare2;
	__be32 original;
	__be32 link_chain;
	__be32 spare[5];
	__be32 hash_chain;
	__be32 parent;
	__be32 extension;
	__be32 stype;
};

struct slink_front
{
	__be32 ptype;
	__be32 key;
	__be32 spare1[3];
	__be32 checksum;
	u8 symname[1];	/* depends on block size */
};

struct affs_data_head
{
	__be32 ptype;
	__be32 key;
	__be32 sequence;
	__be32 size;
	__be32 next;
	__be32 checksum;
	u8 data[1];	/* depends on block size */
};

/* Permission bits */

#define FIBF_OTR_READ		0x8000
#define FIBF_OTR_WRITE		0x4000
#define FIBF_OTR_EXECUTE	0x2000
#define FIBF_OTR_DELETE		0x1000
#define FIBF_GRP_READ		0x0800
#define FIBF_GRP_WRITE		0x0400
#define FIBF_GRP_EXECUTE	0x0200
#define FIBF_GRP_DELETE		0x0100

#define FIBF_HIDDEN		0x0080
#define FIBF_SCRIPT		0x0040
#define FIBF_PURE		0x0020		/* no use under linux */
#define FIBF_ARCHIVED		0x0010		/* never set, always cleared on write */
#define FIBF_NOREAD		0x0008		/* 0 means allowed */
#define FIBF_NOWRITE		0x0004		/* 0 means allowed */
#define FIBF_NOEXECUTE		0x0002		/* 0 means allowed, ignored under linux */
#define FIBF_NODELETE		0x0001		/* 0 means allowed */

#define FIBF_OWNER		0x000F		/* Bits pertaining to owner */
#define FIBF_MASK		0xEE0E		/* Bits modified by Linux */

#endif
