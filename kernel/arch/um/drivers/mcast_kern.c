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
 * user-mode-linux networking multicast transport
 * Copyright (C) 2001 by Harald Welte <laforge@gnumonks.org>
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 *
 * based on the existing uml-networking code, which is
 * Copyright (C) 2001 Lennert Buytenhek (buytenh@gnu.org) and
 * James Leu (jleu@mindspring.net).
 * Copyright (C) 2001 by various other people who didn't put their name here.
 *
 * Licensed under the GPL.
 */

#include "linux/init.h"
#include <linux/netdevice.h>
#include "mcast.h"
#include "net_kern.h"

struct mcast_init {
	char *addr;
	int port;
	int ttl;
};

static void mcast_init(struct net_device *dev, void *data)
{
	struct uml_net_private *pri;
	struct mcast_data *dpri;
	struct mcast_init *init = data;

	pri = netdev_priv(dev);
	dpri = (struct mcast_data *) pri->user;
	dpri->addr = init->addr;
	dpri->port = init->port;
	dpri->ttl = init->ttl;
	dpri->dev = dev;

	printk("mcast backend multicast address: %s:%u, TTL:%u\n",
	       dpri->addr, dpri->port, dpri->ttl);
}

static int mcast_read(int fd, struct sk_buff *skb, struct uml_net_private *lp)
{
	return net_recvfrom(fd, skb_mac_header(skb),
			    skb->dev->mtu + ETH_HEADER_OTHER);
}

static int mcast_write(int fd, struct sk_buff *skb, struct uml_net_private *lp)
{
	return mcast_user_write(fd, skb->data, skb->len,
				(struct mcast_data *) &lp->user);
}

static const struct net_kern_info mcast_kern_info = {
	.init			= mcast_init,
	.protocol		= eth_protocol,
	.read			= mcast_read,
	.write			= mcast_write,
};

static int mcast_setup(char *str, char **mac_out, void *data)
{
	struct mcast_init *init = data;
	char *port_str = NULL, *ttl_str = NULL, *remain;
	char *last;

	*init = ((struct mcast_init)
		{ .addr 	= "239.192.168.1",
		  .port 	= 1102,
		  .ttl 		= 1 });

	remain = split_if_spec(str, mac_out, &init->addr, &port_str, &ttl_str,
			       NULL);
	if (remain != NULL) {
		printk(KERN_ERR "mcast_setup - Extra garbage on "
		       "specification : '%s'\n", remain);
		return 0;
	}

	if (port_str != NULL) {
		init->port = simple_strtoul(port_str, &last, 10);
		if ((*last != '\0') || (last == port_str)) {
			printk(KERN_ERR "mcast_setup - Bad port : '%s'\n",
			       port_str);
			return 0;
		}
	}

	if (ttl_str != NULL) {
		init->ttl = simple_strtoul(ttl_str, &last, 10);
		if ((*last != '\0') || (last == ttl_str)) {
			printk(KERN_ERR "mcast_setup - Bad ttl : '%s'\n",
			       ttl_str);
			return 0;
		}
	}

	printk(KERN_INFO "Configured mcast device: %s:%u-%u\n", init->addr,
	       init->port, init->ttl);

	return 1;
}

static struct transport mcast_transport = {
	.list 		= LIST_HEAD_INIT(mcast_transport.list),
	.name 		= "mcast",
	.setup  	= mcast_setup,
	.user 		= &mcast_user_info,
	.kern 		= &mcast_kern_info,
	.private_size 	= sizeof(struct mcast_data),
	.setup_size 	= sizeof(struct mcast_init),
};

static int register_mcast(void)
{
	register_transport(&mcast_transport);
	return 0;
}

late_initcall(register_mcast);
