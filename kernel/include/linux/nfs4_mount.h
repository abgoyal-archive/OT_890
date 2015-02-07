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

#ifndef _LINUX_NFS4_MOUNT_H
#define _LINUX_NFS4_MOUNT_H

/*
 *  linux/include/linux/nfs4_mount.h
 *
 *  Copyright (C) 2002  Trond Myklebust
 *
 *  structure passed from user-space to kernel-space during an nfsv4 mount
 */

/*
 * WARNING!  Do not delete or change the order of these fields.  If
 * a new field is required then add it to the end.  The version field
 * tracks which fields are present.  This will ensure some measure of
 * mount-to-kernel version compatibility.  Some of these aren't used yet
 * but here they are anyway.
 */
#define NFS4_MOUNT_VERSION	1

struct nfs_string {
	unsigned int len;
	const char __user * data;
};

struct nfs4_mount_data {
	int version;				/* 1 */
	int flags;				/* 1 */
	int rsize;				/* 1 */
	int wsize;				/* 1 */
	int timeo;				/* 1 */
	int retrans;				/* 1 */
	int acregmin;				/* 1 */
	int acregmax;				/* 1 */
	int acdirmin;				/* 1 */
	int acdirmax;				/* 1 */

	/* see the definition of 'struct clientaddr4' in RFC3010 */
	struct nfs_string client_addr;		/* 1 */

	/* Mount path */
	struct nfs_string mnt_path;		/* 1 */

	/* Server details */
	struct nfs_string hostname;		/* 1 */
	/* Server IP address */
	unsigned int host_addrlen;		/* 1 */
	struct sockaddr __user * host_addr;	/* 1 */

	/* Transport protocol to use */
	int proto;				/* 1 */

	/* Pseudo-flavours to use for authentication. See RFC2623 */
	int auth_flavourlen;			/* 1 */
	int __user *auth_flavours;		/* 1 */
};

/* bits in the flags field */
/* Note: the fields that correspond to existing NFSv2/v3 mount options
 * 	 should mirror the values from include/linux/nfs_mount.h
 */

#define NFS4_MOUNT_SOFT		0x0001	/* 1 */
#define NFS4_MOUNT_INTR		0x0002	/* 1 */
#define NFS4_MOUNT_NOCTO	0x0010	/* 1 */
#define NFS4_MOUNT_NOAC		0x0020	/* 1 */
#define NFS4_MOUNT_STRICTLOCK	0x1000	/* 1 */
#define NFS4_MOUNT_UNSHARED	0x8000	/* 1 */
#define NFS4_MOUNT_FLAGMASK	0x9033

#endif
