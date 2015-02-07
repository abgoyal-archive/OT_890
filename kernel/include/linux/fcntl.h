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

#ifndef _LINUX_FCNTL_H
#define _LINUX_FCNTL_H

#include <asm/fcntl.h>

#define F_SETLEASE	(F_LINUX_SPECIFIC_BASE + 0)
#define F_GETLEASE	(F_LINUX_SPECIFIC_BASE + 1)

/*
 * Cancel a blocking posix lock; internal use only until we expose an
 * asynchronous lock api to userspace:
 */
#define F_CANCELLK	(F_LINUX_SPECIFIC_BASE + 5)

/* Create a file descriptor with FD_CLOEXEC set. */
#define F_DUPFD_CLOEXEC	(F_LINUX_SPECIFIC_BASE + 6)

/*
 * Request nofications on a directory.
 * See below for events that may be notified.
 */
#define F_NOTIFY	(F_LINUX_SPECIFIC_BASE+2)

/*
 * Types of directory notifications that may be requested.
 */
#define DN_ACCESS	0x00000001	/* File accessed */
#define DN_MODIFY	0x00000002	/* File modified */
#define DN_CREATE	0x00000004	/* File created */
#define DN_DELETE	0x00000008	/* File removed */
#define DN_RENAME	0x00000010	/* File renamed */
#define DN_ATTRIB	0x00000020	/* File changed attibutes */
#define DN_MULTISHOT	0x80000000	/* Don't remove notifier */

#define AT_FDCWD		-100    /* Special value used to indicate
                                           openat should use the current
                                           working directory. */
#define AT_SYMLINK_NOFOLLOW	0x100   /* Do not follow symbolic links.  */
#define AT_REMOVEDIR		0x200   /* Remove directory instead of
                                           unlinking file.  */
#define AT_SYMLINK_FOLLOW	0x400   /* Follow symbolic links.  */

#ifdef __KERNEL__

#ifndef force_o_largefile
#define force_o_largefile() (BITS_PER_LONG != 32)
#endif

#if BITS_PER_LONG == 32
#define IS_GETLK32(cmd)		((cmd) == F_GETLK)
#define IS_SETLK32(cmd)		((cmd) == F_SETLK)
#define IS_SETLKW32(cmd)	((cmd) == F_SETLKW)
#define IS_GETLK64(cmd)		((cmd) == F_GETLK64)
#define IS_SETLK64(cmd)		((cmd) == F_SETLK64)
#define IS_SETLKW64(cmd)	((cmd) == F_SETLKW64)
#else
#define IS_GETLK32(cmd)		(0)
#define IS_SETLK32(cmd)		(0)
#define IS_SETLKW32(cmd)	(0)
#define IS_GETLK64(cmd)		((cmd) == F_GETLK)
#define IS_SETLK64(cmd)		((cmd) == F_SETLK)
#define IS_SETLKW64(cmd)	((cmd) == F_SETLKW)
#endif /* BITS_PER_LONG == 32 */

#define IS_GETLK(cmd)	(IS_GETLK32(cmd)  || IS_GETLK64(cmd))
#define IS_SETLK(cmd)	(IS_SETLK32(cmd)  || IS_SETLK64(cmd))
#define IS_SETLKW(cmd)	(IS_SETLKW32(cmd) || IS_SETLKW64(cmd))

#endif /* __KERNEL__ */

#endif
