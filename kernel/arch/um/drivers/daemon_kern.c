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
 * Copyright (C) 2001 Lennert Buytenhek (buytenh@gnu.org) and
 * James Leu (jleu@mindspring.net).
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Copyright (C) 2001 by various other people who didn't put their name here.
 * Licensed under the GPL.
 */

#include "linux/init.h"
#include <linux/netdevice.h>
#include "net_kern.h"
#include "daemon.h"

struct daemon_init {
	char *sock_type;
	char *ctl_sock;
};

static void daemon_init(struct net_device *dev, void *data)
{
	struct uml_net_private *pri;
	struct daemon_data *dpri;
	struct daemon_init *init = data;

	pri = netdev_priv(dev);
	dpri = (struct daemon_data *) pri->user;
	dpri->sock_type = init->sock_type;
	dpri->ctl_sock = init->ctl_sock;
	dpri->fd = -1;
	dpri->control = -1;
	dpri->dev = dev;
	/* We will free this pointer. If it contains crap we're burned. */
	dpri->ctl_addr = NULL;
	dpri->data_addr = NULL;
	dpri->local_addr = NULL;

	printk("daemon backend (uml_switch version %d) - %s:%s",
	       SWITCH_VERSION, dpri->sock_type, dpri->ctl_sock);
	printk("\n");
}

static int daemon_read(int fd, struct sk_buff *skb, struct uml_net_private *lp)
{
	return net_recvfrom(fd, skb_mac_header(skb),
			    skb->dev->mtu + ETH_HEADER_OTHER);
}

static int daemon_write(int fd, struct sk_buff *skb, struct uml_net_private *lp)
{
	return daemon_user_write(fd, skb->data, skb->len,
				 (struct daemon_data *) &lp->user);
}

static const struct net_kern_info daemon_kern_info = {
	.init			= daemon_init,
	.protocol		= eth_protocol,
	.read			= daemon_read,
	.write			= daemon_write,
};

static int daemon_setup(char *str, char **mac_out, void *data)
{
	struct daemon_init *init = data;
	char *remain;

	*init = ((struct daemon_init)
		{ .sock_type 		= "unix",
		  .ctl_sock 		= "/tmp/uml.ctl" });

	remain = split_if_spec(str, mac_out, &init->sock_type, &init->ctl_sock,
			       NULL);
	if (remain != NULL)
		printk(KERN_WARNING "daemon_setup : Ignoring data socket "
		       "specification\n");

	return 1;
}

static struct transport daemon_transport = {
	.list 		= LIST_HEAD_INIT(daemon_transport.list),
	.name 		= "daemon",
	.setup  	= daemon_setup,
	.user 		= &daemon_user_info,
	.kern 		= &daemon_kern_info,
	.private_size 	= sizeof(struct daemon_data),
	.setup_size 	= sizeof(struct daemon_init),
};

static int register_daemon(void)
{
	register_transport(&daemon_transport);
	return 0;
}

late_initcall(register_daemon);
