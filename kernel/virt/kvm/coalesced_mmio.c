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
 * KVM coalesced MMIO
 *
 * Copyright (c) 2008 Bull S.A.S.
 *
 *  Author: Laurent Vivier <Laurent.Vivier@bull.net>
 *
 */

#include "iodev.h"

#include <linux/kvm_host.h>
#include <linux/kvm.h>

#include "coalesced_mmio.h"

static int coalesced_mmio_in_range(struct kvm_io_device *this,
				   gpa_t addr, int len, int is_write)
{
	struct kvm_coalesced_mmio_dev *dev =
				(struct kvm_coalesced_mmio_dev*)this->private;
	struct kvm_coalesced_mmio_zone *zone;
	int next;
	int i;

	if (!is_write)
		return 0;

	/* kvm->lock is taken by the caller and must be not released before
         * dev.read/write
         */

	/* Are we able to batch it ? */

	/* last is the first free entry
	 * check if we don't meet the first used entry
	 * there is always one unused entry in the buffer
	 */

	next = (dev->kvm->coalesced_mmio_ring->last + 1) %
							KVM_COALESCED_MMIO_MAX;
	if (next == dev->kvm->coalesced_mmio_ring->first) {
		/* full */
		return 0;
	}

	/* is it in a batchable area ? */

	for (i = 0; i < dev->nb_zones; i++) {
		zone = &dev->zone[i];

		/* (addr,len) is fully included in
		 * (zone->addr, zone->size)
		 */

		if (zone->addr <= addr &&
		    addr + len <= zone->addr + zone->size)
			return 1;
	}
	return 0;
}

static void coalesced_mmio_write(struct kvm_io_device *this,
				 gpa_t addr, int len, const void *val)
{
	struct kvm_coalesced_mmio_dev *dev =
				(struct kvm_coalesced_mmio_dev*)this->private;
	struct kvm_coalesced_mmio_ring *ring = dev->kvm->coalesced_mmio_ring;

	/* kvm->lock must be taken by caller before call to in_range()*/

	/* copy data in first free entry of the ring */

	ring->coalesced_mmio[ring->last].phys_addr = addr;
	ring->coalesced_mmio[ring->last].len = len;
	memcpy(ring->coalesced_mmio[ring->last].data, val, len);
	smp_wmb();
	ring->last = (ring->last + 1) % KVM_COALESCED_MMIO_MAX;
}

static void coalesced_mmio_destructor(struct kvm_io_device *this)
{
	kfree(this);
}

int kvm_coalesced_mmio_init(struct kvm *kvm)
{
	struct kvm_coalesced_mmio_dev *dev;

	dev = kzalloc(sizeof(struct kvm_coalesced_mmio_dev), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;
	dev->dev.write  = coalesced_mmio_write;
	dev->dev.in_range  = coalesced_mmio_in_range;
	dev->dev.destructor  = coalesced_mmio_destructor;
	dev->dev.private  = dev;
	dev->kvm = kvm;
	kvm->coalesced_mmio_dev = dev;
	kvm_io_bus_register_dev(&kvm->mmio_bus, &dev->dev);

	return 0;
}

int kvm_vm_ioctl_register_coalesced_mmio(struct kvm *kvm,
				         struct kvm_coalesced_mmio_zone *zone)
{
	struct kvm_coalesced_mmio_dev *dev = kvm->coalesced_mmio_dev;

	if (dev == NULL)
		return -EINVAL;

	mutex_lock(&kvm->lock);
	if (dev->nb_zones >= KVM_COALESCED_MMIO_ZONE_MAX) {
		mutex_unlock(&kvm->lock);
		return -ENOBUFS;
	}

	dev->zone[dev->nb_zones] = *zone;
	dev->nb_zones++;

	mutex_unlock(&kvm->lock);
	return 0;
}

int kvm_vm_ioctl_unregister_coalesced_mmio(struct kvm *kvm,
					   struct kvm_coalesced_mmio_zone *zone)
{
	int i;
	struct kvm_coalesced_mmio_dev *dev = kvm->coalesced_mmio_dev;
	struct kvm_coalesced_mmio_zone *z;

	if (dev == NULL)
		return -EINVAL;

	mutex_lock(&kvm->lock);

	i = dev->nb_zones;
	while(i) {
		z = &dev->zone[i - 1];

		/* unregister all zones
		 * included in (zone->addr, zone->size)
		 */

		if (zone->addr <= z->addr &&
		    z->addr + z->size <= zone->addr + zone->size) {
			dev->nb_zones--;
			*z = dev->zone[dev->nb_zones];
		}
		i--;
	}

	mutex_unlock(&kvm->lock);

	return 0;
}
