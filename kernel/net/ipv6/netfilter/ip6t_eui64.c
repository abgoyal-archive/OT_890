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

/* Kernel module to match EUI64 address parameters. */

/* (C) 2001-2002 Andras Kis-Szabo <kisza@sch.bme.hu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ipv6.h>
#include <linux/if_ether.h>

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv6/ip6_tables.h>

MODULE_DESCRIPTION("Xtables: IPv6 EUI64 address match");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andras Kis-Szabo <kisza@sch.bme.hu>");

static bool
eui64_mt6(const struct sk_buff *skb, const struct xt_match_param *par)
{
	unsigned char eui64[8];
	int i = 0;

	if (!(skb_mac_header(skb) >= skb->head &&
	      skb_mac_header(skb) + ETH_HLEN <= skb->data) &&
	    par->fragoff != 0) {
		*par->hotdrop = true;
		return false;
	}

	memset(eui64, 0, sizeof(eui64));

	if (eth_hdr(skb)->h_proto == htons(ETH_P_IPV6)) {
		if (ipv6_hdr(skb)->version == 0x6) {
			memcpy(eui64, eth_hdr(skb)->h_source, 3);
			memcpy(eui64 + 5, eth_hdr(skb)->h_source + 3, 3);
			eui64[3] = 0xff;
			eui64[4] = 0xfe;
			eui64[0] ^= 0x02;

			i = 0;
			while (ipv6_hdr(skb)->saddr.s6_addr[8 + i] == eui64[i]
			       && i < 8)
				i++;

			if (i == 8)
				return true;
		}
	}

	return false;
}

static struct xt_match eui64_mt6_reg __read_mostly = {
	.name		= "eui64",
	.family		= NFPROTO_IPV6,
	.match		= eui64_mt6,
	.matchsize	= sizeof(int),
	.hooks		= (1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_LOCAL_IN) |
			  (1 << NF_INET_FORWARD),
	.me		= THIS_MODULE,
};

static int __init eui64_mt6_init(void)
{
	return xt_register_match(&eui64_mt6_reg);
}

static void __exit eui64_mt6_exit(void)
{
	xt_unregister_match(&eui64_mt6_reg);
}

module_init(eui64_mt6_init);
module_exit(eui64_mt6_exit);
