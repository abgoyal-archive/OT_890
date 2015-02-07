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

/******************************************************************************
*******************************************************************************
**
**  Copyright (C) Sistina Software, Inc.  1997-2003  All rights reserved.
**  Copyright (C) 2004-2007 Red Hat, Inc.  All rights reserved.
**
**  This copyrighted material is made available to anyone wishing to use,
**  modify, copy, or redistribute it subject to the terms and conditions
**  of the GNU General Public License v.2.
**
*******************************************************************************
******************************************************************************/

#ifndef _LINUX_DLM_DEVICE_H
#define _LINUX_DLM_DEVICE_H

/* This is the device interface for dlm, most users will use a library
 * interface.
 */

#include <linux/dlm.h>
#include <linux/types.h>

#define DLM_USER_LVB_LEN	32

/* Version of the device interface */
#define DLM_DEVICE_VERSION_MAJOR 6
#define DLM_DEVICE_VERSION_MINOR 0
#define DLM_DEVICE_VERSION_PATCH 1

/* struct passed to the lock write */
struct dlm_lock_params {
	__u8 mode;
	__u8 namelen;
	__u16 unused;
	__u32 flags;
	__u32 lkid;
	__u32 parent;
	__u64 xid;
	__u64 timeout;
	void __user *castparam;
	void __user *castaddr;
	void __user *bastparam;
	void __user *bastaddr;
	struct dlm_lksb __user *lksb;
	char lvb[DLM_USER_LVB_LEN];
	char name[0];
};

struct dlm_lspace_params {
	__u32 flags;
	__u32 minor;
	char name[0];
};

struct dlm_purge_params {
	__u32 nodeid;
	__u32 pid;
};

struct dlm_write_request {
	__u32 version[3];
	__u8 cmd;
	__u8 is64bit;
	__u8 unused[2];

	union  {
		struct dlm_lock_params   lock;
		struct dlm_lspace_params lspace;
		struct dlm_purge_params  purge;
	} i;
};

struct dlm_device_version {
	__u32 version[3];
};

/* struct read from the "device" fd,
   consists mainly of userspace pointers for the library to use */

struct dlm_lock_result {
	__u32 version[3];
	__u32 length;
	void __user * user_astaddr;
	void __user * user_astparam;
	struct dlm_lksb __user * user_lksb;
	struct dlm_lksb lksb;
	__u8 bast_mode;
	__u8 unused[3];
	/* Offsets may be zero if no data is present */
	__u32 lvb_offset;
};

/* Commands passed to the device */
#define DLM_USER_LOCK         1
#define DLM_USER_UNLOCK       2
#define DLM_USER_QUERY        3
#define DLM_USER_CREATE_LOCKSPACE  4
#define DLM_USER_REMOVE_LOCKSPACE  5
#define DLM_USER_PURGE        6
#define DLM_USER_DEADLOCK     7

/* Lockspace flags */
#define DLM_USER_LSFLG_AUTOFREE   1
#define DLM_USER_LSFLG_FORCEFREE  2

#endif

