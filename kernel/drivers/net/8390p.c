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

/* 8390 core for ISA devices needing bus delays */

static const char version[] =
    "8390p.c:v1.10cvs 9/23/94 Donald Becker (becker@cesdis.gsfc.nasa.gov)\n";

#define ei_inb(_p)	inb(_p)
#define ei_outb(_v, _p)	outb(_v, _p)
#define ei_inb_p(_p)	inb_p(_p)
#define ei_outb_p(_v, _p) outb_p(_v, _p)

#include "lib8390.c"

int eip_open(struct net_device *dev)
{
	return __ei_open(dev);
}
EXPORT_SYMBOL(eip_open);

int eip_close(struct net_device *dev)
{
	return __ei_close(dev);
}
EXPORT_SYMBOL(eip_close);

int eip_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	return __ei_start_xmit(skb, dev);
}
EXPORT_SYMBOL(eip_start_xmit);

struct net_device_stats *eip_get_stats(struct net_device *dev)
{
	return __ei_get_stats(dev);
}
EXPORT_SYMBOL(eip_get_stats);

void eip_set_multicast_list(struct net_device *dev)
{
	__ei_set_multicast_list(dev);
}
EXPORT_SYMBOL(eip_set_multicast_list);

void eip_tx_timeout(struct net_device *dev)
{
	__ei_tx_timeout(dev);
}
EXPORT_SYMBOL(eip_tx_timeout);

irqreturn_t eip_interrupt(int irq, void *dev_id)
{
	return __ei_interrupt(irq, dev_id);
}
EXPORT_SYMBOL(eip_interrupt);

#ifdef CONFIG_NET_POLL_CONTROLLER
void eip_poll(struct net_device *dev)
{
	__ei_poll(dev);
}
EXPORT_SYMBOL(eip_poll);
#endif

const struct net_device_ops eip_netdev_ops = {
	.ndo_open		= eip_open,
	.ndo_stop		= eip_close,
	.ndo_start_xmit		= eip_start_xmit,
	.ndo_tx_timeout		= eip_tx_timeout,
	.ndo_get_stats		= eip_get_stats,
	.ndo_set_multicast_list = eip_set_multicast_list,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_set_mac_address 	= eth_mac_addr,
	.ndo_change_mtu		= eth_change_mtu,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= eip_poll,
#endif
};
EXPORT_SYMBOL(eip_netdev_ops);

struct net_device *__alloc_eip_netdev(int size)
{
	struct net_device *dev = ____alloc_ei_netdev(size);
#ifdef CONFIG_COMPAT_NET_DEV_OPS
	if (dev) {
		dev->hard_start_xmit = eip_start_xmit;
		dev->get_stats	= eip_get_stats;
		dev->set_multicast_list = eip_set_multicast_list;
		dev->tx_timeout = eip_tx_timeout;
	}
#endif
	return dev;
}
EXPORT_SYMBOL(__alloc_eip_netdev);

void NS8390p_init(struct net_device *dev, int startp)
{
	__NS8390_init(dev, startp);
}
EXPORT_SYMBOL(NS8390p_init);

#if defined(MODULE)

int init_module(void)
{
	return 0;
}

void cleanup_module(void)
{
}

#endif /* MODULE */
MODULE_LICENSE("GPL");
