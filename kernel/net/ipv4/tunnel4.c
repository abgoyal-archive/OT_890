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

/* tunnel4.c: Generic IP tunnel transformer.
 *
 * Copyright (C) 2003 David S. Miller (davem@redhat.com)
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <net/icmp.h>
#include <net/ip.h>
#include <net/protocol.h>
#include <net/xfrm.h>

static struct xfrm_tunnel *tunnel4_handlers;
static struct xfrm_tunnel *tunnel64_handlers;
static DEFINE_MUTEX(tunnel4_mutex);

static inline struct xfrm_tunnel **fam_handlers(unsigned short family)
{
	return (family == AF_INET) ? &tunnel4_handlers : &tunnel64_handlers;
}

int xfrm4_tunnel_register(struct xfrm_tunnel *handler, unsigned short family)
{
	struct xfrm_tunnel **pprev;
	int ret = -EEXIST;
	int priority = handler->priority;

	mutex_lock(&tunnel4_mutex);

	for (pprev = fam_handlers(family); *pprev; pprev = &(*pprev)->next) {
		if ((*pprev)->priority > priority)
			break;
		if ((*pprev)->priority == priority)
			goto err;
	}

	handler->next = *pprev;
	*pprev = handler;

	ret = 0;

err:
	mutex_unlock(&tunnel4_mutex);

	return ret;
}

EXPORT_SYMBOL(xfrm4_tunnel_register);

int xfrm4_tunnel_deregister(struct xfrm_tunnel *handler, unsigned short family)
{
	struct xfrm_tunnel **pprev;
	int ret = -ENOENT;

	mutex_lock(&tunnel4_mutex);

	for (pprev = fam_handlers(family); *pprev; pprev = &(*pprev)->next) {
		if (*pprev == handler) {
			*pprev = handler->next;
			ret = 0;
			break;
		}
	}

	mutex_unlock(&tunnel4_mutex);

	synchronize_net();

	return ret;
}

EXPORT_SYMBOL(xfrm4_tunnel_deregister);

static int tunnel4_rcv(struct sk_buff *skb)
{
	struct xfrm_tunnel *handler;

	if (!pskb_may_pull(skb, sizeof(struct iphdr)))
		goto drop;

	for (handler = tunnel4_handlers; handler; handler = handler->next)
		if (!handler->handler(skb))
			return 0;

	icmp_send(skb, ICMP_DEST_UNREACH, ICMP_PORT_UNREACH, 0);

drop:
	kfree_skb(skb);
	return 0;
}

#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
static int tunnel64_rcv(struct sk_buff *skb)
{
	struct xfrm_tunnel *handler;

	if (!pskb_may_pull(skb, sizeof(struct ipv6hdr)))
		goto drop;

	for (handler = tunnel64_handlers; handler; handler = handler->next)
		if (!handler->handler(skb))
			return 0;

	icmp_send(skb, ICMP_DEST_UNREACH, ICMP_PORT_UNREACH, 0);

drop:
	kfree_skb(skb);
	return 0;
}
#endif

static void tunnel4_err(struct sk_buff *skb, u32 info)
{
	struct xfrm_tunnel *handler;

	for (handler = tunnel4_handlers; handler; handler = handler->next)
		if (!handler->err_handler(skb, info))
			break;
}

#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
static void tunnel64_err(struct sk_buff *skb, u32 info)
{
	struct xfrm_tunnel *handler;

	for (handler = tunnel64_handlers; handler; handler = handler->next)
		if (!handler->err_handler(skb, info))
			break;
}
#endif

static struct net_protocol tunnel4_protocol = {
	.handler	=	tunnel4_rcv,
	.err_handler	=	tunnel4_err,
	.no_policy	=	1,
	.netns_ok	=	1,
};

#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
static struct net_protocol tunnel64_protocol = {
	.handler	=	tunnel64_rcv,
	.err_handler	=	tunnel64_err,
	.no_policy	=	1,
	.netns_ok	=	1,
};
#endif

static int __init tunnel4_init(void)
{
	if (inet_add_protocol(&tunnel4_protocol, IPPROTO_IPIP)) {
		printk(KERN_ERR "tunnel4 init: can't add protocol\n");
		return -EAGAIN;
	}
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
	if (inet_add_protocol(&tunnel64_protocol, IPPROTO_IPV6)) {
		printk(KERN_ERR "tunnel64 init: can't add protocol\n");
		inet_del_protocol(&tunnel4_protocol, IPPROTO_IPIP);
		return -EAGAIN;
	}
#endif
	return 0;
}

static void __exit tunnel4_fini(void)
{
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
	if (inet_del_protocol(&tunnel64_protocol, IPPROTO_IPV6))
		printk(KERN_ERR "tunnel64 close: can't remove protocol\n");
#endif
	if (inet_del_protocol(&tunnel4_protocol, IPPROTO_IPIP))
		printk(KERN_ERR "tunnel4 close: can't remove protocol\n");
}

module_init(tunnel4_init);
module_exit(tunnel4_fini);
MODULE_LICENSE("GPL");
