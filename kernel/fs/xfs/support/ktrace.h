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
 * Copyright (c) 2000-2003,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef __XFS_SUPPORT_KTRACE_H__
#define __XFS_SUPPORT_KTRACE_H__

/*
 * Trace buffer entry structure.
 */
typedef struct ktrace_entry {
	void	*val[16];
} ktrace_entry_t;

/*
 * Trace buffer header structure.
 */
typedef struct ktrace {
	int		kt_nentries;	/* number of entries in trace buf */
	atomic_t	kt_index;	/* current index in entries */
	unsigned int	kt_index_mask;
	int		kt_rollover;
	ktrace_entry_t	*kt_entries;	/* buffer of entries */
} ktrace_t;

/*
 * Trace buffer snapshot structure.
 */
typedef struct ktrace_snap {
	int		ks_start;	/* kt_index at time of snap */
	int		ks_index;	/* current index */
} ktrace_snap_t;


#ifdef CONFIG_XFS_TRACE

extern void ktrace_init(int zentries);
extern void ktrace_uninit(void);

extern ktrace_t *ktrace_alloc(int, unsigned int __nocast);
extern void ktrace_free(ktrace_t *);

extern void ktrace_enter(
	ktrace_t	*,
	void		*,
	void		*,
	void		*,
	void		*,
	void		*,
	void		*,
	void		*,
	void		*,
	void		*,
	void		*,
	void		*,
	void		*,
	void		*,
	void		*,
	void		*,
	void		*);

extern ktrace_entry_t   *ktrace_first(ktrace_t *, ktrace_snap_t *);
extern int              ktrace_nentries(ktrace_t *);
extern ktrace_entry_t   *ktrace_next(ktrace_t *, ktrace_snap_t *);
extern ktrace_entry_t   *ktrace_skip(ktrace_t *, int, ktrace_snap_t *);

#else
#define ktrace_init(x)	do { } while (0)
#define ktrace_uninit()	do { } while (0)
#endif	/* CONFIG_XFS_TRACE */

#endif	/* __XFS_SUPPORT_KTRACE_H__ */
