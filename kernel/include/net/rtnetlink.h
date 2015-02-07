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

#ifndef __NET_RTNETLINK_H
#define __NET_RTNETLINK_H

#include <linux/rtnetlink.h>
#include <net/netlink.h>

typedef int (*rtnl_doit_func)(struct sk_buff *, struct nlmsghdr *, void *);
typedef int (*rtnl_dumpit_func)(struct sk_buff *, struct netlink_callback *);

extern int	__rtnl_register(int protocol, int msgtype,
				rtnl_doit_func, rtnl_dumpit_func);
extern void	rtnl_register(int protocol, int msgtype,
			      rtnl_doit_func, rtnl_dumpit_func);
extern int	rtnl_unregister(int protocol, int msgtype);
extern void	rtnl_unregister_all(int protocol);

static inline int rtnl_msg_family(struct nlmsghdr *nlh)
{
	if (nlmsg_len(nlh) >= sizeof(struct rtgenmsg))
		return ((struct rtgenmsg *) nlmsg_data(nlh))->rtgen_family;
	else
		return AF_UNSPEC;
}

/**
 *	struct rtnl_link_ops - rtnetlink link operations
 *
 *	@list: Used internally
 *	@kind: Identifier
 *	@maxtype: Highest device specific netlink attribute number
 *	@policy: Netlink policy for device specific attribute validation
 *	@validate: Optional validation function for netlink/changelink parameters
 *	@priv_size: sizeof net_device private space
 *	@setup: net_device setup function
 *	@newlink: Function for configuring and registering a new device
 *	@changelink: Function for changing parameters of an existing device
 *	@dellink: Function to remove a device
 *	@get_size: Function to calculate required room for dumping device
 *		   specific netlink attributes
 *	@fill_info: Function to dump device specific netlink attributes
 *	@get_xstats_size: Function to calculate required room for dumping devic
 *			  specific statistics
 *	@fill_xstats: Function to dump device specific statistics
 */
struct rtnl_link_ops {
	struct list_head	list;

	const char		*kind;

	size_t			priv_size;
	void			(*setup)(struct net_device *dev);

	int			maxtype;
	const struct nla_policy	*policy;
	int			(*validate)(struct nlattr *tb[],
					    struct nlattr *data[]);

	int			(*newlink)(struct net_device *dev,
					   struct nlattr *tb[],
					   struct nlattr *data[]);
	int			(*changelink)(struct net_device *dev,
					      struct nlattr *tb[],
					      struct nlattr *data[]);
	void			(*dellink)(struct net_device *dev);

	size_t			(*get_size)(const struct net_device *dev);
	int			(*fill_info)(struct sk_buff *skb,
					     const struct net_device *dev);

	size_t			(*get_xstats_size)(const struct net_device *dev);
	int			(*fill_xstats)(struct sk_buff *skb,
					       const struct net_device *dev);
};

extern int	__rtnl_link_register(struct rtnl_link_ops *ops);
extern void	__rtnl_link_unregister(struct rtnl_link_ops *ops);
extern void	rtnl_kill_links(struct net *net, struct rtnl_link_ops *ops);

extern int	rtnl_link_register(struct rtnl_link_ops *ops);
extern void	rtnl_link_unregister(struct rtnl_link_ops *ops);

extern struct net_device *rtnl_create_link(struct net *net, char *ifname,
		const struct rtnl_link_ops *ops, struct nlattr *tb[]);
extern const struct nla_policy ifla_policy[IFLA_MAX+1];

#define MODULE_ALIAS_RTNL_LINK(kind) MODULE_ALIAS("rtnl-link-" kind)

#endif
