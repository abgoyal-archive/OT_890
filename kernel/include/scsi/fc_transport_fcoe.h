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

#ifndef FC_TRANSPORT_FCOE_H
#define FC_TRANSPORT_FCOE_H

#include <linux/device.h>
#include <linux/netdevice.h>
#include <scsi/scsi_host.h>
#include <scsi/libfc.h>

/**
 * struct fcoe_transport - FCoE transport struct for generic transport
 * for Ethernet devices as well as pure HBAs
 *
 * @name: name for thsi transport
 * @bus: physical bus type (pci_bus_type)
 * @driver: physical bus driver for network device
 * @create: entry create function
 * @destroy: exit destroy function
 * @list: list of transports
 */
struct fcoe_transport {
	char *name;
	unsigned short vendor;
	unsigned short device;
	struct bus_type *bus;
	struct device_driver *driver;
	int (*create)(struct net_device *device);
	int (*destroy)(struct net_device *device);
	bool (*match)(struct net_device *device);
	struct list_head list;
	struct list_head devlist;
	struct mutex devlock;
};

/**
 * MODULE_ALIAS_FCOE_PCI
 *
 * some care must be taken with this, vendor and device MUST be a hex value
 * preceded with 0x and with letters in lower case (0x12ab, not 0x12AB or 12AB)
 */
#define MODULE_ALIAS_FCOE_PCI(vendor, device) \
	MODULE_ALIAS("fcoe-pci-" __stringify(vendor) "-" __stringify(device))

/* exported funcs */
int fcoe_transport_attach(struct net_device *netdev);
int fcoe_transport_release(struct net_device *netdev);
int fcoe_transport_register(struct fcoe_transport *t);
int fcoe_transport_unregister(struct fcoe_transport *t);
int fcoe_load_transport_driver(struct net_device *netdev);
int __init fcoe_transport_init(void);
int __exit fcoe_transport_exit(void);

/* fcow_sw is the default transport */
extern struct fcoe_transport fcoe_sw_transport;
#endif /* FC_TRANSPORT_FCOE_H */
