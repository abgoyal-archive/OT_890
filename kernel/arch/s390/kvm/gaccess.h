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
 * gaccess.h -  access guest memory
 *
 * Copyright IBM Corp. 2008
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (version 2 only)
 * as published by the Free Software Foundation.
 *
 *    Author(s): Carsten Otte <cotte@de.ibm.com>
 */

#ifndef __KVM_S390_GACCESS_H
#define __KVM_S390_GACCESS_H

#include <linux/compiler.h>
#include <linux/kvm_host.h>
#include <asm/uaccess.h>

static inline void __user *__guestaddr_to_user(struct kvm_vcpu *vcpu,
					       unsigned long guestaddr)
{
	unsigned long prefix  = vcpu->arch.sie_block->prefix;
	unsigned long origin  = vcpu->kvm->arch.guest_origin;
	unsigned long memsize = vcpu->kvm->arch.guest_memsize;

	if (guestaddr < 2 * PAGE_SIZE)
		guestaddr += prefix;
	else if ((guestaddr >= prefix) && (guestaddr < prefix + 2 * PAGE_SIZE))
		guestaddr -= prefix;

	if (guestaddr > memsize)
		return (void __user __force *) ERR_PTR(-EFAULT);

	guestaddr += origin;

	return (void __user *) guestaddr;
}

static inline int get_guest_u64(struct kvm_vcpu *vcpu, unsigned long guestaddr,
				u64 *result)
{
	void __user *uptr = __guestaddr_to_user(vcpu, guestaddr);

	BUG_ON(guestaddr & 7);

	if (IS_ERR((void __force *) uptr))
		return PTR_ERR((void __force *) uptr);

	return get_user(*result, (unsigned long __user *) uptr);
}

static inline int get_guest_u32(struct kvm_vcpu *vcpu, unsigned long guestaddr,
				u32 *result)
{
	void __user *uptr = __guestaddr_to_user(vcpu, guestaddr);

	BUG_ON(guestaddr & 3);

	if (IS_ERR((void __force *) uptr))
		return PTR_ERR((void __force *) uptr);

	return get_user(*result, (u32 __user *) uptr);
}

static inline int get_guest_u16(struct kvm_vcpu *vcpu, unsigned long guestaddr,
				u16 *result)
{
	void __user *uptr = __guestaddr_to_user(vcpu, guestaddr);

	BUG_ON(guestaddr & 1);

	if (IS_ERR(uptr))
		return PTR_ERR(uptr);

	return get_user(*result, (u16 __user *) uptr);
}

static inline int get_guest_u8(struct kvm_vcpu *vcpu, unsigned long guestaddr,
			       u8 *result)
{
	void __user *uptr = __guestaddr_to_user(vcpu, guestaddr);

	if (IS_ERR((void __force *) uptr))
		return PTR_ERR((void __force *) uptr);

	return get_user(*result, (u8 __user *) uptr);
}

static inline int put_guest_u64(struct kvm_vcpu *vcpu, unsigned long guestaddr,
				u64 value)
{
	void __user *uptr = __guestaddr_to_user(vcpu, guestaddr);

	BUG_ON(guestaddr & 7);

	if (IS_ERR((void __force *) uptr))
		return PTR_ERR((void __force *) uptr);

	return put_user(value, (u64 __user *) uptr);
}

static inline int put_guest_u32(struct kvm_vcpu *vcpu, unsigned long guestaddr,
				u32 value)
{
	void __user *uptr = __guestaddr_to_user(vcpu, guestaddr);

	BUG_ON(guestaddr & 3);

	if (IS_ERR((void __force *) uptr))
		return PTR_ERR((void __force *) uptr);

	return put_user(value, (u32 __user *) uptr);
}

static inline int put_guest_u16(struct kvm_vcpu *vcpu, unsigned long guestaddr,
				u16 value)
{
	void __user *uptr = __guestaddr_to_user(vcpu, guestaddr);

	BUG_ON(guestaddr & 1);

	if (IS_ERR((void __force *) uptr))
		return PTR_ERR((void __force *) uptr);

	return put_user(value, (u16 __user *) uptr);
}

static inline int put_guest_u8(struct kvm_vcpu *vcpu, unsigned long guestaddr,
			       u8 value)
{
	void __user *uptr = __guestaddr_to_user(vcpu, guestaddr);

	if (IS_ERR((void __force *) uptr))
		return PTR_ERR((void __force *) uptr);

	return put_user(value, (u8 __user *) uptr);
}


