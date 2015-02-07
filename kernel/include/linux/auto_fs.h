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

/* -*- linux-c -*- ------------------------------------------------------- *
 *   
 * linux/include/linux/auto_fs.h
 *
 *   Copyright 1997 Transmeta Corporation - All Rights Reserved
 *
 * This file is part of the Linux kernel and is made available under
 * the terms of the GNU General Public License, version 2, or at your
 * option, any later version, incorporated herein by reference.
 *
 * ----------------------------------------------------------------------- */


#ifndef _LINUX_AUTO_FS_H
#define _LINUX_AUTO_FS_H

#ifdef __KERNEL__
#include <linux/fs.h>
#include <linux/limits.h>
#include <asm/types.h>
#endif /* __KERNEL__ */

#include <linux/ioctl.h>

/* This file describes autofs v3 */
#define AUTOFS_PROTO_VERSION	3

/* Range of protocol versions defined */
#define AUTOFS_MAX_PROTO_VERSION	AUTOFS_PROTO_VERSION
#define AUTOFS_MIN_PROTO_VERSION	AUTOFS_PROTO_VERSION

/*
 * Architectures where both 32- and 64-bit binaries can be executed
 * on 64-bit kernels need this.  This keeps the structure format
 * uniform, and makes sure the wait_queue_token isn't too big to be
 * passed back down to the kernel.
 *
 * This assumes that on these architectures:
 * mode     32 bit    64 bit
 * -------------------------
 * int      32 bit    32 bit
 * long     32 bit    64 bit
 *
 * If so, 32-bit user-space code should be backwards compatible.
 */

#if defined(__sparc__) || defined(__mips__) || defined(__x86_64__) \
 || defined(__powerpc__) || defined(__s390__)
typedef unsigned int autofs_wqt_t;
#else
typedef unsigned long autofs_wqt_t;
#endif

/* Packet types */
#define autofs_ptype_missing	0	/* Missing entry (mount request) */
#define autofs_ptype_expire	1	/* Expire entry (umount request) */

struct autofs_packet_hdr {
	int proto_version;		/* Protocol version */
	int type;			/* Type of packet */
};

struct autofs_packet_missing {
	struct autofs_packet_hdr hdr;
        autofs_wqt_t wait_queue_token;
	int len;
	char name[NAME_MAX+1];
};	

/* v3 expire (via ioctl) */
struct autofs_packet_expire {
	struct autofs_packet_hdr hdr;
	int len;
	char name[NAME_MAX+1];
};

#define AUTOFS_IOC_READY      _IO(0x93,0x60)
#define AUTOFS_IOC_FAIL       _IO(0x93,0x61)
#define AUTOFS_IOC_CATATONIC  _IO(0x93,0x62)
#define AUTOFS_IOC_PROTOVER   _IOR(0x93,0x63,int)
#define AUTOFS_IOC_SETTIMEOUT _IOWR(0x93,0x64,unsigned long)
#define AUTOFS_IOC_EXPIRE     _IOR(0x93,0x65,struct autofs_packet_expire)

#endif /* _LINUX_AUTO_FS_H */
