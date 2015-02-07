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
 * Copyright (C) 2007 Red Hat, Inc.  All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License v.2.
 */

#include <net/genetlink.h>
#include <linux/dlm.h>
#include <linux/dlm_netlink.h>

#include "dlm_internal.h"

static uint32_t dlm_nl_seqnum;
static uint32_t listener_nlpid;

static struct genl_family family = {
	.id		= GENL_ID_GENERATE,
	.name		= DLM_GENL_NAME,
	.version	= DLM_GENL_VERSION,
};

static int prepare_data(u8 cmd, struct sk_buff **skbp, size_t size)
{
	struct sk_buff *skb;
	void *data;

	skb = genlmsg_new(size, GFP_KERNEL);
	if (!skb)
		return -ENOMEM;

	/* add the message headers */
	data = genlmsg_put(skb, 0, dlm_nl_seqnum++, &family, 0, cmd);
	if (!data) {
		nlmsg_free(skb);
		return -EINVAL;
	}

	*skbp = skb;
	return 0;
}

static struct dlm_lock_data *mk_data(struct sk_buff *skb)
{
	struct nlattr *ret;

	ret = nla_reserve(skb, DLM_TYPE_LOCK, sizeof(struct dlm_lock_data));
	if (!ret)
		return NULL;
	return nla_data(ret);
}

static int send_data(struct sk_buff *skb)
{
	struct genlmsghdr *genlhdr = nlmsg_data((struct nlmsghdr *)skb->data);
	void *data = genlmsg_data(genlhdr);
	int rv;

	rv = genlmsg_end(skb, data);
	if (rv < 0) {
		nlmsg_free(skb);
		return rv;
	}

	return genlmsg_unicast(skb, listener_nlpid);
}

static int user_cmd(struct sk_buff *skb, struct genl_info *info)
{
	listener_nlpid = info->snd_pid;
	printk("user_cmd nlpid %u\n", listener_nlpid);
	return 0;
}

static struct genl_ops dlm_nl_ops = {
	.cmd		= DLM_CMD_HELLO,
	.doit		= user_cmd,
};

int __init dlm_netlink_init(void)
{
	int rv;

	rv = genl_register_family(&family);
	if (rv)
		return rv;

	rv = genl_register_ops(&family, &dlm_nl_ops);
	if (rv < 0)
		goto err;
	return 0;
 err:
	genl_unregister_family(&family);
	return rv;
}

void dlm_netlink_exit(void)
{
	genl_unregister_ops(&family, &dlm_nl_ops);
	genl_unregister_family(&family);
}

static void fill_data(struct dlm_lock_data *data, struct dlm_lkb *lkb)
{
	struct dlm_rsb *r = lkb->lkb_resource;

	memset(data, 0, sizeof(struct dlm_lock_data));

	data->version = DLM_LOCK_DATA_VERSION;
	data->nodeid = lkb->lkb_nodeid;
	data->ownpid = lkb->lkb_ownpid;
	data->id = lkb->lkb_id;
	data->remid = lkb->lkb_remid;
	data->status = lkb->lkb_status;
	data->grmode = lkb->lkb_grmode;
	data->rqmode = lkb->lkb_rqmode;
	if (lkb->lkb_ua)
		data->xid = lkb->lkb_ua->xid;
	if (r) {
		data->lockspace_id = r->res_ls->ls_global_id;
		data->resource_namelen = r->res_length;
		memcpy(data->resource_name, r->res_name, r->res_length);
	}
}

void dlm_timeout_warn(struct dlm_lkb *lkb)
{
	struct sk_buff *uninitialized_var(send_skb);
	struct dlm_lock_data *data;
	size_t size;
	int rv;

	size = nla_total_size(sizeof(struct dlm_lock_data)) +
	       nla_total_size(0); /* why this? */

	rv = prepare_data(DLM_CMD_TIMEOUT, &send_skb, size);
	if (rv < 0)
		return;

	data = mk_data(send_skb);
	if (!data) {
		nlmsg_free(send_skb);
		return;
	}

	fill_data(data, lkb);

	send_data(send_skb);
}

