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

#ifndef _ADFS_FS_H
#define _ADFS_FS_H

#include <linux/types.h>
#include <linux/magic.h>

/*
 * Disc Record at disc address 0xc00
 */
struct adfs_discrecord {
    __u8  log2secsize;
    __u8  secspertrack;
    __u8  heads;
    __u8  density;
    __u8  idlen;
    __u8  log2bpmb;
    __u8  skew;
    __u8  bootoption;
    __u8  lowsector;
    __u8  nzones;
    __le16 zone_spare;
    __le32 root;
    __le32 disc_size;
    __le16 disc_id;
    __u8  disc_name[10];
    __le32 disc_type;
    __le32 disc_size_high;
    __u8  log2sharesize:4;
    __u8  unused40:4;
    __u8  big_flag:1;
    __u8  unused41:1;
    __u8  nzones_high;
    __le32 format_version;
    __le32 root_size;
    __u8  unused52[60 - 52];
};

#define ADFS_DISCRECORD		(0xc00)
#define ADFS_DR_OFFSET		(0x1c0)
#define ADFS_DR_SIZE		 60
#define ADFS_DR_SIZE_BITS	(ADFS_DR_SIZE << 3)

#ifdef __KERNEL__
#include <linux/adfs_fs_i.h>
#include <linux/adfs_fs_sb.h>
/*
 * Calculate the boot block checksum on an ADFS drive.  Note that this will
 * appear to be correct if the sector contains all zeros, so also check that
 * the disk size is non-zero!!!
 */
static inline int adfs_checkbblk(unsigned char *ptr)
{
	unsigned int result = 0;
	unsigned char *p = ptr + 511;

	do {
	        result = (result & 0xff) + (result >> 8);
        	result = result + *--p;
	} while (p != ptr);

	return (result & 0xff) != ptr[511];
}

static inline struct adfs_sb_info *ADFS_SB(struct super_block *sb)
{
	return sb->s_fs_info;
}

static inline struct adfs_inode_info *ADFS_I(struct inode *inode)
{
	return container_of(inode, struct adfs_inode_info, vfs_inode);
}

#endif

#endif
