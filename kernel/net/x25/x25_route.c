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
 *	X.25 Packet Layer release 002
 *
 *	This is ALPHA test software. This code may break your machine,
 *	randomly fail to work with new releases, misbehave and/or generally
 *	screw up. It might even work.
 *
 *	This code REQUIRES 2.1.15 or higher
 *
 *	This module:
 *		This module is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 *	History
 *	X.25 001	Jonathan Naylor	Started coding.
 */

#include <linux/if_arp.h>
#include <linux/init.h>
#include <net/x25.h>

LIST_HEAD(x25_route_list);
DEFINE_RWLOCK(x25_route_list_lock);

/*
 *	Add a new route.
 */
static int x25_add_route(struct x25_address *address, unsigned int sigdigits,
			 struct net_device *dev)
{
	struct x25_route *rt;
	struct list_head *entry;
	int rc = -EINVAL;

	write_lock_bh(&x25_route_list_lock);

	list_for_each(entry, &x25_route_list) {
		rt = list_entry(entry, struct x25_route, node);

		if (!memcmp(&rt->address, address, sigdigits) &&
		    rt->sigdigits == sigdigits)
			goto out;
	}

	rt = kmalloc(sizeof(*rt), GFP_ATOMIC);
	rc = -ENOMEM;
	if (!rt)
		goto out;

	strcpy(rt->address.x25_addr, "000000000000000");
	memcpy(rt->address.x25_addr, address->x25_addr, sigdigits);

	rt->sigdigits = sigdigits;
	rt->dev       = dev;
	atomic_set(&rt->refcnt, 1);

	list_add(&rt->node, &x25_route_list);
	rc = 0;
out:
	write_unlock_bh(&x25_route_list_lock);
	return rc;
}

/**
 * __x25_remove_route - remove route from x25_route_list
 * @rt - route to remove
 *
 * Remove route from x25_route_list. If it was there.
 * Caller must hold x25_route_list_lock.
 */
static void __x25_remove_route(struct x25_route *rt)
{
	if (rt->node.next) {
		list_del(&rt->node);
		x25_route_put(rt);
	}
}

static int x25_del_route(struct x25_address *address, unsigned int sigdigits,
			 struct net_device *dev)
{
	struct x25_route *rt;
	struct list_head *entry;
	int rc = -EINVAL;

	write_lock_bh(&x25_route_list_lock);

	list_for_each(entry, &x25_route_list) {
		rt = list_entry(entry, struct x25_route, node);

		if (!memcmp(&rt->address, address, sigdigits) &&
		    rt->sigdigits == sigdigits && rt->dev == dev) {
			__x25_remove_route(rt);
			rc = 0;
			break;
		}
	}

	write_unlock_bh(&x25_route_list_lock);
	return rc;
}

/*
 *	A device has been removed, remove its routes.
 */
void x25_route_device_down(struct net_device *dev)
{
	struct x25_route *rt;
	struct list_head *entry, *tmp;

	write_lock_bh(&x25_route_list_lock);

	list_for_each_safe(entry, tmp, &x25_route_list) {
		rt = list_entry(entry, struct x25_route, node);

		if (rt->dev == dev)
			__x25_remove_route(rt);
	}
	write_unlock_bh(&x25_route_list_lock);

	/* Remove any related forwarding */
	x25_clear_forward_by_dev(dev);
}

/*
 *	Check that the device given is a valid X.25 interface that is "up".
 */
struct net_device *x25_dev_get(char *devname)
{
	struct net_device *dev = dev_get_by_name(&init_net, devname);

	if (dev &&
	    (!(dev->flags & IFF_UP) || (dev->type != ARPHRD_X25
#if defined(CONFIG_LLC) || defined(CONFIG_LLC_MODULE)
					&& dev->type != ARPHRD_ETHER
#endif
					)))
		dev_put(dev);

	return dev;
}

/**
 * 	x25_get_route -	Find a route given an X.25 address.
 * 	@addr - address to find a route for
 *
 * 	Find a route given an X.25 address.
 */
struct x25_route *x25_get_route(struct x25_address *addr)
{
	struct x25_route *rt, *use = NULL;
	struct list_head *entry;

	read_lock_bh(&x25_route_list_lock);

	list_for_each(entry, &x25_route_list) {
		rt = list_entry(entry, struct x25_route, node);

		if (!memcmp(&rt->address, addr, rt->sigdigits)) {
			if (!use)
				use = rt;
			else if (rt->sigdigits > use->sigdigits)
				use = rt;
		}
	}

	if (use)
		x25_route_hold(use);

	read_unlock_bh(&x25_route_list_lock);
	return use;
}

/*
 *	Handle the ioctls that control the routing functions.
 */
int x25_route_ioctl(unsigned int cmd, void __user *arg)
{
	struct x25_route_struct rt;
	struct net_device *dev;
	int rc = -EINVAL;

	if (cmd != SIOCADDRT && cmd != SIOCDELRT)
		goto out;

	rc = -EFAULT;
	if (copy_from_user(&rt, arg, sizeof(rt)))
		goto out;

	rc = -EINVAL;
	if (rt.sigdigits < 0 || rt.sigdigits > 15)
		goto out;

	dev = x25_dev_get(rt.device);
	if (!dev)
		goto out;

	if (cmd == SIOCADDRT)
		rc = x25_add_route(&rt.address, rt.sigdigits, dev);
	else
		rc = x25_del_route(&rt.address, rt.sigdigits, dev);
	dev_put(dev);
out:
	return rc;
}

/*
 *	Release all memory associated with X.25 routing structures.
 */
void __exit x25_route_free(void)
{
	struct x25_route *rt;
	struct list_head *entry, *tmp;

	write_lock_bh(&x25_route_list_lock);
	list_for_each_safe(entry, tmp, &x25_route_list) {
		rt = list_entry(entry, struct x25_route, node);
		__x25_remove_route(rt);
	}
	write_unlock_bh(&x25_route_list_lock);
}
