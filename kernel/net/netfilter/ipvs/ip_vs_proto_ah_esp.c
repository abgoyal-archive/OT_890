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
 * ip_vs_proto_ah_esp.c:	AH/ESP IPSec load balancing support for IPVS
 *
 * Authors:	Julian Anastasov <ja@ssi.bg>, February 2002
 *		Wensong Zhang <wensong@linuxvirtualserver.org>
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		version 2 as published by the Free Software Foundation;
 *
 */

#include <linux/in.h>
#include <linux/ip.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

#include <net/ip_vs.h>


/* TODO:

struct isakmp_hdr {
	__u8		icookie[8];
	__u8		rcookie[8];
	__u8		np;
	__u8		version;
	__u8		xchgtype;
	__u8		flags;
	__u32		msgid;
	__u32		length;
};

*/

#define PORT_ISAKMP	500


static struct ip_vs_conn *
ah_esp_conn_in_get(int af, const struct sk_buff *skb, struct ip_vs_protocol *pp,
		   const struct ip_vs_iphdr *iph, unsigned int proto_off,
		   int inverse)
{
	struct ip_vs_conn *cp;

	if (likely(!inverse)) {
		cp = ip_vs_conn_in_get(af, IPPROTO_UDP,
				       &iph->saddr,
				       htons(PORT_ISAKMP),
				       &iph->daddr,
				       htons(PORT_ISAKMP));
	} else {
		cp = ip_vs_conn_in_get(af, IPPROTO_UDP,
				       &iph->daddr,
				       htons(PORT_ISAKMP),
				       &iph->saddr,
				       htons(PORT_ISAKMP));
	}

	if (!cp) {
		/*
		 * We are not sure if the packet is from our
		 * service, so our conn_schedule hook should return NF_ACCEPT
		 */
		IP_VS_DBG_BUF(12, "Unknown ISAKMP entry for outin packet "
			      "%s%s %s->%s\n",
			      inverse ? "ICMP+" : "",
			      pp->name,
			      IP_VS_DBG_ADDR(af, &iph->saddr),
			      IP_VS_DBG_ADDR(af, &iph->daddr));
	}

	return cp;
}


static struct ip_vs_conn *
ah_esp_conn_out_get(int af, const struct sk_buff *skb,
		    struct ip_vs_protocol *pp,
		    const struct ip_vs_iphdr *iph,
		    unsigned int proto_off,
		    int inverse)
{
	struct ip_vs_conn *cp;

	if (likely(!inverse)) {
		cp = ip_vs_conn_out_get(af, IPPROTO_UDP,
					&iph->saddr,
					htons(PORT_ISAKMP),
					&iph->daddr,
					htons(PORT_ISAKMP));
	} else {
		cp = ip_vs_conn_out_get(af, IPPROTO_UDP,
					&iph->daddr,
					htons(PORT_ISAKMP),
					&iph->saddr,
					htons(PORT_ISAKMP));
	}

	if (!cp) {
		IP_VS_DBG_BUF(12, "Unknown ISAKMP entry for inout packet "
			      "%s%s %s->%s\n",
			      inverse ? "ICMP+" : "",
			      pp->name,
			      IP_VS_DBG_ADDR(af, &iph->saddr),
			      IP_VS_DBG_ADDR(af, &iph->daddr));
	}

	return cp;
}


static int
ah_esp_conn_schedule(int af, struct sk_buff *skb, struct ip_vs_protocol *pp,
		     int *verdict, struct ip_vs_conn **cpp)
{
	/*
	 * AH/ESP is only related traffic. Pass the packet to IP stack.
	 */
	*verdict = NF_ACCEPT;
	return 0;
}


static void
ah_esp_debug_packet_v4(struct ip_vs_protocol *pp, const struct sk_buff *skb,
		       int offset, const char *msg)
{
	char buf[256];
	struct iphdr _iph, *ih;

	ih = skb_header_pointer(skb, offset, sizeof(_iph), &_iph);
	if (ih == NULL)
		sprintf(buf, "%s TRUNCATED", pp->name);
	else
		sprintf(buf, "%s %pI4->%pI4",
			pp->name, &ih->saddr, &ih->daddr);

	printk(KERN_DEBUG "IPVS: %s: %s\n", msg, buf);
}

#ifdef CONFIG_IP_VS_IPV6
static void
ah_esp_debug_packet_v6(struct ip_vs_protocol *pp, const struct sk_buff *skb,
		       int offset, const char *msg)
{
	char buf[256];
	struct ipv6hdr _iph, *ih;

	ih = skb_header_pointer(skb, offset, sizeof(_iph), &_iph);
	if (ih == NULL)
		sprintf(buf, "%s TRUNCATED", pp->name);
	else
		sprintf(buf, "%s %pI6->%pI6",
			pp->name, &ih->saddr, &ih->daddr);

	printk(KERN_DEBUG "IPVS: %s: %s\n", msg, buf);
}
#endif

static void
ah_esp_debug_packet(struct ip_vs_protocol *pp, const struct sk_buff *skb,
		    int offset, const char *msg)
{
#ifdef CONFIG_IP_VS_IPV6
	if (skb->protocol == htons(ETH_P_IPV6))
		ah_esp_debug_packet_v6(pp, skb, offset, msg);
	else
#endif
		ah_esp_debug_packet_v4(pp, skb, offset, msg);
}


static void ah_esp_init(struct ip_vs_protocol *pp)
{
	/* nothing to do now */
}


static void ah_esp_exit(struct ip_vs_protocol *pp)
{
	/* nothing to do now */
}


#ifdef CONFIG_IP_VS_PROTO_AH
struct ip_vs_protocol ip_vs_protocol_ah = {
	.name =			"AH",
	.protocol =		IPPROTO_AH,
	.num_states =		1,
	.dont_defrag =		1,
	.init =			ah_esp_init,
	.exit =			ah_esp_exit,
	.conn_schedule =	ah_esp_conn_schedule,
	.conn_in_get =		ah_esp_conn_in_get,
	.conn_out_get =		ah_esp_conn_out_get,
	.snat_handler =		NULL,
	.dnat_handler =		NULL,
	.csum_check =		NULL,
	.state_transition =	NULL,
	.register_app =		NULL,
	.unregister_app =	NULL,
	.app_conn_bind =	NULL,
	.debug_packet =		ah_esp_debug_packet,
	.timeout_change =	NULL,		/* ISAKMP */
	.set_state_timeout =	NULL,
};
#endif

#ifdef CONFIG_IP_VS_PROTO_ESP
struct ip_vs_protocol ip_vs_protocol_esp = {
	.name =			"ESP",
	.protocol =		IPPROTO_ESP,
	.num_states =		1,
	.dont_defrag =		1,
	.init =			ah_esp_init,
	.exit =			ah_esp_exit,
	.conn_schedule =	ah_esp_conn_schedule,
	.conn_in_get =		ah_esp_conn_in_get,
	.conn_out_get =		ah_esp_conn_out_get,
	.snat_handler =		NULL,
	.dnat_handler =		NULL,
	.csum_check =		NULL,
	.state_transition =	NULL,
	.register_app =		NULL,
	.unregister_app =	NULL,
	.app_conn_bind =	NULL,
	.debug_packet =		ah_esp_debug_packet,
	.timeout_change =	NULL,		/* ISAKMP */
};
#endif
