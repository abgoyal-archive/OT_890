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
 * Netlink event notifications for SELinux.
 *
 * Author: James Morris <jmorris@redhat.com>
 *
 * Copyright (C) 2004 Red Hat, Inc., James Morris <jmorris@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2,
 * as published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/types.h>
#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/skbuff.h>
#include <linux/netlink.h>
#include <linux/selinux_netlink.h>
#include <net/net_namespace.h>

static struct sock *selnl;

static int selnl_msglen(int msgtype)
{
	int ret = 0;

	switch (msgtype) {
	case SELNL_MSG_SETENFORCE:
		ret = sizeof(struct selnl_msg_setenforce);
		break;

	case SELNL_MSG_POLICYLOAD:
		ret = sizeof(struct selnl_msg_policyload);
		break;

	default:
		BUG();
	}
	return ret;
}

static void selnl_add_payload(struct nlmsghdr *nlh, int len, int msgtype, void *data)
{
	switch (msgtype) {
	case SELNL_MSG_SETENFORCE: {
		struct selnl_msg_setenforce *msg = NLMSG_DATA(nlh);

		memset(msg, 0, len);
		msg->val = *((int *)data);
		break;
	}

	case SELNL_MSG_POLICYLOAD: {
		struct selnl_msg_policyload *msg = NLMSG_DATA(nlh);

		memset(msg, 0, len);
		msg->seqno = *((u32 *)data);
		break;
	}

	default:
		BUG();
	}
}

static void selnl_notify(int msgtype, void *data)
{
	int len;
	sk_buff_data_t tmp;
	struct sk_buff *skb;
	struct nlmsghdr *nlh;

	len = selnl_msglen(msgtype);

	skb = alloc_skb(NLMSG_SPACE(len), GFP_USER);
	if (!skb)
		goto oom;

	tmp = skb->tail;
	nlh = NLMSG_PUT(skb, 0, 0, msgtype, len);
	selnl_add_payload(nlh, len, msgtype, data);
	nlh->nlmsg_len = skb->tail - tmp;
	NETLINK_CB(skb).dst_group = SELNLGRP_AVC;
	netlink_broadcast(selnl, skb, 0, SELNLGRP_AVC, GFP_USER);
out:
	return;

nlmsg_failure:
	kfree_skb(skb);
oom:
	printk(KERN_ERR "SELinux:  OOM in %s\n", __func__);
	goto out;
}

void selnl_notify_setenforce(int val)
{
	selnl_notify(SELNL_MSG_SETENFORCE, &val);
}

void selnl_notify_policyload(u32 seqno)
{
	selnl_notify(SELNL_MSG_POLICYLOAD, &seqno);
}

static int __init selnl_init(void)
{
	selnl = netlink_kernel_create(&init_net, NETLINK_SELINUX,
				      SELNLGRP_MAX, NULL, NULL, THIS_MODULE);
	if (selnl == NULL)
		panic("SELinux:  Cannot create netlink socket.");
	netlink_set_nonroot(NETLINK_SELINUX, NL_NONROOT_RECV);
	return 0;
}

__initcall(selnl_init);
