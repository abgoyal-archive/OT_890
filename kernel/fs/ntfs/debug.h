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
 * debug.h - NTFS kernel debug support. Part of the Linux-NTFS project.
 *
 * Copyright (c) 2001-2004 Anton Altaparmakov
 *
 * This program/include file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program/include file is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the main directory of the Linux-NTFS
 * distribution in the file COPYING); if not, write to the Free Software
 * Foundation,Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _LINUX_NTFS_DEBUG_H
#define _LINUX_NTFS_DEBUG_H

#include <linux/fs.h>

#include "runlist.h"

#ifdef DEBUG

extern int debug_msgs;

extern void __ntfs_debug(const char *file, int line, const char *function,
	const char *format, ...) __attribute__ ((format (printf, 4, 5)));
/**
 * ntfs_debug - write a debug level message to syslog
 * @f:		a printf format string containing the message
 * @...:	the variables to substitute into @f
 *
 * ntfs_debug() writes a DEBUG level message to the syslog but only if the
 * driver was compiled with -DDEBUG. Otherwise, the call turns into a NOP.
 */
#define ntfs_debug(f, a...)						\
	__ntfs_debug(__FILE__, __LINE__, __func__, f, ##a)

extern void ntfs_debug_dump_runlist(const runlist_element *rl);

#else	/* !DEBUG */

#define ntfs_debug(f, a...)		do {} while (0)
#define ntfs_debug_dump_runlist(rl)	do {} while (0)

#endif	/* !DEBUG */

extern void __ntfs_warning(const char *function, const struct super_block *sb,
		const char *fmt, ...) __attribute__ ((format (printf, 3, 4)));
#define ntfs_warning(sb, f, a...)	__ntfs_warning(__func__, sb, f, ##a)

extern void __ntfs_error(const char *function, const struct super_block *sb,
		const char *fmt, ...) __attribute__ ((format (printf, 3, 4)));
#define ntfs_error(sb, f, a...)		__ntfs_error(__func__, sb, f, ##a)

#endif /* _LINUX_NTFS_DEBUG_H */
