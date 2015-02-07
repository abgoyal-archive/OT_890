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
 * Copyright (C) 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL.
 */

#include <linux/if_arp.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include "net_kern.h"
#include "slip.h"

struct slip_init {
	char *gate_addr;
};

static void slip_init(struct net_device *dev, void *data)
{
	struct uml_net_private *private;
	struct slip_data *spri;
	struct slip_init *init = data;

	private = netdev_priv(dev);
	spri = (struct slip_data *) private->user;

	memset(spri->name, 0, sizeof(spri->name));
	spri->addr = NULL;
	spri->gate_addr = init->gate_addr;
	spri->slave = -1;
	spri->dev = dev;

	slip_proto_init(&spri->slip);

	dev->init = NULL;
	dev->hard_header_len = 0;
	dev->header_ops = NULL;
	dev->addr_len = 0;
	dev->type = ARPHRD_SLIP;
	dev->tx_queue_len = 256;
	dev->flags = IFF_NOARP;
	printk("SLIP backend - SLIP IP = %s\n", spri->gate_addr);
}

static unsigned short slip_protocol(struct sk_buff *skbuff)
{
	return htons(ETH_P_IP);
}

static int slip_read(int fd, struct sk_buff *skb, struct uml_net_private *lp)
{
	return slip_user_read(fd, skb_mac_header(skb), skb->dev->mtu,
			      (struct slip_data *) &lp->user);
}

static int slip_write(int fd, struct sk_buff *skb, struct uml_net_private *lp)
{
	return slip_user_write(fd, skb->data, skb->len,
			       (struct slip_data *) &lp->user);
}

static const struct net_kern_info slip_kern_info = {
	.init			= slip_init,
	.protocol		= slip_protocol,
	.read			= slip_read,
	.write			= slip_write,
};

static int slip_setup(char *str, char **mac_out, void *data)
{
	struct slip_init *init = data;

	*init = ((struct slip_init) { .gate_addr = NULL });

	if (str[0] != '\0')
		init->gate_addr = str;
	return 1;
}

static struct transport slip_transport = {
	.list 		= LIST_HEAD_INIT(slip_transport.list),
	.name 		= "slip",
	.setup  	= slip_setup,
	.user 		= &slip_user_info,
	.kern 		= &slip_kern_info,
	.private_size 	= sizeof(struct slip_data),
	.setup_size 	= sizeof(struct slip_init),
};

static int register_slip(void)
{
	register_transport(&slip_transport);
	return 0;
}

late_initcall(register_slip);
