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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1995, 1999 by Ralf Baechle
 */
#ifndef _ASM_STATFS_H
#define _ASM_STATFS_H

#include <linux/posix_types.h>
#include <asm/sgidefs.h>

#ifndef __KERNEL_STRICT_NAMES

#include <linux/types.h>

typedef __kernel_fsid_t        fsid_t;

#endif

struct statfs {
	long		f_type;
#define f_fstyp f_type
	long		f_bsize;
	long		f_frsize;	/* Fragment size - unsupported */
	long		f_blocks;
	long		f_bfree;
	long		f_files;
	long		f_ffree;
	long		f_bavail;

	/* Linux specials */
	__kernel_fsid_t	f_fsid;
	long		f_namelen;
	long		f_spare[6];
};

#if (_MIPS_SIM == _MIPS_SIM_ABI32) || (_MIPS_SIM == _MIPS_SIM_NABI32)

/*
 * Unlike the traditional version the LFAPI version has none of the ABI junk
 */
struct statfs64 {
	__u32	f_type;
	__u32	f_bsize;
	__u32	f_frsize;	/* Fragment size - unsupported */
	__u32	__pad;
	__u64	f_blocks;
	__u64	f_bfree;
	__u64	f_files;
	__u64	f_ffree;
	__u64	f_bavail;
	__kernel_fsid_t f_fsid;
	__u32	f_namelen;
	__u32	f_spare[6];
};

#endif /* _MIPS_SIM == _MIPS_SIM_ABI32 */

#if _MIPS_SIM == _MIPS_SIM_ABI64

struct statfs64 {			/* Same as struct statfs */
	long		f_type;
	long		f_bsize;
	long		f_frsize;	/* Fragment size - unsupported */
	long		f_blocks;
	long		f_bfree;
	long		f_files;
	long		f_ffree;
	long		f_bavail;

	/* Linux specials */
	__kernel_fsid_t	f_fsid;
	long		f_namelen;
	long		f_spare[6];
};

struct compat_statfs64 {
	__u32	f_type;
	__u32	f_bsize;
	__u32	f_frsize;	/* Fragment size - unsupported */
	__u32	__pad;
	__u64	f_blocks;
	__u64	f_bfree;
	__u64	f_files;
	__u64	f_ffree;
	__u64	f_bavail;
	__kernel_fsid_t f_fsid;
	__u32	f_namelen;
	__u32	f_spare[6];
};

#endif /* _MIPS_SIM == _MIPS_SIM_ABI64 */

#endif /* _ASM_STATFS_H */
