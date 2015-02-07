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

#ifndef _ALPHA_STAT_H
#define _ALPHA_STAT_H

struct stat {
	unsigned int	st_dev;
	unsigned int	st_ino;
	unsigned int	st_mode;
	unsigned int	st_nlink;
	unsigned int	st_uid;
	unsigned int	st_gid;
	unsigned int	st_rdev;
	long		st_size;
	unsigned long	st_atime;
	unsigned long	st_mtime;
	unsigned long	st_ctime;
	unsigned int	st_blksize;
	unsigned int	st_blocks;
	unsigned int	st_flags;
	unsigned int	st_gen;
};

/* The stat64 structure increases the size of dev_t, blkcnt_t, adds
   nanosecond resolution times, and padding for expansion.  */

struct stat64 {
	unsigned long	st_dev;
	unsigned long	st_ino;
	unsigned long	st_rdev;
	long		st_size;
	unsigned long	st_blocks;

	unsigned int	st_mode;
	unsigned int	st_uid;
	unsigned int	st_gid;
	unsigned int	st_blksize;
	unsigned int	st_nlink;
	unsigned int	__pad0;

	unsigned long	st_atime;
	unsigned long 	st_atime_nsec; 
	unsigned long	st_mtime;
	unsigned long	st_mtime_nsec;
	unsigned long	st_ctime;
	unsigned long   st_ctime_nsec;
  	long		__unused[3];
};

#endif
