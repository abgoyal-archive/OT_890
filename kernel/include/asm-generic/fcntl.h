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

#ifndef _ASM_GENERIC_FCNTL_H
#define _ASM_GENERIC_FCNTL_H

#include <linux/types.h>

/* open/fcntl - O_SYNC is only implemented on blocks devices and on files
   located on an ext2 file system */
#define O_ACCMODE	00000003
#define O_RDONLY	00000000
#define O_WRONLY	00000001
#define O_RDWR		00000002
#ifndef O_CREAT
#define O_CREAT		00000100	/* not fcntl */
#endif
#ifndef O_EXCL
#define O_EXCL		00000200	/* not fcntl */
#endif
#ifndef O_NOCTTY
#define O_NOCTTY	00000400	/* not fcntl */
#endif
#ifndef O_TRUNC
#define O_TRUNC		00001000	/* not fcntl */
#endif
#ifndef O_APPEND
#define O_APPEND	00002000
#endif
#ifndef O_NONBLOCK
#define O_NONBLOCK	00004000
#endif
#ifndef O_SYNC
#define O_SYNC		00010000
#endif
#ifndef FASYNC
#define FASYNC		00020000	/* fcntl, for BSD compatibility */
#endif
#ifndef O_DIRECT
#define O_DIRECT	00040000	/* direct disk access hint */
#endif
#ifndef O_LARGEFILE
#define O_LARGEFILE	00100000
#endif
#ifndef O_DIRECTORY
#define O_DIRECTORY	00200000	/* must be a directory */
#endif
#ifndef O_NOFOLLOW
#define O_NOFOLLOW	00400000	/* don't follow links */
#endif
#ifndef O_NOATIME
#define O_NOATIME	01000000
#endif
#ifndef O_CLOEXEC
#define O_CLOEXEC	02000000	/* set close_on_exec */
#endif
#ifndef O_NDELAY
#define O_NDELAY	O_NONBLOCK
#endif

#define F_DUPFD		0	/* dup */
#define F_GETFD		1	/* get close_on_exec */
#define F_SETFD		2	/* set/clear close_on_exec */
#define F_GETFL		3	/* get file->f_flags */
#define F_SETFL		4	/* set file->f_flags */
#ifndef F_GETLK
#define F_GETLK		5
#define F_SETLK		6
#define F_SETLKW	7
#endif
#ifndef F_SETOWN
#define F_SETOWN	8	/* for sockets. */
#define F_GETOWN	9	/* for sockets. */
#endif
#ifndef F_SETSIG
#define F_SETSIG	10	/* for sockets. */
#define F_GETSIG	11	/* for sockets. */
#endif

/* for F_[GET|SET]FL */
#define FD_CLOEXEC	1	/* actually anything with low bit set goes */

/* for posix fcntl() and lockf() */
#ifndef F_RDLCK
#define F_RDLCK		0
#define F_WRLCK		1
#define F_UNLCK		2
#endif

/* for old implementation of bsd flock () */
#ifndef F_EXLCK
#define F_EXLCK		4	/* or 3 */
#define F_SHLCK		8	/* or 4 */
#endif

/* for leases */
#ifndef F_INPROGRESS
#define F_INPROGRESS	16
#endif

/* operations for bsd flock(), also used by the kernel implementation */
#define LOCK_SH		1	/* shared lock */
#define LOCK_EX		2	/* exclusive lock */
#define LOCK_NB		4	/* or'd with one of the above to prevent
				   blocking */
#define LOCK_UN		8	/* remove lock */

#define LOCK_MAND	32	/* This is a mandatory flock ... */
#define LOCK_READ	64	/* which allows concurrent read operations */
#define LOCK_WRITE	128	/* which allows concurrent write operations */
#define LOCK_RW		192	/* which allows concurrent read & write ops */

#define F_LINUX_SPECIFIC_BASE	1024

#ifndef HAVE_ARCH_STRUCT_FLOCK
#ifndef __ARCH_FLOCK_PAD
#define __ARCH_FLOCK_PAD
#endif

struct flock {
	short	l_type;
	short	l_whence;
	off_t	l_start;
	off_t	l_len;
	pid_t	l_pid;
	__ARCH_FLOCK_PAD
};
#endif

#ifndef CONFIG_64BIT

#ifndef F_GETLK64
#define F_GETLK64	12	/*  using 'struct flock64' */
#define F_SETLK64	13
#define F_SETLKW64	14
#endif

#ifndef HAVE_ARCH_STRUCT_FLOCK64
#ifndef __ARCH_FLOCK64_PAD
#define __ARCH_FLOCK64_PAD
#endif

struct flock64 {
	short  l_type;
	short  l_whence;
	loff_t l_start;
	loff_t l_len;
	pid_t  l_pid;
	__ARCH_FLOCK64_PAD
};
#endif
#endif /* !CONFIG_64BIT */

#endif /* _ASM_GENERIC_FCNTL_H */
