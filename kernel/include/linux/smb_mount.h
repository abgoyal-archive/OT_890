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
 *  smb_mount.h
 *
 *  Copyright (C) 1995, 1996 by Paal-Kr. Engstad and Volker Lendecke
 *  Copyright (C) 1997 by Volker Lendecke
 *
 */

#ifndef _LINUX_SMB_MOUNT_H
#define _LINUX_SMB_MOUNT_H

#include <linux/types.h>

#define SMB_MOUNT_VERSION	6

struct smb_mount_data {
	int version;
	__kernel_uid_t mounted_uid; /* Who may umount() this filesystem? */
	__kernel_uid_t uid;
	__kernel_gid_t gid;
	__kernel_mode_t file_mode;
	__kernel_mode_t dir_mode;
};


#ifdef __KERNEL__

/* "vers" in big-endian */
#define SMB_MOUNT_ASCII 0x76657273

#define SMB_MOUNT_OLDVERSION	6
#undef SMB_MOUNT_VERSION
#define SMB_MOUNT_VERSION	7

/* flags */
#define SMB_MOUNT_WIN95		0x0001	/* Win 95 server */
#define SMB_MOUNT_OLDATTR	0x0002	/* Use core getattr (Win 95 speedup) */
#define SMB_MOUNT_DIRATTR	0x0004	/* Use find_first for getattr */
#define SMB_MOUNT_CASE		0x0008	/* Be case sensitive */
#define SMB_MOUNT_UNICODE	0x0010	/* Server talks unicode */
#define SMB_MOUNT_UID		0x0020  /* Use user specified uid */
#define SMB_MOUNT_GID		0x0040  /* Use user specified gid */
#define SMB_MOUNT_FMODE		0x0080  /* Use user specified file mode */
#define SMB_MOUNT_DMODE		0x0100  /* Use user specified dir mode */

struct smb_mount_data_kernel {
	int version;

	uid_t mounted_uid;	/* Who may umount() this filesystem? */
	uid_t uid;
	gid_t gid;
	mode_t file_mode;
	mode_t dir_mode;

	u32 flags;

        /* maximum age in jiffies (inode, dentry and dircache) */
	int ttl;

	struct smb_nls_codepage codepage;
};

#endif

#endif
