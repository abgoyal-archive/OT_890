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

#ifndef _LINUX_NFS_MOUNT_H
#define _LINUX_NFS_MOUNT_H

/*
 *  linux/include/linux/nfs_mount.h
 *
 *  Copyright (C) 1992  Rick Sladkey
 *
 *  structure passed from user-space to kernel-space during an nfs mount
 */
#include <linux/in.h>
#include <linux/nfs.h>
#include <linux/nfs2.h>
#include <linux/nfs3.h>

/*
 * WARNING!  Do not delete or change the order of these fields.  If
 * a new field is required then add it to the end.  The version field
 * tracks which fields are present.  This will ensure some measure of
 * mount-to-kernel version compatibility.  Some of these aren't used yet
 * but here they are anyway.
 */
#define NFS_MOUNT_VERSION	6
#define NFS_MAX_CONTEXT_LEN	256

struct nfs_mount_data {
	int		version;		/* 1 */
	int		fd;			/* 1 */
	struct nfs2_fh	old_root;		/* 1 */
	int		flags;			/* 1 */
	int		rsize;			/* 1 */
	int		wsize;			/* 1 */
	int		timeo;			/* 1 */
	int		retrans;		/* 1 */
	int		acregmin;		/* 1 */
	int		acregmax;		/* 1 */
	int		acdirmin;		/* 1 */
	int		acdirmax;		/* 1 */
	struct sockaddr_in addr;		/* 1 */
	char		hostname[NFS_MAXNAMLEN + 1];		/* 1 */
	int		namlen;			/* 2 */
	unsigned int	bsize;			/* 3 */
	struct nfs3_fh	root;			/* 4 */
	int		pseudoflavor;		/* 5 */
	char		context[NFS_MAX_CONTEXT_LEN + 1];	/* 6 */
};

/* bits in the flags field visible to user space */

#define NFS_MOUNT_SOFT		0x0001	/* 1 */
#define NFS_MOUNT_INTR		0x0002	/* 1 */ /* now unused, but ABI */
#define NFS_MOUNT_SECURE	0x0004	/* 1 */
#define NFS_MOUNT_POSIX		0x0008	/* 1 */
#define NFS_MOUNT_NOCTO		0x0010	/* 1 */
#define NFS_MOUNT_NOAC		0x0020	/* 1 */
#define NFS_MOUNT_TCP		0x0040	/* 2 */
#define NFS_MOUNT_VER3		0x0080	/* 3 */
#define NFS_MOUNT_KERBEROS	0x0100	/* 3 */
#define NFS_MOUNT_NONLM		0x0200	/* 3 */
#define NFS_MOUNT_BROKEN_SUID	0x0400	/* 4 */
#define NFS_MOUNT_NOACL		0x0800	/* 4 */
#define NFS_MOUNT_STRICTLOCK	0x1000	/* reserved for NFSv4 */
#define NFS_MOUNT_SECFLAVOUR	0x2000	/* 5 */
#define NFS_MOUNT_NORDIRPLUS	0x4000	/* 5 */
#define NFS_MOUNT_UNSHARED	0x8000	/* 5 */
#define NFS_MOUNT_FLAGMASK	0xFFFF

/* The following are for internal use only */
#define NFS_MOUNT_LOOKUP_CACHE_NONEG	0x10000
#define NFS_MOUNT_LOOKUP_CACHE_NONE	0x20000
#define NFS_MOUNT_NORESVPORT		0x40000

#endif
