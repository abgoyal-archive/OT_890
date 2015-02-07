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

/* AFS network device helpers
 *
 * Copyright (c) 2007 Patrick McHardy <kaber@trash.net>
 */

#include <linux/string.h>
#include <linux/rtnetlink.h>
#include <linux/inetdevice.h>
#include <linux/netdevice.h>
#include <linux/if_arp.h>
#include <net/net_namespace.h>
#include "internal.h"

/*
 * get a MAC address from a random ethernet interface that has a real one
 * - the buffer will normally be 6 bytes in size
 */
int afs_get_MAC_address(u8 *mac, size_t maclen)
{
	struct net_device *dev;
	int ret = -ENODEV;

	if (maclen != ETH_ALEN)
		BUG();

	rtnl_lock();
	dev = __dev_getfirstbyhwtype(&init_net, ARPHRD_ETHER);
	if (dev) {
		memcpy(mac, dev->dev_addr, maclen);
		ret = 0;
	}
	rtnl_unlock();
	return ret;
}

/*
 * get a list of this system's interface IPv4 addresses, netmasks and MTUs
 * - maxbufs must be at least 1
 * - returns the number of interface records in the buffer
 */
int afs_get_ipv4_interfaces(struct afs_interface *bufs, size_t maxbufs,
			    bool wantloopback)
{
	struct net_device *dev;
	struct in_device *idev;
	int n = 0;

	ASSERT(maxbufs > 0);

	rtnl_lock();
	for_each_netdev(&init_net, dev) {
		if (dev->type == ARPHRD_LOOPBACK && !wantloopback)
			continue;
		idev = __in_dev_get_rtnl(dev);
		if (!idev)
			continue;
		for_primary_ifa(idev) {
			bufs[n].address.s_addr = ifa->ifa_address;
			bufs[n].netmask.s_addr = ifa->ifa_mask;
			bufs[n].mtu = dev->mtu;
			n++;
			if (n >= maxbufs)
				goto out;
		} endfor_ifa(idev);
	}
out:
	rtnl_unlock();
	return n;
}
