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

#ifndef _PARISC_FCNTL_H
#define _PARISC_FCNTL_H

/* open/fcntl - O_SYNC is only implemented on blocks devices and on files
   located on an ext2 file system */
#define O_APPEND	000000010
#define O_BLKSEEK	000000100 /* HPUX only */
#define O_CREAT		000000400 /* not fcntl */
#define O_EXCL		000002000 /* not fcntl */
#define O_LARGEFILE	000004000
#define O_SYNC		000100000
#define O_NONBLOCK	000200004 /* HPUX has separate NDELAY & NONBLOCK */
#define O_NOCTTY	000400000 /* not fcntl */
#define O_DSYNC		001000000 /* HPUX only */
#define O_RSYNC		002000000 /* HPUX only */
#define O_NOATIME	004000000
#define O_CLOEXEC	010000000 /* set close_on_exec */

#define O_DIRECTORY	000010000 /* must be a directory */
#define O_NOFOLLOW	000000200 /* don't follow links */
#define O_INVISIBLE	004000000 /* invisible I/O, for DMAPI/XDSM */

#define F_GETLK64	8
#define F_SETLK64	9
#define F_SETLKW64	10

#define F_GETOWN	11	/*  for sockets. */
#define F_SETOWN	12	/*  for sockets. */
#define F_SETSIG	13	/*  for sockets. */
#define F_GETSIG	14	/*  for sockets. */

/* for posix fcntl() and lockf() */
#define F_RDLCK		01
#define F_WRLCK		02
#define F_UNLCK		03

#include <asm-generic/fcntl.h>

#endif
