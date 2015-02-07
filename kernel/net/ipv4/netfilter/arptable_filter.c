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
 * Filtering ARP tables module.
 *
 * Copyright (C) 2002 David S. Miller (davem@redhat.com)
 *
 */

#include <linux/module.h>
#include <linux/netfilter_arp/arp_tables.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("David S. Miller <davem@redhat.com>");
MODULE_DESCRIPTION("arptables filter table");

#define FILTER_VALID_HOOKS ((1 << NF_ARP_IN) | (1 << NF_ARP_OUT) | \
			   (1 << NF_ARP_FORWARD))

static struct
{
	struct arpt_replace repl;
	struct arpt_standard entries[3];
	struct arpt_error term;
} initial_table __net_initdata = {
	.repl = {
		.name = "filter",
		.valid_hooks = FILTER_VALID_HOOKS,
		.num_entries = 4,
		.size = sizeof(struct arpt_standard) * 3 + sizeof(struct arpt_error),
		.hook_entry = {
			[NF_ARP_IN] = 0,
			[NF_ARP_OUT] = sizeof(struct arpt_standard),
			[NF_ARP_FORWARD] = 2 * sizeof(struct arpt_standard),
		},
		.underflow = {
			[NF_ARP_IN] = 0,
			[NF_ARP_OUT] = sizeof(struct arpt_standard),
			[NF_ARP_FORWARD] = 2 * sizeof(struct arpt_standard),
		},
	},
	.entries = {
		ARPT_STANDARD_INIT(NF_ACCEPT),	/* ARP_IN */
		ARPT_STANDARD_INIT(NF_ACCEPT),	/* ARP_OUT */
		ARPT_STANDARD_INIT(NF_ACCEPT),	/* ARP_FORWARD */
	},
	.term = ARPT_ERROR_INIT,
};

static struct xt_table packet_filter = {
	.name		= "filter",
	.valid_hooks	= FILTER_VALID_HOOKS,
	.lock		= __RW_LOCK_UNLOCKED(packet_filter.lock),
	.private	= NULL,
	.me		= THIS_MODULE,
	.af		= NFPROTO_ARP,
};

/* The work comes in here from netfilter.c */
static unsigned int arpt_in_hook(unsigned int hook,
				 struct sk_buff *skb,
				 const struct net_device *in,
				 const struct net_device *out,
				 int (*okfn)(struct sk_buff *))
{
	return arpt_do_table(skb, hook, in, out,
			     dev_net(in)->ipv4.arptable_filter);
}

static unsigned int arpt_out_hook(unsigned int hook,
				  struct sk_buff *skb,
				  const struct net_device *in,
				  const struct net_device *out,
				  int (*okfn)(struct sk_buff *))
{
	return arpt_do_table(skb, hook, in, out,
			     dev_net(out)->ipv4.arptable_filter);
}

static struct nf_hook_ops arpt_ops[] __read_mostly = {
	{
		.hook		= arpt_in_hook,
		.owner		= THIS_MODULE,
		.pf		= NFPROTO_ARP,
		.hooknum	= NF_ARP_IN,
		.priority	= NF_IP_PRI_FILTER,
	},
	{
		.hook		= arpt_out_hook,
		.owner		= THIS_MODULE,
		.pf		= NFPROTO_ARP,
		.hooknum	= NF_ARP_OUT,
		.priority	= NF_IP_PRI_FILTER,
	},
	{
		.hook		= arpt_in_hook,
		.owner		= THIS_MODULE,
		.pf		= NFPROTO_ARP,
		.hooknum	= NF_ARP_FORWARD,
		.priority	= NF_IP_PRI_FILTER,
	},
};

static int __net_init arptable_filter_net_init(struct net *net)
{
	/* Register table */
	net->ipv4.arptable_filter =
		arpt_register_table(net, &packet_filter, &initial_table.repl);
	if (IS_ERR(net->ipv4.arptable_filter))
		return PTR_ERR(net->ipv4.arptable_filter);
	return 0;
}

static void __net_exit arptable_filter_net_exit(struct net *net)
{
	arpt_unregister_table(net->ipv4.arptable_filter);
}

static struct pernet_operations arptable_filter_net_ops = {
	.init = arptable_filter_net_init,
	.exit = arptable_filter_net_exit,
};

static int __init arptable_filter_init(void)
{
	int ret;

	ret = register_pernet_subsys(&arptable_filter_net_ops);
	if (ret < 0)
		return ret;

	ret = nf_register_hooks(arpt_ops, ARRAY_SIZE(arpt_ops));
	if (ret < 0)
		goto cleanup_table;
	return ret;

cleanup_table:
	unregister_pernet_subsys(&arptable_filter_net_ops);
	return ret;
}

static void __exit arptable_filter_fini(void)
{
	nf_unregister_hooks(arpt_ops, ARRAY_SIZE(arpt_ops));
	unregister_pernet_subsys(&arptable_filter_net_ops);
}

module_init(arptable_filter_init);
module_exit(arptable_filter_fini);
