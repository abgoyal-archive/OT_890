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

#ifndef _ALPHA_FCNTL_H
#define _ALPHA_FCNTL_H

/* open/fcntl - O_SYNC is only implemented on blocks devices and on files
   located on an ext2 file system */
#define O_CREAT		 01000	/* not fcntl */
#define O_TRUNC		 02000	/* not fcntl */
#define O_EXCL		 04000	/* not fcntl */
#define O_NOCTTY	010000	/* not fcntl */

#define O_NONBLOCK	 00004
#define O_APPEND	 00010
#define O_SYNC		040000
#define O_DIRECTORY	0100000	/* must be a directory */
#define O_NOFOLLOW	0200000 /* don't follow links */
#define O_LARGEFILE	0400000 /* will be set by the kernel on every open */
#define O_DIRECT	02000000 /* direct disk access - should check with OSF/1 */
#define O_NOATIME	04000000
#define O_CLOEXEC	010000000 /* set close_on_exec */

#define F_GETLK		7
#define F_SETLK		8
#define F_SETLKW	9

#define F_SETOWN	5	/*  for sockets. */
#define F_GETOWN	6	/*  for sockets. */
#define F_SETSIG	10	/*  for sockets. */
#define F_GETSIG	11	/*  for sockets. */

/* for posix fcntl() and lockf() */
#define F_RDLCK		1
#define F_WRLCK		2
#define F_UNLCK		8

/* for old implementation of bsd flock () */
#define F_EXLCK		16	/* or 3 */
#define F_SHLCK		32	/* or 4 */

#define F_INPROGRESS	64

#include <asm-generic/fcntl.h>

#endif
