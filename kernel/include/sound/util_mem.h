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

#ifndef __SOUND_UTIL_MEM_H
#define __SOUND_UTIL_MEM_H

#include <linux/mutex.h>
/*
 *  Copyright (C) 2000 Takashi Iwai <tiwai@suse.de>
 *
 *  Generic memory management routines for soundcard memory allocation
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

/*
 * memory block
 */
struct snd_util_memblk {
	unsigned int size;		/* size of this block */
	unsigned int offset;		/* zero-offset of this block */
	struct list_head list;		/* link */
};

#define snd_util_memblk_argptr(blk)	(void*)((char*)(blk) + sizeof(struct snd_util_memblk))

/*
 * memory management information
 */
struct snd_util_memhdr {
	unsigned int size;		/* size of whole data */
	struct list_head block;		/* block linked-list header */
	int nblocks;			/* # of allocated blocks */
	unsigned int used;		/* used memory size */
	int block_extra_size;		/* extra data size of chunk */
	struct mutex block_mutex;	/* lock */
};

/*
 * prototypes
 */
struct snd_util_memhdr *snd_util_memhdr_new(int memsize);
void snd_util_memhdr_free(struct snd_util_memhdr *hdr);
struct snd_util_memblk *snd_util_mem_alloc(struct snd_util_memhdr *hdr, int size);
int snd_util_mem_free(struct snd_util_memhdr *hdr, struct snd_util_memblk *blk);
int snd_util_mem_avail(struct snd_util_memhdr *hdr);

/* functions without mutex */
struct snd_util_memblk *__snd_util_mem_alloc(struct snd_util_memhdr *hdr, int size);
void __snd_util_mem_free(struct snd_util_memhdr *hdr, struct snd_util_memblk *blk);
struct snd_util_memblk *__snd_util_memblk_new(struct snd_util_memhdr *hdr,
					      unsigned int units,
					      struct list_head *prev);

#endif /* __SOUND_UTIL_MEM_H */
