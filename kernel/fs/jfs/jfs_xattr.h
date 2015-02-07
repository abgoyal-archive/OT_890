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
 *   Copyright (C) International Business Machines Corp., 2000-2002
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef H_JFS_XATTR
#define H_JFS_XATTR

/*
 * jfs_ea_list describe the on-disk format of the extended attributes.
 * I know the null-terminator is redundant since namelen is stored, but
 * I am maintaining compatibility with OS/2 where possible.
 */
struct jfs_ea {
	u8 flag;	/* Unused? */
	u8 namelen;	/* Length of name */
	__le16 valuelen;	/* Length of value */
	char name[0];	/* Attribute name (includes null-terminator) */
};			/* Value immediately follows name */

struct jfs_ea_list {
	__le32 size;		/* overall size */
	struct jfs_ea ea[0];	/* Variable length list */
};

/* Macros for defining maxiumum number of bytes supported for EAs */
#define MAXEASIZE	65535
#define MAXEALISTSIZE	MAXEASIZE

/*
 * some macros for dealing with variable length EA lists.
 */
#define EA_SIZE(ea) \
	(sizeof (struct jfs_ea) + (ea)->namelen + 1 + \
	 le16_to_cpu((ea)->valuelen))
#define	NEXT_EA(ea) ((struct jfs_ea *) (((char *) (ea)) + (EA_SIZE (ea))))
#define	FIRST_EA(ealist) ((ealist)->ea)
#define	EALIST_SIZE(ealist) le32_to_cpu((ealist)->size)
#define	END_EALIST(ealist) \
	((struct jfs_ea *) (((char *) (ealist)) + EALIST_SIZE(ealist)))

extern int __jfs_setxattr(tid_t, struct inode *, const char *, const void *,
			  size_t, int);
extern int jfs_setxattr(struct dentry *, const char *, const void *, size_t,
			int);
extern ssize_t __jfs_getxattr(struct inode *, const char *, void *, size_t);
extern ssize_t jfs_getxattr(struct dentry *, const char *, void *, size_t);
extern ssize_t jfs_listxattr(struct dentry *, char *, size_t);
extern int jfs_removexattr(struct dentry *, const char *);

#ifdef CONFIG_JFS_SECURITY
extern int jfs_init_security(tid_t, struct inode *, struct inode *);
#else
static inline int jfs_init_security(tid_t tid, struct inode *inode,
				    struct inode *dir)
{
	return 0;
}
#endif

#endif	/* H_JFS_XATTR */
