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
 * kvm_virtio.h - definition for virtio for kvm on s390
 *
 * Copyright IBM Corp. 2008
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (version 2 only)
 * as published by the Free Software Foundation.
 *
 *    Author(s): Christian Borntraeger <borntraeger@de.ibm.com>
 */

#ifndef __KVM_S390_VIRTIO_H
#define __KVM_S390_VIRTIO_H

#include <linux/types.h>

struct kvm_device_desc {
	/* The device type: console, network, disk etc.  Type 0 terminates. */
	__u8 type;
	/* The number of virtqueues (first in config array) */
	__u8 num_vq;
	/*
	 * The number of bytes of feature bits.  Multiply by 2: one for host
	 * features and one for guest acknowledgements.
	 */
	__u8 feature_len;
	/* The number of bytes of the config array after virtqueues. */
	__u8 config_len;
	/* A status byte, written by the Guest. */
	__u8 status;
	__u8 config[0];
};

/*
 * This is how we expect the device configuration field for a virtqueue
 * to be laid out in config space.
 */
struct kvm_vqconfig {
	/* The token returned with an interrupt. Set by the guest */
	__u64 token;
	/* The address of the virtio ring */
	__u64 address;
	/* The number of entries in the virtio_ring */
	__u16 num;

};

#define KVM_S390_VIRTIO_NOTIFY		0
#define KVM_S390_VIRTIO_RESET		1
#define KVM_S390_VIRTIO_SET_STATUS	2

/* The alignment to use between consumer and producer parts of vring.
 * This is pagesize for historical reasons. */
#define KVM_S390_VIRTIO_RING_ALIGN	4096

#ifdef __KERNEL__
/* early virtio console setup */
#ifdef CONFIG_S390_GUEST
extern void s390_virtio_console_init(void);
#else
static inline void s390_virtio_console_init(void)
{
}
#endif /* CONFIG_VIRTIO_CONSOLE */
#endif /* __KERNEL__ */
#endif
