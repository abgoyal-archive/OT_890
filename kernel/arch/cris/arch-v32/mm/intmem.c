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
 * Simple allocator for internal RAM in ETRAX FS
 *
 * Copyright (c) 2004 Axis Communications AB.
 */

#include <linux/list.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <memmap.h>

#define STATUS_FREE 0
#define STATUS_ALLOCATED 1

#ifdef CONFIG_ETRAX_L2CACHE
#define RESERVED_SIZE 66*1024
#else
#define RESERVED_SIZE 0
#endif

struct intmem_allocation {
	struct list_head entry;
	unsigned int size;
	unsigned offset;
	char status;
};


static struct list_head intmem_allocations;
static void* intmem_virtual;

static void crisv32_intmem_init(void)
{
	static int initiated = 0;
	if (!initiated) {
		struct intmem_allocation* alloc =
		  (struct intmem_allocation*)kmalloc(sizeof *alloc, GFP_KERNEL);
		INIT_LIST_HEAD(&intmem_allocations);
		intmem_virtual = ioremap(MEM_INTMEM_START + RESERVED_SIZE,
					 MEM_INTMEM_SIZE - RESERVED_SIZE);
		initiated = 1;
		alloc->size = MEM_INTMEM_SIZE - RESERVED_SIZE;
		alloc->offset = 0;
		alloc->status = STATUS_FREE;
		list_add_tail(&alloc->entry, &intmem_allocations);
	}
}

void* crisv32_intmem_alloc(unsigned size, unsigned align)
{
	struct intmem_allocation* allocation;
	struct intmem_allocation* tmp;
	void* ret = NULL;

	preempt_disable();
	crisv32_intmem_init();

	list_for_each_entry_safe(allocation, tmp, &intmem_allocations, entry) {
		int alignment = allocation->offset % align;
		alignment = alignment ? align - alignment : alignment;

		if (allocation->status == STATUS_FREE &&
		    allocation->size >= size + alignment) {
			if (allocation->size > size + alignment) {
				struct intmem_allocation* alloc =
					(struct intmem_allocation*)
					kmalloc(sizeof *alloc, GFP_ATOMIC);
				alloc->status = STATUS_FREE;
				alloc->size = allocation->size - size -
					alignment;
				alloc->offset = allocation->offset + size +
					alignment;
				list_add(&alloc->entry, &allocation->entry);

				if (alignment) {
					struct intmem_allocation *tmp;
					tmp = (struct intmem_allocation *)
						kmalloc(sizeof *tmp,
							GFP_ATOMIC);
					tmp->offset = allocation->offset;
					tmp->size = alignment;
					tmp->status = STATUS_FREE;
					allocation->offset += alignment;
					list_add_tail(&tmp->entry,
						&allocation->entry);
				}
			}
			allocation->status = STATUS_ALLOCATED;
			allocation->size = size;
			ret = (void*)((int)intmem_virtual + allocation->offset);
		}
	}
	preempt_enable();
	return ret;
}

void crisv32_intmem_free(void* addr)
{
	struct intmem_allocation* allocation;
	struct intmem_allocation* tmp;

	if (addr == NULL)
		return;

	preempt_disable();
	crisv32_intmem_init();

	list_for_each_entry_safe(allocation, tmp, &intmem_allocations, entry) {
		if (allocation->offset == (int)(addr - intmem_virtual)) {
			struct intmem_allocation *prev =
			  list_entry(allocation->entry.prev,
			             struct intmem_allocation, entry);
			struct intmem_allocation *next =
			  list_entry(allocation->entry.next,
				     struct intmem_allocation, entry);

			allocation->status = STATUS_FREE;
			/* Join with prev and/or next if also free */
			if ((prev != &intmem_allocations) &&
					(prev->status == STATUS_FREE)) {
				prev->size += allocation->size;
				list_del(&allocation->entry);
				kfree(allocation);
				allocation = prev;
			}
			if ((next != &intmem_allocations) &&
					(next->status == STATUS_FREE)) {
				allocation->size += next->size;
				list_del(&next->entry);
				kfree(next);
			}
			preempt_enable();
			return;
		}
	}
	preempt_enable();
}

void* crisv32_intmem_phys_to_virt(unsigned long addr)
{
	return (void *)(addr - (MEM_INTMEM_START + RESERVED_SIZE) +
		(unsigned long)intmem_virtual);
}

unsigned long crisv32_intmem_virt_to_phys(void* addr)
{
	return (unsigned long)((unsigned long )addr -
		(unsigned long)intmem_virtual + MEM_INTMEM_START +
		RESERVED_SIZE);
}

module_init(crisv32_intmem_init);

