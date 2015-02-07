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
 * Copyright (c) 2006, 2007 QLogic Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <asm/pgtable.h>

#include "ipath_verbs.h"

/**
 * ipath_release_mmap_info - free mmap info structure
 * @ref: a pointer to the kref within struct ipath_mmap_info
 */
void ipath_release_mmap_info(struct kref *ref)
{
	struct ipath_mmap_info *ip =
		container_of(ref, struct ipath_mmap_info, ref);
	struct ipath_ibdev *dev = to_idev(ip->context->device);

	spin_lock_irq(&dev->pending_lock);
	list_del(&ip->pending_mmaps);
	spin_unlock_irq(&dev->pending_lock);

	vfree(ip->obj);
	kfree(ip);
}

/*
 * open and close keep track of how many times the CQ is mapped,
 * to avoid releasing it.
 */
static void ipath_vma_open(struct vm_area_struct *vma)
{
	struct ipath_mmap_info *ip = vma->vm_private_data;

	kref_get(&ip->ref);
}

static void ipath_vma_close(struct vm_area_struct *vma)
{
	struct ipath_mmap_info *ip = vma->vm_private_data;

	kref_put(&ip->ref, ipath_release_mmap_info);
}

static struct vm_operations_struct ipath_vm_ops = {
	.open =     ipath_vma_open,
	.close =    ipath_vma_close,
};

/**
 * ipath_mmap - create a new mmap region
 * @context: the IB user context of the process making the mmap() call
 * @vma: the VMA to be initialized
 * Return zero if the mmap is OK. Otherwise, return an errno.
 */
int ipath_mmap(struct ib_ucontext *context, struct vm_area_struct *vma)
{
	struct ipath_ibdev *dev = to_idev(context->device);
	unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
	unsigned long size = vma->vm_end - vma->vm_start;
	struct ipath_mmap_info *ip, *pp;
	int ret = -EINVAL;

	/*
	 * Search the device's list of objects waiting for a mmap call.
	 * Normally, this list is very short since a call to create a
	 * CQ, QP, or SRQ is soon followed by a call to mmap().
	 */
	spin_lock_irq(&dev->pending_lock);
	list_for_each_entry_safe(ip, pp, &dev->pending_mmaps,
				 pending_mmaps) {
		/* Only the creator is allowed to mmap the object */
		if (context != ip->context || (__u64) offset != ip->offset)
			continue;
		/* Don't allow a mmap larger than the object. */
		if (size > ip->size)
			break;

		list_del_init(&ip->pending_mmaps);
		spin_unlock_irq(&dev->pending_lock);

		ret = remap_vmalloc_range(vma, ip->obj, 0);
		if (ret)
			goto done;
		vma->vm_ops = &ipath_vm_ops;
		vma->vm_private_data = ip;
		ipath_vma_open(vma);
		goto done;
	}
	spin_unlock_irq(&dev->pending_lock);
done:
	return ret;
}

/*
 * Allocate information for ipath_mmap
 */
struct ipath_mmap_info *ipath_create_mmap_info(struct ipath_ibdev *dev,
					       u32 size,
					       struct ib_ucontext *context,
					       void *obj) {
	struct ipath_mmap_info *ip;

	ip = kmalloc(sizeof *ip, GFP_KERNEL);
	if (!ip)
		goto bail;

	size = PAGE_ALIGN(size);

	spin_lock_irq(&dev->mmap_offset_lock);
	if (dev->mmap_offset == 0)
		dev->mmap_offset = PAGE_SIZE;
	ip->offset = dev->mmap_offset;
	dev->mmap_offset += size;
	spin_unlock_irq(&dev->mmap_offset_lock);

	INIT_LIST_HEAD(&ip->pending_mmaps);
	ip->size = size;
	ip->context = context;
	ip->obj = obj;
	kref_init(&ip->ref);

bail:
	return ip;
}

void ipath_update_mmap_info(struct ipath_ibdev *dev,
			    struct ipath_mmap_info *ip,
			    u32 size, void *obj) {
	size = PAGE_ALIGN(size);

	spin_lock_irq(&dev->mmap_offset_lock);
	if (dev->mmap_offset == 0)
		dev->mmap_offset = PAGE_SIZE;
	ip->offset = dev->mmap_offset;
	dev->mmap_offset += size;
	spin_unlock_irq(&dev->mmap_offset_lock);

	ip->size = size;
	ip->obj = obj;
}
