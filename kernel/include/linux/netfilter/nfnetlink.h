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

#ifndef _NFNETLINK_H
#define _NFNETLINK_H
#include <linux/types.h>
#include <linux/netfilter/nfnetlink_compat.h>

enum nfnetlink_groups {
	NFNLGRP_NONE,
#define NFNLGRP_NONE			NFNLGRP_NONE
	NFNLGRP_CONNTRACK_NEW,
#define NFNLGRP_CONNTRACK_NEW		NFNLGRP_CONNTRACK_NEW
	NFNLGRP_CONNTRACK_UPDATE,
#define NFNLGRP_CONNTRACK_UPDATE	NFNLGRP_CONNTRACK_UPDATE
	NFNLGRP_CONNTRACK_DESTROY,
#define NFNLGRP_CONNTRACK_DESTROY	NFNLGRP_CONNTRACK_DESTROY
	NFNLGRP_CONNTRACK_EXP_NEW,
#define	NFNLGRP_CONNTRACK_EXP_NEW	NFNLGRP_CONNTRACK_EXP_NEW
	NFNLGRP_CONNTRACK_EXP_UPDATE,
#define NFNLGRP_CONNTRACK_EXP_UPDATE	NFNLGRP_CONNTRACK_EXP_UPDATE
	NFNLGRP_CONNTRACK_EXP_DESTROY,
#define NFNLGRP_CONNTRACK_EXP_DESTROY	NFNLGRP_CONNTRACK_EXP_DESTROY
	__NFNLGRP_MAX,
};
#define NFNLGRP_MAX	(__NFNLGRP_MAX - 1)

/* General form of address family dependent message.
 */
struct nfgenmsg {
	u_int8_t  nfgen_family;		/* AF_xxx */
	u_int8_t  version;		/* nfnetlink version */
	__be16    res_id;		/* resource id */
};

#define NFNETLINK_V0	0

/* netfilter netlink message types are split in two pieces:
 * 8 bit subsystem, 8bit operation.
 */

#define NFNL_SUBSYS_ID(x)	((x & 0xff00) >> 8)
#define NFNL_MSG_TYPE(x)	(x & 0x00ff)

/* No enum here, otherwise __stringify() trick of MODULE_ALIAS_NFNL_SUBSYS()
 * won't work anymore */
#define NFNL_SUBSYS_NONE 		0
#define NFNL_SUBSYS_CTNETLINK		1
#define NFNL_SUBSYS_CTNETLINK_EXP	2
#define NFNL_SUBSYS_QUEUE		3
#define NFNL_SUBSYS_ULOG		4
#define NFNL_SUBSYS_COUNT		5

#ifdef __KERNEL__

#include <linux/netlink.h>
#include <linux/capability.h>
#include <net/netlink.h>

struct nfnl_callback
{
	int (*call)(struct sock *nl, struct sk_buff *skb, 
		struct nlmsghdr *nlh, struct nlattr *cda[]);
	const struct nla_policy *policy;	/* netlink attribute policy */
	const u_int16_t attr_count;		/* number of nlattr's */
};

struct nfnetlink_subsystem
{
	const char *name;
	__u8 subsys_id;			/* nfnetlink subsystem ID */
	__u8 cb_count;			/* number of callbacks */
	const struct nfnl_callback *cb;	/* callback for individual types */
};

extern int nfnetlink_subsys_register(const struct nfnetlink_subsystem *n);
extern int nfnetlink_subsys_unregister(const struct nfnetlink_subsystem *n);

extern int nfnetlink_has_listeners(unsigned int group);
extern int nfnetlink_send(struct sk_buff *skb, u32 pid, unsigned group, 
			  int echo);
extern int nfnetlink_unicast(struct sk_buff *skb, u_int32_t pid, int flags);

extern void nfnl_lock(void);
extern void nfnl_unlock(void);

#define MODULE_ALIAS_NFNL_SUBSYS(subsys) \
	MODULE_ALIAS("nfnetlink-subsys-" __stringify(subsys))

#endif	/* __KERNEL__ */
#endif	/* _NFNETLINK_H */
