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
 * include/linux/nfsd/const.h
 *
 * Various constants related to NFS.
 *
 * Copyright (C) 1995-1997 Olaf Kirch <okir@monad.swb.de>
 */

#ifndef _LINUX_NFSD_CONST_H
#define _LINUX_NFSD_CONST_H

#include <linux/nfs.h>
#include <linux/nfs2.h>
#include <linux/nfs3.h>
#include <linux/nfs4.h>

/*
 * Maximum protocol version supported by knfsd
 */
#define NFSSVC_MAXVERS		3

/*
 * Maximum blocksizes supported by daemon under various circumstances.
 */
#define NFSSVC_MAXBLKSIZE	RPCSVC_MAXPAYLOAD
/* NFSv2 is limited by the protocol specification, see RFC 1094 */
#define NFSSVC_MAXBLKSIZE_V2	(8*1024)

#ifdef __KERNEL__

#include <linux/sunrpc/msg_prot.h>

/*
 * Largest number of bytes we need to allocate for an NFS
 * call or reply.  Used to control buffer sizes.  We use
 * the length of v3 WRITE, READDIR and READDIR replies
 * which are an RPC header, up to 26 XDR units of reply
 * data, and some page data.
 *
 * Note that accuracy here doesn't matter too much as the
 * size is rounded up to a page size when allocating space.
 */
#define NFSD_BUFSIZE		((RPC_MAX_HEADER_WITH_AUTH+26)*XDR_UNIT + NFSSVC_MAXBLKSIZE)

#ifdef CONFIG_NFSD_V4
# define NFSSVC_XDRSIZE		NFS4_SVC_XDRSIZE
#elif defined(CONFIG_NFSD_V3)
# define NFSSVC_XDRSIZE		NFS3_SVC_XDRSIZE
#else
# define NFSSVC_XDRSIZE		NFS2_SVC_XDRSIZE
#endif

#endif /* __KERNEL__ */

#endif /* _LINUX_NFSD_CONST_H */
