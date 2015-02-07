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
 *   fs/cifs/export.c
 *
 *   Copyright (C) International Business Machines  Corp., 2007
 *   Author(s): Steve French (sfrench@us.ibm.com)
 *
 *   Common Internet FileSystem (CIFS) client
 *
 *   Operations related to support for exporting files via NFSD
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published
 *   by the Free Software Foundation; either version 2.1 of the License, or
 *   (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

 /*
  * See Documentation/filesystems/Exporting
  * and examples in fs/exportfs
  *
  * Since cifs is a network file system, an "fsid" must be included for
  * any nfs exports file entries which refer to cifs paths.  In addition
  * the cifs mount must be mounted with the "serverino" option (ie use stable
  * server inode numbers instead of locally generated temporary ones).
  * Although cifs inodes do not use generation numbers (have generation number
  * of zero) - the inode number alone should be good enough for simple cases
  * in which users want to export cifs shares with NFS. The decode and encode
  * could be improved by using a new routine which expects 64 bit inode numbers
  * instead of the default 32 bit routines in fs/exportfs
  *
  */

#include <linux/fs.h>
#include <linux/exportfs.h>
#include "cifsglob.h"
#include "cifs_debug.h"
#include "cifsfs.h"

#ifdef CONFIG_CIFS_EXPERIMENTAL
static struct dentry *cifs_get_parent(struct dentry *dentry)
{
	/* BB need to add code here eventually to enable export via NFSD */
	cFYI(1, ("get parent for %p", dentry));
	return ERR_PTR(-EACCES);
}

const struct export_operations cifs_export_ops = {
	.get_parent = cifs_get_parent,
/*	Following five export operations are unneeded so far and can default:
	.get_dentry =
	.get_name =
	.find_exported_dentry =
	.decode_fh =
	.encode_fs =  */
};

#endif /* EXPERIMENTAL */

