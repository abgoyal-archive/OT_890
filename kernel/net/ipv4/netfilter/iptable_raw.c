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
 * 'raw' table, which is the very first hooked in at PRE_ROUTING and LOCAL_OUT .
 *
 * Copyright (C) 2003 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
 */
#include <linux/module.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <net/ip.h>

#define RAW_VALID_HOOKS ((1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_LOCAL_OUT))

static struct
{
	struct ipt_replace repl;
	struct ipt_standard entries[2];
	struct ipt_error term;
} initial_table __net_initdata = {
	.repl = {
		.name = "raw",
		.valid_hooks = RAW_VALID_HOOKS,
		.num_entries = 3,
		.size = sizeof(struct ipt_standard) * 2 + sizeof(struct ipt_error),
		.hook_entry = {
			[NF_INET_PRE_ROUTING] = 0,
			[NF_INET_LOCAL_OUT] = sizeof(struct ipt_standard)
		},
		.underflow = {
			[NF_INET_PRE_ROUTING] = 0,
			[NF_INET_LOCAL_OUT]  = sizeof(struct ipt_standard)
		},
	},
	.entries = {
		IPT_STANDARD_INIT(NF_ACCEPT),	/* PRE_ROUTING */
		IPT_STANDARD_INIT(NF_ACCEPT),	/* LOCAL_OUT */
	},
	.term = IPT_ERROR_INIT,			/* ERROR */
};

static struct xt_table packet_raw = {
	.name = "raw",
	.valid_hooks =  RAW_VALID_HOOKS,
	.lock = __RW_LOCK_UNLOCKED(packet_raw.lock),
	.me = THIS_MODULE,
	.af = AF_INET,
};

/* The work comes in here from netfilter.c. */
static unsigned int
ipt_hook(unsigned int hook,
	 struct sk_buff *skb,
	 const struct net_device *in,
	 const struct net_device *out,
	 int (*okfn)(struct sk_buff *))
{
	return ipt_do_table(skb, hook, in, out,
			    dev_net(in)->ipv4.iptable_raw);
}

static unsigned int
ipt_local_hook(unsigned int hook,
	       struct sk_buff *skb,
	       const struct net_device *in,
	       const struct net_device *out,
	       int (*okfn)(struct sk_buff *))
{
	/* root is playing with raw sockets. */
	if (skb->len < sizeof(struct iphdr) ||
	    ip_hdrlen(skb) < sizeof(struct iphdr))
		return NF_ACCEPT;
	return ipt_do_table(skb, hook, in, out,
			    dev_net(out)->ipv4.iptable_raw);
}

/* 'raw' is the very first table. */
static struct nf_hook_ops ipt_ops[] __read_mostly = {
	{
		.hook = ipt_hook,
		.pf = PF_INET,
		.hooknum = NF_INET_PRE_ROUTING,
		.priority = NF_IP_PRI_RAW,
		.owner = THIS_MODULE,
	},
	{
		.hook = ipt_local_hook,
		.pf = PF_INET,
		.hooknum = NF_INET_LOCAL_OUT,
		.priority = NF_IP_PRI_RAW,
		.owner = THIS_MODULE,
	},
};

static int __net_init iptable_raw_net_init(struct net *net)
{
	/* Register table */
	net->ipv4.iptable_raw =
		ipt_register_table(net, &packet_raw, &initial_table.repl);
	if (IS_ERR(net->ipv4.iptable_raw))
		return PTR_ERR(net->ipv4.iptable_raw);
	return 0;
}

static void __net_exit iptable_raw_net_exit(struct net *net)
{
	ipt_unregister_table(net->ipv4.iptable_raw);
}

static struct pernet_operations iptable_raw_net_ops = {
	.init = iptable_raw_net_init,
	.exit = iptable_raw_net_exit,
};

static int __init iptable_raw_init(void)
{
	int ret;

	ret = register_pernet_subsys(&iptable_raw_net_ops);
	if (ret < 0)
		return ret;

	/* Register hooks */
	ret = nf_register_hooks(ipt_ops, ARRAY_SIZE(ipt_ops));
	if (ret < 0)
		goto cleanup_table;

	return ret;

 cleanup_table:
	unregister_pernet_subsys(&iptable_raw_net_ops);
	return ret;
}

static void __exit iptable_raw_fini(void)
{
	nf_unregister_hooks(ipt_ops, ARRAY_SIZE(ipt_ops));
	unregister_pernet_subsys(&iptable_raw_net_ops);
}

module_init(iptable_raw_init);
module_exit(iptable_raw_fini);
MODULE_LICENSE("GPL");
