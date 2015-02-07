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

/* AFS File Service definitions
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef AFS_FS_H
#define AFS_FS_H

#define AFS_FS_PORT		7000	/* AFS file server port */
#define FS_SERVICE		1	/* AFS File Service ID */

enum AFS_FS_Operations {
	FSFETCHDATA		= 130,	/* AFS Fetch file data */
	FSFETCHSTATUS		= 132,	/* AFS Fetch file status */
	FSSTOREDATA		= 133,	/* AFS Store file data */
	FSSTORESTATUS		= 135,	/* AFS Store file status */
	FSREMOVEFILE		= 136,	/* AFS Remove a file */
	FSCREATEFILE		= 137,	/* AFS Create a file */
	FSRENAME		= 138,	/* AFS Rename or move a file or directory */
	FSSYMLINK		= 139,	/* AFS Create a symbolic link */
	FSLINK			= 140,	/* AFS Create a hard link */
	FSMAKEDIR		= 141,	/* AFS Create a directory */
	FSREMOVEDIR		= 142,	/* AFS Remove a directory */
	FSGIVEUPCALLBACKS	= 147,	/* AFS Discard callback promises */
	FSGETVOLUMEINFO		= 148,	/* AFS Get information about a volume */
	FSGETVOLUMESTATUS	= 149,	/* AFS Get volume status information */
	FSGETROOTVOLUME		= 151,	/* AFS Get root volume name */
	FSSETLOCK		= 156,	/* AFS Request a file lock */
	FSEXTENDLOCK		= 157,	/* AFS Extend a file lock */
	FSRELEASELOCK		= 158,	/* AFS Release a file lock */
	FSLOOKUP		= 161,	/* AFS lookup file in directory */
	FSFETCHDATA64		= 65537, /* AFS Fetch file data */
	FSSTOREDATA64		= 65538, /* AFS Store file data */
};

enum AFS_FS_Errors {
	VSALVAGE	= 101,	/* volume needs salvaging */
	VNOVNODE	= 102,	/* no such file/dir (vnode) */
	VNOVOL		= 103,	/* no such volume or volume unavailable */
	VVOLEXISTS	= 104,	/* volume name already exists */
	VNOSERVICE	= 105,	/* volume not currently in service */
	VOFFLINE	= 106,	/* volume is currently offline (more info available [VVL-spec]) */
	VONLINE		= 107,	/* volume is already online */
	VDISKFULL	= 108,	/* disk partition is full */
	VOVERQUOTA	= 109,	/* volume's maximum quota exceeded */
	VBUSY		= 110,	/* volume is temporarily unavailable */
	VMOVED		= 111,	/* volume moved to new server - ask this FS where */
};

#endif /* AFS_FS_H */
