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
 * File: linux/nfsacl.h
 *
 * (C) 2003 Andreas Gruenbacher <agruen@suse.de>
 */
#ifndef __LINUX_NFSACL_H
#define __LINUX_NFSACL_H

#define NFS_ACL_PROGRAM	100227

#define ACLPROC2_GETACL		1
#define ACLPROC2_SETACL		2
#define ACLPROC2_GETATTR	3
#define ACLPROC2_ACCESS		4

#define ACLPROC3_GETACL		1
#define ACLPROC3_SETACL		2


/* Flags for the getacl/setacl mode */
#define NFS_ACL			0x0001
#define NFS_ACLCNT		0x0002
#define NFS_DFACL		0x0004
#define NFS_DFACLCNT		0x0008

/* Flag for Default ACL entries */
#define NFS_ACL_DEFAULT		0x1000

#ifdef __KERNEL__

#include <linux/posix_acl.h>

/* Maximum number of ACL entries over NFS */
#define NFS_ACL_MAX_ENTRIES	1024

#define NFSACL_MAXWORDS		(2*(2+3*NFS_ACL_MAX_ENTRIES))
#define NFSACL_MAXPAGES		((2*(8+12*NFS_ACL_MAX_ENTRIES) + PAGE_SIZE-1) \
				 >> PAGE_SHIFT)

#define NFS_ACL_MAX_ENTRIES_INLINE	(5)
#define NFS_ACL_INLINE_BUFSIZE	((2*(2+3*NFS_ACL_MAX_ENTRIES_INLINE)) << 2)

static inline unsigned int
nfsacl_size(struct posix_acl *acl_access, struct posix_acl *acl_default)
{
	unsigned int w = 16;
	w += max(acl_access ? (int)acl_access->a_count : 3, 4) * 12;
	if (acl_default)
		w += max((int)acl_default->a_count, 4) * 12;
	return w;
}

extern unsigned int
nfsacl_encode(struct xdr_buf *buf, unsigned int base, struct inode *inode,
	      struct posix_acl *acl, int encode_entries, int typeflag);
extern unsigned int
nfsacl_decode(struct xdr_buf *buf, unsigned int base, unsigned int *aclcnt,
	      struct posix_acl **pacl);

#endif /* __KERNEL__ */
#endif  /* __LINUX_NFSACL_H */
