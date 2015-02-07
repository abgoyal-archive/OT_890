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
 * AGPGART backend specific includes. Not for userspace consumption.
 *
 * Copyright (C) 2004 Silicon Graphics, Inc.
 * Copyright (C) 2002-2003 Dave Jones
 * Copyright (C) 1999 Jeff Hartmann
 * Copyright (C) 1999 Precision Insight, Inc.
 * Copyright (C) 1999 Xi Graphics, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * JEFF HARTMANN, OR ANY OTHER CONTRIBUTORS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _AGP_BACKEND_H
#define _AGP_BACKEND_H 1

#include <linux/list.h>

enum chipset_type {
	NOT_SUPPORTED,
	SUPPORTED,
};

struct agp_version {
	u16 major;
	u16 minor;
};

struct agp_kern_info {
	struct agp_version version;
	struct pci_dev *device;
	enum chipset_type chipset;
	unsigned long mode;
	unsigned long aper_base;
	size_t aper_size;
	int max_memory;		/* In pages */
	int current_memory;
	bool cant_use_aperture;
	unsigned long page_mask;
	struct vm_operations_struct *vm_ops;
};

/*
 * The agp_memory structure has information about the block of agp memory
 * allocated.  A caller may manipulate the next and prev pointers to link
 * each allocated item into a list.  These pointers are ignored by the backend.
 * Everything else should never be written to, but the caller may read any of
 * the items to determine the status of this block of agp memory.
 */

struct agp_bridge_data;

struct agp_memory {
	struct agp_memory *next;
	struct agp_memory *prev;
	struct agp_bridge_data *bridge;
	unsigned long *memory;
	size_t page_count;
	int key;
	int num_scratch_pages;
	off_t pg_start;
	u32 type;
	u32 physical;
	bool is_bound;
	bool is_flushed;
        bool vmalloc_flag;
	/* list of agp_memory mapped to the aperture */
	struct list_head mapped_list;
};

#define AGP_NORMAL_MEMORY 0

#define AGP_USER_TYPES (1 << 16)
#define AGP_USER_MEMORY (AGP_USER_TYPES)
#define AGP_USER_CACHED_MEMORY (AGP_USER_TYPES + 1)

extern struct agp_bridge_data *agp_bridge;
extern struct list_head agp_bridges;

extern struct agp_bridge_data *(*agp_find_bridge)(struct pci_dev *);

extern void agp_free_memory(struct agp_memory *);
extern struct agp_memory *agp_allocate_memory(struct agp_bridge_data *, size_t, u32);
extern int agp_copy_info(struct agp_bridge_data *, struct agp_kern_info *);
extern int agp_bind_memory(struct agp_memory *, off_t);
extern int agp_unbind_memory(struct agp_memory *);
extern int agp_rebind_memory(void);
extern void agp_enable(struct agp_bridge_data *, u32);
extern struct agp_bridge_data *agp_backend_acquire(struct pci_dev *);
extern void agp_backend_release(struct agp_bridge_data *);
extern void agp_flush_chipset(struct agp_bridge_data *);

#endif				/* _AGP_BACKEND_H */
