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
 * include/asm-ppc/rheap.h
 *
 * Header file for the implementation of a remote heap.
 *
 * Author: Pantelis Antoniou <panto@intracom.gr>
 *
 * 2004 (c) INTRACOM S.A. Greece. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#ifndef __ASM_PPC_RHEAP_H__
#define __ASM_PPC_RHEAP_H__

#include <linux/list.h>

typedef struct _rh_block {
	struct list_head list;
	unsigned long start;
	int size;
	const char *owner;
} rh_block_t;

typedef struct _rh_info {
	unsigned int alignment;
	int max_blocks;
	int empty_slots;
	rh_block_t *block;
	struct list_head empty_list;
	struct list_head free_list;
	struct list_head taken_list;
	unsigned int flags;
} rh_info_t;

#define RHIF_STATIC_INFO	0x1
#define RHIF_STATIC_BLOCK	0x2

typedef struct _rh_stats {
	unsigned long start;
	int size;
	const char *owner;
} rh_stats_t;

#define RHGS_FREE	0
#define RHGS_TAKEN	1

/* Create a remote heap dynamically */
extern rh_info_t *rh_create(unsigned int alignment);

/* Destroy a remote heap, created by rh_create() */
extern void rh_destroy(rh_info_t * info);

/* Initialize in place a remote info block */
extern void rh_init(rh_info_t * info, unsigned int alignment, int max_blocks,
		    rh_block_t * block);

/* Attach a free region to manage */
extern int rh_attach_region(rh_info_t * info, unsigned long start, int size);

/* Detach a free region */
extern unsigned long rh_detach_region(rh_info_t * info, unsigned long start, int size);

/* Allocate the given size from the remote heap (with alignment) */
extern unsigned long rh_alloc_align(rh_info_t * info, int size, int alignment,
		const char *owner);

/* Allocate the given size from the remote heap */
extern unsigned long rh_alloc(rh_info_t * info, int size, const char *owner);

/* Allocate the given size from the given address */
extern unsigned long rh_alloc_fixed(rh_info_t * info, unsigned long start, int size,
			    const char *owner);

/* Free the allocated area */
extern int rh_free(rh_info_t * info, unsigned long start);

/* Get stats for debugging purposes */
extern int rh_get_stats(rh_info_t * info, int what, int max_stats,
			rh_stats_t * stats);

/* Simple dump of remote heap info */
extern void rh_dump(rh_info_t * info);

/* Set owner of taken block */
extern int rh_set_owner(rh_info_t * info, unsigned long start, const char *owner);

#endif				/* __ASM_PPC_RHEAP_H__ */
