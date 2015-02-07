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
 * include/asm-xtensa/mman.h
 *
 * Xtensa Processor memory-manager definitions
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1995 by Ralf Baechle
 * Copyright (C) 2001 - 2005 Tensilica Inc.
 */

#ifndef _XTENSA_MMAN_H
#define _XTENSA_MMAN_H

/*
 * Protections are chosen from these bits, OR'd together.  The
 * implementation does not necessarily support PROT_EXEC or PROT_WRITE
 * without PROT_READ.  The only guarantees are that no writing will be
 * allowed without PROT_WRITE and no access will be allowed for PROT_NONE.
 */

#define PROT_NONE	0x0		/* page can not be accessed */
#define PROT_READ	0x1		/* page can be read */
#define PROT_WRITE	0x2		/* page can be written */
#define PROT_EXEC	0x4		/* page can be executed */

#define PROT_SEM	0x10		/* page may be used for atomic ops */
#define PROT_GROWSDOWN	0x01000000	/* mprotect flag: extend change to start of growsdown vma */
#define PROT_GROWSUP	0x02000000	/* mprotect flag: extend change to end fo growsup vma */

/*
 * Flags for mmap
 */
#define MAP_SHARED	0x001		/* Share changes */
#define MAP_PRIVATE	0x002		/* Changes are private */
#define MAP_TYPE	0x00f		/* Mask for type of mapping */
#define MAP_FIXED	0x010		/* Interpret addr exactly */

/* not used by linux, but here to make sure we don't clash with ABI defines */
#define MAP_RENAME	0x020		/* Assign page to file */
#define MAP_AUTOGROW	0x040		/* File may grow by writing */
#define MAP_LOCAL	0x080		/* Copy on fork/sproc */
#define MAP_AUTORSRV	0x100		/* Logical swap reserved on demand */

/* These are linux-specific */
#define MAP_NORESERVE	0x0400		/* don't check for reservations */
#define MAP_ANONYMOUS	0x0800		/* don't use a file */
#define MAP_GROWSDOWN	0x1000		/* stack-like segment */
#define MAP_DENYWRITE	0x2000		/* ETXTBSY */
#define MAP_EXECUTABLE	0x4000		/* mark it as an executable */
#define MAP_LOCKED	0x8000		/* pages are locked */
#define MAP_POPULATE	0x10000		/* populate (prefault) pagetables */
#define MAP_NONBLOCK	0x20000		/* do not block on IO */

/*
 * Flags for msync
 */
#define MS_ASYNC	0x0001		/* sync memory asynchronously */
#define MS_INVALIDATE	0x0002		/* invalidate mappings & caches */
#define MS_SYNC		0x0004		/* synchronous memory sync */

/*
 * Flags for mlockall
 */
#define MCL_CURRENT	1		/* lock all current mappings */
#define MCL_FUTURE	2		/* lock all future mappings */

#define MADV_NORMAL	0		/* no further special treatment */
#define MADV_RANDOM	1		/* expect random page references */
#define MADV_SEQUENTIAL	2		/* expect sequential page references */
#define MADV_WILLNEED	3		/* will need these pages */
#define MADV_DONTNEED	4		/* don't need these pages */

/* common parameters: try to keep these consistent across architectures */
#define MADV_REMOVE	9		/* remove these pages & resources */
#define MADV_DONTFORK	10		/* don't inherit across fork */
#define MADV_DOFORK	11		/* do inherit across fork */

/* compatibility flags */
#define MAP_FILE	0

#endif /* _XTENSA_MMAN_H */
