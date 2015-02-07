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
 * Simple insertion-only static-sized priority heap containing
 * pointers, based on CLR, chapter 7
 */

#include <linux/slab.h>
#include <linux/prio_heap.h>

int heap_init(struct ptr_heap *heap, size_t size, gfp_t gfp_mask,
	      int (*gt)(void *, void *))
{
	heap->ptrs = kmalloc(size, gfp_mask);
	if (!heap->ptrs)
		return -ENOMEM;
	heap->size = 0;
	heap->max = size / sizeof(void *);
	heap->gt = gt;
	return 0;
}

void heap_free(struct ptr_heap *heap)
{
	kfree(heap->ptrs);
}

void *heap_insert(struct ptr_heap *heap, void *p)
{
	void *res;
	void **ptrs = heap->ptrs;
	int pos;

	if (heap->size < heap->max) {
		/* Heap insertion */
		pos = heap->size++;
		while (pos > 0 && heap->gt(p, ptrs[(pos-1)/2])) {
			ptrs[pos] = ptrs[(pos-1)/2];
			pos = (pos-1)/2;
		}
		ptrs[pos] = p;
		return NULL;
	}

	/* The heap is full, so something will have to be dropped */

	/* If the new pointer is greater than the current max, drop it */
	if (heap->gt(p, ptrs[0]))
		return p;

	/* Replace the current max and heapify */
	res = ptrs[0];
	ptrs[0] = p;
	pos = 0;

	while (1) {
		int left = 2 * pos + 1;
		int right = 2 * pos + 2;
		int largest = pos;
		if (left < heap->size && heap->gt(ptrs[left], p))
			largest = left;
		if (right < heap->size && heap->gt(ptrs[right], ptrs[largest]))
			largest = right;
		if (largest == pos)
			break;
		/* Push p down the heap one level and bump one up */
		ptrs[pos] = ptrs[largest];
		ptrs[largest] = p;
		pos = largest;
	}
	return res;
}
