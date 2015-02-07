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
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include <linux/netdevice.h>
#include <linux/init.h>
#include <linux/skbuff.h>
#include <asm/errno.h>
#include "net_kern.h"
#include "tuntap.h"

struct tuntap_init {
	char *dev_name;
	char *gate_addr;
};

static void tuntap_init(struct net_device *dev, void *data)
{
	struct uml_net_private *pri;
	struct tuntap_data *tpri;
	struct tuntap_init *init = data;

	pri = netdev_priv(dev);
	tpri = (struct tuntap_data *) pri->user;
	tpri->dev_name = init->dev_name;
	tpri->fixed_config = (init->dev_name != NULL);
	tpri->gate_addr = init->gate_addr;
	tpri->fd = -1;
	tpri->dev = dev;

	printk(KERN_INFO "TUN/TAP backend - ");
	if (tpri->gate_addr != NULL)
		printk(KERN_CONT "IP = %s", tpri->gate_addr);
	printk(KERN_CONT "\n");
}

static int tuntap_read(int fd, struct sk_buff *skb, struct uml_net_private *lp)
{
	return net_read(fd, skb_mac_header(skb),
			skb->dev->mtu + ETH_HEADER_OTHER);
}

static int tuntap_write(int fd, struct sk_buff *skb, struct uml_net_private *lp)
{
	return net_write(fd, skb->data, skb->len);
}

const struct net_kern_info tuntap_kern_info = {
	.init			= tuntap_init,
	.protocol		= eth_protocol,
	.read			= tuntap_read,
	.write 			= tuntap_write,
};

int tuntap_setup(char *str, char **mac_out, void *data)
{
	struct tuntap_init *init = data;

	*init = ((struct tuntap_init)
		{ .dev_name 	= NULL,
		  .gate_addr 	= NULL });
	if (tap_setup_common(str, "tuntap", &init->dev_name, mac_out,
			    &init->gate_addr))
		return 0;

	return 1;
}

static struct transport tuntap_transport = {
	.list 		= LIST_HEAD_INIT(tuntap_transport.list),
	.name 		= "tuntap",
	.setup  	= tuntap_setup,
	.user 		= &tuntap_user_info,
	.kern 		= &tuntap_kern_info,
	.private_size 	= sizeof(struct tuntap_data),
	.setup_size 	= sizeof(struct tuntap_init),
};

static int register_tuntap(void)
{
	register_transport(&tuntap_transport);
	return 0;
}

late_initcall(register_tuntap);
