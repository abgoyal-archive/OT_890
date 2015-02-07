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
 * Virtio PCI driver
 *
 * This module allows virtio devices to be used over a virtual PCI device.
 * This can be used with QEMU based VMMs like KVM or Xen.
 *
 * Copyright IBM Corp. 2007
 *
 * Authors:
 *  Anthony Liguori  <aliguori@us.ibm.com>
 *
 * This header is BSD licensed so anyone can use the definitions to implement
 * compatible drivers/servers.
 */

#ifndef _LINUX_VIRTIO_PCI_H
#define _LINUX_VIRTIO_PCI_H

#include <linux/virtio_config.h>

/* A 32-bit r/o bitmask of the features supported by the host */
#define VIRTIO_PCI_HOST_FEATURES	0

/* A 32-bit r/w bitmask of features activated by the guest */
#define VIRTIO_PCI_GUEST_FEATURES	4

/* A 32-bit r/w PFN for the currently selected queue */
#define VIRTIO_PCI_QUEUE_PFN		8

/* A 16-bit r/o queue size for the currently selected queue */
#define VIRTIO_PCI_QUEUE_NUM		12

/* A 16-bit r/w queue selector */
#define VIRTIO_PCI_QUEUE_SEL		14

/* A 16-bit r/w queue notifier */
#define VIRTIO_PCI_QUEUE_NOTIFY		16

/* An 8-bit device status register.  */
#define VIRTIO_PCI_STATUS		18

/* An 8-bit r/o interrupt status register.  Reading the value will return the
 * current contents of the ISR and will also clear it.  This is effectively
 * a read-and-acknowledge. */
#define VIRTIO_PCI_ISR			19

/* The bit of the ISR which indicates a device configuration change. */
#define VIRTIO_PCI_ISR_CONFIG		0x2

/* The remaining space is defined by each driver as the per-driver
 * configuration space */
#define VIRTIO_PCI_CONFIG		20

/* Virtio ABI version, this must match exactly */
#define VIRTIO_PCI_ABI_VERSION		0

/* How many bits to shift physical queue address written to QUEUE_PFN.
 * 12 is historical, and due to x86 page size. */
#define VIRTIO_PCI_QUEUE_ADDR_SHIFT	12

/* The alignment to use between consumer and producer parts of vring.
 * x86 pagesize again. */
#define VIRTIO_PCI_VRING_ALIGN		4096
#endif
