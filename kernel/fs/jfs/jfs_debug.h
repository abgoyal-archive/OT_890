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
 *   Copyright (C) International Business Machines Corp., 2000-2002
 *   Portions Copyright (C) Christoph Hellwig, 2001-2002
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#ifndef _H_JFS_DEBUG
#define _H_JFS_DEBUG

/*
 *	jfs_debug.h
 *
 * global debug message, data structure/macro definitions
 * under control of CONFIG_JFS_DEBUG, CONFIG_JFS_STATISTICS;
 */

/*
 * Create /proc/fs/jfs if procfs is enabled andeither
 * CONFIG_JFS_DEBUG or CONFIG_JFS_STATISTICS is defined
 */
#if defined(CONFIG_PROC_FS) && (defined(CONFIG_JFS_DEBUG) || defined(CONFIG_JFS_STATISTICS))
#define PROC_FS_JFS
extern void jfs_proc_init(void);
extern void jfs_proc_clean(void);
#endif

/*
 *	assert with traditional printf/panic
 */
#define assert(p) do {	\
	if (!(p)) {	\
		printk(KERN_CRIT "BUG at %s:%d assert(%s)\n",	\
		       __FILE__, __LINE__, #p);			\
		BUG();	\
	}		\
} while (0)

/*
 *	debug ON
 *	--------
 */
#ifdef CONFIG_JFS_DEBUG
#define ASSERT(p) assert(p)

/* printk verbosity */
#define JFS_LOGLEVEL_ERR 1
#define JFS_LOGLEVEL_WARN 2
#define JFS_LOGLEVEL_DEBUG 3
#define JFS_LOGLEVEL_INFO 4

extern int jfsloglevel;

extern const struct file_operations jfs_txanchor_proc_fops;

/* information message: e.g., configuration, major event */
#define jfs_info(fmt, arg...) do {			\
	if (jfsloglevel >= JFS_LOGLEVEL_INFO)		\
		printk(KERN_INFO fmt "\n", ## arg);	\
} while (0)

/* debug message: ad hoc */
#define jfs_debug(fmt, arg...) do {			\
	if (jfsloglevel >= JFS_LOGLEVEL_DEBUG)		\
		printk(KERN_DEBUG fmt "\n", ## arg);	\
} while (0)

/* warn message: */
#define jfs_warn(fmt, arg...) do {			\
	if (jfsloglevel >= JFS_LOGLEVEL_WARN)		\
		printk(KERN_WARNING fmt "\n", ## arg);	\
} while (0)

/* error event message: e.g., i/o error */
#define jfs_err(fmt, arg...) do {			\
	if (jfsloglevel >= JFS_LOGLEVEL_ERR)		\
		printk(KERN_ERR fmt "\n", ## arg);	\
} while (0)

/*
 *	debug OFF
 *	---------
 */
#else				/* CONFIG_JFS_DEBUG */
#define ASSERT(p) do {} while (0)
#define jfs_info(fmt, arg...) do {} while (0)
#define jfs_debug(fmt, arg...) do {} while (0)
#define jfs_warn(fmt, arg...) do {} while (0)
#define jfs_err(fmt, arg...) do {} while (0)
#endif				/* CONFIG_JFS_DEBUG */

/*
 *	statistics
 *	----------
 */
#ifdef	CONFIG_JFS_STATISTICS
extern const struct file_operations jfs_lmstats_proc_fops;
extern const struct file_operations jfs_txstats_proc_fops;
extern const struct file_operations jfs_mpstat_proc_fops;
extern const struct file_operations jfs_xtstat_proc_fops;

#define	INCREMENT(x)		((x)++)
#define	DECREMENT(x)		((x)--)
#define	HIGHWATERMARK(x,y)	((x) = max((x), (y)))
#else
#define	INCREMENT(x)
#define	DECREMENT(x)
#define	HIGHWATERMARK(x,y)
#endif				/* CONFIG_JFS_STATISTICS */

#endif				/* _H_JFS_DEBUG */
