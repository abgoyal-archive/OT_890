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

#ifndef _GENERIC_STATFS_H
#define _GENERIC_STATFS_H

#ifndef __KERNEL_STRICT_NAMES
# include <linux/types.h>
typedef __kernel_fsid_t	fsid_t;
#endif

/*
 * Most 64-bit platforms use 'long', while most 32-bit platforms use '__u32'.
 * Yes, they differ in signedness as well as size.
 * Special cases can override it for themselves -- except for S390x, which
 * is just a little too special for us. And MIPS, which I'm not touching
 * with a 10' pole.
 */
#ifndef __statfs_word
#if BITS_PER_LONG == 64
#define __statfs_word long
#else
#define __statfs_word __u32
#endif
#endif

struct statfs {
	__statfs_word f_type;
	__statfs_word f_bsize;
	__statfs_word f_blocks;
	__statfs_word f_bfree;
	__statfs_word f_bavail;
	__statfs_word f_files;
	__statfs_word f_ffree;
	__kernel_fsid_t f_fsid;
	__statfs_word f_namelen;
	__statfs_word f_frsize;
	__statfs_word f_spare[5];
};

/*
 * ARM needs to avoid the 32-bit padding at the end, for consistency
 * between EABI and OABI 
 */
#ifndef ARCH_PACK_STATFS64
#define ARCH_PACK_STATFS64
#endif

struct statfs64 {
	__statfs_word f_type;
	__statfs_word f_bsize;
	__u64 f_blocks;
	__u64 f_bfree;
	__u64 f_bavail;
	__u64 f_files;
	__u64 f_ffree;
	__kernel_fsid_t f_fsid;
	__statfs_word f_namelen;
	__statfs_word f_frsize;
	__statfs_word f_spare[5];
} ARCH_PACK_STATFS64;

/* 
 * IA64 and x86_64 need to avoid the 32-bit padding at the end,
 * to be compatible with the i386 ABI
 */
#ifndef ARCH_PACK_COMPAT_STATFS64
#define ARCH_PACK_COMPAT_STATFS64
#endif

struct compat_statfs64 {
	__u32 f_type;
	__u32 f_bsize;
	__u64 f_blocks;
	__u64 f_bfree;
	__u64 f_bavail;
	__u64 f_files;
	__u64 f_ffree;
	__kernel_fsid_t f_fsid;
	__u32 f_namelen;
	__u32 f_frsize;
	__u32 f_spare[5];
} ARCH_PACK_COMPAT_STATFS64;

#endif
