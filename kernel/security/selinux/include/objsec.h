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
 *  NSA Security-Enhanced Linux (SELinux) security module
 *
 *  This file contains the SELinux security data structures for kernel objects.
 *
 *  Author(s):  Stephen Smalley, <sds@epoch.ncsc.mil>
 *		Chris Vance, <cvance@nai.com>
 *		Wayne Salamon, <wsalamon@nai.com>
 *		James Morris <jmorris@redhat.com>
 *
 *  Copyright (C) 2001,2002 Networks Associates Technology, Inc.
 *  Copyright (C) 2003 Red Hat, Inc., James Morris <jmorris@redhat.com>
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2,
 *	as published by the Free Software Foundation.
 */
#ifndef _SELINUX_OBJSEC_H_
#define _SELINUX_OBJSEC_H_

#include <linux/list.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/binfmts.h>
#include <linux/in.h>
#include <linux/spinlock.h>
#include "flask.h"
#include "avc.h"

struct task_security_struct {
	u32 osid;		/* SID prior to last execve */
	u32 sid;		/* current SID */
	u32 exec_sid;		/* exec SID */
	u32 create_sid;		/* fscreate SID */
	u32 keycreate_sid;	/* keycreate SID */
	u32 sockcreate_sid;	/* fscreate SID */
};

struct inode_security_struct {
	struct inode *inode;	/* back pointer to inode object */
	struct list_head list;	/* list of inode_security_struct */
	u32 task_sid;		/* SID of creating task */
	u32 sid;		/* SID of this object */
	u16 sclass;		/* security class of this object */
	unsigned char initialized;	/* initialization flag */
	struct mutex lock;
};

struct file_security_struct {
	u32 sid;		/* SID of open file description */
	u32 fown_sid;		/* SID of file owner (for SIGIO) */
	u32 isid;		/* SID of inode at the time of file open */
	u32 pseqno;		/* Policy seqno at the time of file open */
};

struct superblock_security_struct {
	struct super_block *sb;		/* back pointer to sb object */
	struct list_head list;		/* list of superblock_security_struct */
	u32 sid;			/* SID of file system superblock */
	u32 def_sid;			/* default SID for labeling */
	u32 mntpoint_sid;		/* SECURITY_FS_USE_MNTPOINT context for files */
	unsigned int behavior;		/* labeling behavior */
	unsigned char initialized;	/* initialization flag */
	unsigned char flags;		/* which mount options were specified */
	unsigned char proc;		/* proc fs */
	struct mutex lock;
	struct list_head isec_head;
	spinlock_t isec_lock;
};

struct msg_security_struct {
	u32 sid;	/* SID of message */
};

struct ipc_security_struct {
	u16 sclass;	/* security class of this object */
	u32 sid;	/* SID of IPC resource */
};

struct netif_security_struct {
	int ifindex;			/* device index */
	u32 sid;			/* SID for this interface */
};

struct netnode_security_struct {
	union {
		__be32 ipv4;		/* IPv4 node address */
		struct in6_addr ipv6;	/* IPv6 node address */
	} addr;
	u32 sid;			/* SID for this node */
	u16 family;			/* address family */
};

struct netport_security_struct {
	u32 sid;			/* SID for this node */
	u16 port;			/* port number */
	u8 protocol;			/* transport protocol */
};

struct sk_security_struct {
#ifdef CONFIG_NETLABEL
	enum {				/* NetLabel state */
		NLBL_UNSET = 0,
		NLBL_REQUIRE,
		NLBL_LABELED,
		NLBL_REQSKB,
		NLBL_CONNLABELED,
	} nlbl_state;
	struct netlbl_lsm_secattr *nlbl_secattr; /* NetLabel sec attributes */
#endif
	u32 sid;			/* SID of this object */
	u32 peer_sid;			/* SID of peer */
	u16 sclass;			/* sock security class */
};

struct key_security_struct {
	u32 sid;	/* SID of key */
};

extern unsigned int selinux_checkreqprot;

#endif /* _SELINUX_OBJSEC_H_ */
