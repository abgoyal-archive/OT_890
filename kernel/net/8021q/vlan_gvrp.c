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
 * 	IEEE 802.1Q GARP VLAN Registration Protocol (GVRP)
 *
 * 	Copyright (c) 2008 Patrick McHardy <kaber@trash.net>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	version 2 as published by the Free Software Foundation.
 */
#include <linux/types.h>
#include <linux/if_vlan.h>
#include <net/garp.h>
#include "vlan.h"

#define GARP_GVRP_ADDRESS	{ 0x01, 0x80, 0xc2, 0x00, 0x00, 0x21 }

enum gvrp_attributes {
	GVRP_ATTR_INVALID,
	GVRP_ATTR_VID,
	__GVRP_ATTR_MAX
};
#define GVRP_ATTR_MAX	(__GVRP_ATTR_MAX - 1)

static struct garp_application vlan_gvrp_app __read_mostly = {
	.proto.group_address	= GARP_GVRP_ADDRESS,
	.maxattr		= GVRP_ATTR_MAX,
	.type			= GARP_APPLICATION_GVRP,
};

int vlan_gvrp_request_join(const struct net_device *dev)
{
	const struct vlan_dev_info *vlan = vlan_dev_info(dev);
	__be16 vlan_id = htons(vlan->vlan_id);

	return garp_request_join(vlan->real_dev, &vlan_gvrp_app,
				 &vlan_id, sizeof(vlan_id), GVRP_ATTR_VID);
}

void vlan_gvrp_request_leave(const struct net_device *dev)
{
	const struct vlan_dev_info *vlan = vlan_dev_info(dev);
	__be16 vlan_id = htons(vlan->vlan_id);

	garp_request_leave(vlan->real_dev, &vlan_gvrp_app,
			   &vlan_id, sizeof(vlan_id), GVRP_ATTR_VID);
}

int vlan_gvrp_init_applicant(struct net_device *dev)
{
	return garp_init_applicant(dev, &vlan_gvrp_app);
}

void vlan_gvrp_uninit_applicant(struct net_device *dev)
{
	garp_uninit_applicant(dev, &vlan_gvrp_app);
}

int __init vlan_gvrp_init(void)
{
	return garp_register_application(&vlan_gvrp_app);
}

void vlan_gvrp_uninit(void)
{
	garp_unregister_application(&vlan_gvrp_app);
}
