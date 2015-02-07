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
 * Kernel module to match various things tied to sockets associated with
 * locally generated outgoing packets.
 *
 * (C) 2000 Marc Boucher <marc@mbsi.ca>
 *
 * Copyright © CC Computer Consultants GmbH, 2007 - 2008
 * <jengelh@computergmbh.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/file.h>
#include <net/sock.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_owner.h>
#include <linux/netfilter_ipv4/ipt_owner.h>
#include <linux/netfilter_ipv6/ip6t_owner.h>

static bool
owner_mt_v0(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct ipt_owner_info *info = par->matchinfo;
	const struct file *filp;

	if (skb->sk == NULL || skb->sk->sk_socket == NULL)
		return false;

	filp = skb->sk->sk_socket->file;
	if (filp == NULL)
		return false;

	if (info->match & IPT_OWNER_UID)
		if ((filp->f_cred->fsuid != info->uid) ^
		    !!(info->invert & IPT_OWNER_UID))
			return false;

	if (info->match & IPT_OWNER_GID)
		if ((filp->f_cred->fsgid != info->gid) ^
		    !!(info->invert & IPT_OWNER_GID))
			return false;

	return true;
}

static bool
owner_mt6_v0(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct ip6t_owner_info *info = par->matchinfo;
	const struct file *filp;

	if (skb->sk == NULL || skb->sk->sk_socket == NULL)
		return false;

	filp = skb->sk->sk_socket->file;
	if (filp == NULL)
		return false;

	if (info->match & IP6T_OWNER_UID)
		if ((filp->f_cred->fsuid != info->uid) ^
		    !!(info->invert & IP6T_OWNER_UID))
			return false;

	if (info->match & IP6T_OWNER_GID)
		if ((filp->f_cred->fsgid != info->gid) ^
		    !!(info->invert & IP6T_OWNER_GID))
			return false;

	return true;
}

static bool
owner_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct xt_owner_match_info *info = par->matchinfo;
	const struct file *filp;

	if (skb->sk == NULL || skb->sk->sk_socket == NULL)
		return (info->match ^ info->invert) == 0;
	else if (info->match & info->invert & XT_OWNER_SOCKET)
		/*
		 * Socket exists but user wanted ! --socket-exists.
		 * (Single ampersands intended.)
		 */
		return false;

	filp = skb->sk->sk_socket->file;
	if (filp == NULL)
		return ((info->match ^ info->invert) &
		       (XT_OWNER_UID | XT_OWNER_GID)) == 0;

	if (info->match & XT_OWNER_UID)
		if ((filp->f_cred->fsuid >= info->uid_min &&
		    filp->f_cred->fsuid <= info->uid_max) ^
		    !(info->invert & XT_OWNER_UID))
			return false;

	if (info->match & XT_OWNER_GID)
		if ((filp->f_cred->fsgid >= info->gid_min &&
		    filp->f_cred->fsgid <= info->gid_max) ^
		    !(info->invert & XT_OWNER_GID))
			return false;

	return true;
}

static bool owner_mt_check_v0(const struct xt_mtchk_param *par)
{
	const struct ipt_owner_info *info = par->matchinfo;

	if (info->match & (IPT_OWNER_PID | IPT_OWNER_SID | IPT_OWNER_COMM)) {
		printk(KERN_WARNING KBUILD_MODNAME
		       ": PID, SID and command matching is not "
		       "supported anymore\n");
		return false;
	}

	return true;
}

static bool owner_mt6_check_v0(const struct xt_mtchk_param *par)
{
	const struct ip6t_owner_info *info = par->matchinfo;

	if (info->match & (IP6T_OWNER_PID | IP6T_OWNER_SID)) {
		printk(KERN_WARNING KBUILD_MODNAME
		       ": PID and SID matching is not supported anymore\n");
		return false;
	}

	return true;
}

static struct xt_match owner_mt_reg[] __read_mostly = {
	{
		.name       = "owner",
		.revision   = 0,
		.family     = NFPROTO_IPV4,
		.match      = owner_mt_v0,
		.matchsize  = sizeof(struct ipt_owner_info),
		.checkentry = owner_mt_check_v0,
		.hooks      = (1 << NF_INET_LOCAL_OUT) |
		              (1 << NF_INET_POST_ROUTING),
		.me         = THIS_MODULE,
	},
	{
		.name       = "owner",
		.revision   = 0,
		.family     = NFPROTO_IPV6,
		.match      = owner_mt6_v0,
		.matchsize  = sizeof(struct ip6t_owner_info),
		.checkentry = owner_mt6_check_v0,
		.hooks      = (1 << NF_INET_LOCAL_OUT) |
		              (1 << NF_INET_POST_ROUTING),
		.me         = THIS_MODULE,
	},
	{
		.name       = "owner",
		.revision   = 1,
		.family     = NFPROTO_UNSPEC,
		.match      = owner_mt,
		.matchsize  = sizeof(struct xt_owner_match_info),
		.hooks      = (1 << NF_INET_LOCAL_OUT) |
		              (1 << NF_INET_POST_ROUTING),
		.me         = THIS_MODULE,
	},
};

static int __init owner_mt_init(void)
{
	return xt_register_matches(owner_mt_reg, ARRAY_SIZE(owner_mt_reg));
}

static void __exit owner_mt_exit(void)
{
	xt_unregister_matches(owner_mt_reg, ARRAY_SIZE(owner_mt_reg));
}

module_init(owner_mt_init);
module_exit(owner_mt_exit);
MODULE_AUTHOR("Jan Engelhardt <jengelh@computergmbh.de>");
MODULE_DESCRIPTION("Xtables: socket owner matching");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ipt_owner");
MODULE_ALIAS("ip6t_owner");
