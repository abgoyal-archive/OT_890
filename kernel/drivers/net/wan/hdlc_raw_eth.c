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
 * Generic HDLC support routines for Linux
 * HDLC Ethernet emulation support
 *
 * Copyright (C) 2002-2006 Krzysztof Halasa <khc@pm.waw.pl>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 */

#include <linux/errno.h>
#include <linux/etherdevice.h>
#include <linux/hdlc.h>
#include <linux/if_arp.h>
#include <linux/inetdevice.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pkt_sched.h>
#include <linux/poll.h>
#include <linux/rtnetlink.h>
#include <linux/skbuff.h>
#include <linux/slab.h>

static int raw_eth_ioctl(struct net_device *dev, struct ifreq *ifr);

static int eth_tx(struct sk_buff *skb, struct net_device *dev)
{
	int pad = ETH_ZLEN - skb->len;
	if (pad > 0) {		/* Pad the frame with zeros */
		int len = skb->len;
		if (skb_tailroom(skb) < pad)
			if (pskb_expand_head(skb, 0, pad, GFP_ATOMIC)) {
				dev->stats.tx_dropped++;
				dev_kfree_skb(skb);
				return 0;
			}
		skb_put(skb, pad);
		memset(skb->data + len, 0, pad);
	}
	return dev_to_hdlc(dev)->xmit(skb, dev);
}


static struct hdlc_proto proto = {
	.type_trans	= eth_type_trans,
	.ioctl		= raw_eth_ioctl,
	.module		= THIS_MODULE,
};


static int raw_eth_ioctl(struct net_device *dev, struct ifreq *ifr)
{
	raw_hdlc_proto __user *raw_s = ifr->ifr_settings.ifs_ifsu.raw_hdlc;
	const size_t size = sizeof(raw_hdlc_proto);
	raw_hdlc_proto new_settings;
	hdlc_device *hdlc = dev_to_hdlc(dev);
	int result;
	int (*old_ch_mtu)(struct net_device *, int);
	int old_qlen;

	switch (ifr->ifr_settings.type) {
	case IF_GET_PROTO:
		if (dev_to_hdlc(dev)->proto != &proto)
			return -EINVAL;
		ifr->ifr_settings.type = IF_PROTO_HDLC_ETH;
		if (ifr->ifr_settings.size < size) {
			ifr->ifr_settings.size = size; /* data size wanted */
			return -ENOBUFS;
		}
		if (copy_to_user(raw_s, hdlc->state, size))
			return -EFAULT;
		return 0;

	case IF_PROTO_HDLC_ETH:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		if (dev->flags & IFF_UP)
			return -EBUSY;

		if (copy_from_user(&new_settings, raw_s, size))
			return -EFAULT;

		if (new_settings.encoding == ENCODING_DEFAULT)
			new_settings.encoding = ENCODING_NRZ;

		if (new_settings.parity == PARITY_DEFAULT)
			new_settings.parity = PARITY_CRC16_PR1_CCITT;

		result = hdlc->attach(dev, new_settings.encoding,
				      new_settings.parity);
		if (result)
			return result;

		result = attach_hdlc_protocol(dev, &proto,
					      sizeof(raw_hdlc_proto));
		if (result)
			return result;
		memcpy(hdlc->state, &new_settings, size);
		dev->hard_start_xmit = eth_tx;
		old_ch_mtu = dev->change_mtu;
		old_qlen = dev->tx_queue_len;
		ether_setup(dev);
		dev->change_mtu = old_ch_mtu;
		dev->tx_queue_len = old_qlen;
		random_ether_addr(dev->dev_addr);
		netif_dormant_off(dev);
		return 0;
	}

	return -EINVAL;
}


static int __init mod_init(void)
{
	register_hdlc_protocol(&proto);
	return 0;
}



static void __exit mod_exit(void)
{
	unregister_hdlc_protocol(&proto);
}


module_init(mod_init);
module_exit(mod_exit);

MODULE_AUTHOR("Krzysztof Halasa <khc@pm.waw.pl>");
MODULE_DESCRIPTION("Ethernet encapsulation support for generic HDLC");
MODULE_LICENSE("GPL v2");