static inline int __copy_to_guest_slow(struct kvm_vcpu *vcpu,
				       unsigned long guestdest,
				       const void *from, unsigned long n)
{
	int rc;
	unsigned long i;
	const u8 *data = from;

	for (i = 0; i < n; i++) {
		rc = put_guest_u8(vcpu, guestdest++, *(data++));
		if (rc < 0)
			return rc;
	}
	return 0;
}

static inline int copy_to_guest(struct kvm_vcpu *vcpu, unsigned long guestdest,
				const void *from, unsigned long n)
{
	unsigned long prefix  = vcpu->arch.sie_block->prefix;
	unsigned long origin  = vcpu->kvm->arch.guest_origin;
	unsigned long memsize = vcpu->kvm->arch.guest_memsize;

	if ((guestdest < 2 * PAGE_SIZE) && (guestdest + n > 2 * PAGE_SIZE))
		goto slowpath;

	if ((guestdest < prefix) && (guestdest + n > prefix))
		goto slowpath;

	if ((guestdest < prefix + 2 * PAGE_SIZE)
	    && (guestdest + n > prefix + 2 * PAGE_SIZE))
		goto slowpath;

	if (guestdest < 2 * PAGE_SIZE)
		guestdest += prefix;
	else if ((guestdest >= prefix) && (guestdest < prefix + 2 * PAGE_SIZE))
		guestdest -= prefix;

	if (guestdest + n > memsize)
		return -EFAULT;

	if (guestdest + n < guestdest)
		return -EFAULT;

	guestdest += origin;

	return copy_to_user((void __user *) guestdest, from, n);
slowpath:
	return __copy_to_guest_slow(vcpu, guestdest, from, n);
}

static inline int __copy_from_guest_slow(struct kvm_vcpu *vcpu, void *to,
					 unsigned long guestsrc,
					 unsigned long n)
{
	int rc;
	unsigned long i;
	u8 *data = to;

	for (i = 0; i < n; i++) {
		rc = get_guest_u8(vcpu, guestsrc++, data++);
		if (rc < 0)
			return rc;
	}
	return 0;
}

static inline int copy_from_guest(struct kvm_vcpu *vcpu, void *to,
				  unsigned long guestsrc, unsigned long n)
{
	unsigned long prefix  = vcpu->arch.sie_block->prefix;
	unsigned long origin  = vcpu->kvm->arch.guest_origin;
	unsigned long memsize = vcpu->kvm->arch.guest_memsize;

	if ((guestsrc < 2 * PAGE_SIZE) && (guestsrc + n > 2 * PAGE_SIZE))
		goto slowpath;

	if ((guestsrc < prefix) && (guestsrc + n > prefix))
		goto slowpath;

	if ((guestsrc < prefix + 2 * PAGE_SIZE)
	    && (guestsrc + n > prefix + 2 * PAGE_SIZE))
		goto slowpath;

	if (guestsrc < 2 * PAGE_SIZE)
		guestsrc += prefix;
	else if ((guestsrc >= prefix) && (guestsrc < prefix + 2 * PAGE_SIZE))
		guestsrc -= prefix;

	if (guestsrc + n > memsize)
		return -EFAULT;

	if (guestsrc + n < guestsrc)
		return -EFAULT;

	guestsrc += origin;

	return copy_from_user(to, (void __user *) guestsrc, n);
slowpath:
	return __copy_from_guest_slow(vcpu, to, guestsrc, n);
}

static inline int copy_to_guest_absolute(struct kvm_vcpu *vcpu,
					 unsigned long guestdest,
					 const void *from, unsigned long n)
{
	unsigned long origin  = vcpu->kvm->arch.guest_origin;
	unsigned long memsize = vcpu->kvm->arch.guest_memsize;

	if (guestdest + n > memsize)
		return -EFAULT;

	if (guestdest + n < guestdest)
		return -EFAULT;

	guestdest += origin;

	return copy_to_user((void __user *) guestdest, from, n);
}

static inline int copy_from_guest_absolute(struct kvm_vcpu *vcpu, void *to,
					   unsigned long guestsrc,
					   unsigned long n)
{
	unsigned long origin  = vcpu->kvm->arch.guest_origin;
	unsigned long memsize = vcpu->kvm->arch.guest_memsize;

	if (guestsrc + n > memsize)
		return -EFAULT;

	if (guestsrc + n < guestsrc)
		return -EFAULT;

	guestsrc += origin;

	return copy_from_user(to, (void __user *) guestsrc, n);
}
#endif
