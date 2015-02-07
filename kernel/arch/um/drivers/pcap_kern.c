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
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL.
 */

#include "linux/init.h"
#include <linux/netdevice.h>
#include "net_kern.h"
#include "pcap_user.h"

struct pcap_init {
	char *host_if;
	int promisc;
	int optimize;
	char *filter;
};

void pcap_init(struct net_device *dev, void *data)
{
	struct uml_net_private *pri;
	struct pcap_data *ppri;
	struct pcap_init *init = data;

	pri = netdev_priv(dev);
	ppri = (struct pcap_data *) pri->user;
	ppri->host_if = init->host_if;
	ppri->promisc = init->promisc;
	ppri->optimize = init->optimize;
	ppri->filter = init->filter;

	printk("pcap backend, host interface %s\n", ppri->host_if);
}

static int pcap_read(int fd, struct sk_buff *skb, struct uml_net_private *lp)
{
	return pcap_user_read(fd, skb_mac_header(skb),
			      skb->dev->mtu + ETH_HEADER_OTHER,
			      (struct pcap_data *) &lp->user);
}

static int pcap_write(int fd, struct sk_buff *skb, struct uml_net_private *lp)
{
	return -EPERM;
}

static const struct net_kern_info pcap_kern_info = {
	.init			= pcap_init,
	.protocol		= eth_protocol,
	.read			= pcap_read,
	.write			= pcap_write,
};

int pcap_setup(char *str, char **mac_out, void *data)
{
	struct pcap_init *init = data;
	char *remain, *host_if = NULL, *options[2] = { NULL, NULL };
	int i;

	*init = ((struct pcap_init)
		{ .host_if 	= "eth0",
		  .promisc 	= 1,
		  .optimize 	= 0,
		  .filter 	= NULL });

	remain = split_if_spec(str, &host_if, &init->filter,
			       &options[0], &options[1], mac_out, NULL);
	if (remain != NULL) {
		printk(KERN_ERR "pcap_setup - Extra garbage on "
		       "specification : '%s'\n", remain);
		return 0;
	}

	if (host_if != NULL)
		init->host_if = host_if;

	for (i = 0; i < ARRAY_SIZE(options); i++) {
		if (options[i] == NULL)
			continue;
		if (!strcmp(options[i], "promisc"))
			init->promisc = 1;
		else if (!strcmp(options[i], "nopromisc"))
			init->promisc = 0;
		else if (!strcmp(options[i], "optimize"))
			init->optimize = 1;
		else if (!strcmp(options[i], "nooptimize"))
			init->optimize = 0;
		else {
			printk(KERN_ERR "pcap_setup : bad option - '%s'\n",
			       options[i]);
			return 0;
		}
	}

	return 1;
}

static struct transport pcap_transport = {
	.list 		= LIST_HEAD_INIT(pcap_transport.list),
	.name 		= "pcap",
	.setup  	= pcap_setup,
	.user 		= &pcap_user_info,
	.kern 		= &pcap_kern_info,
	.private_size 	= sizeof(struct pcap_data),
	.setup_size 	= sizeof(struct pcap_init),
};

static int register_pcap(void)
{
	register_transport(&pcap_transport);
	return 0;
}

late_initcall(register_pcap);
