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
 * xfrm_output.c - Common IPsec encapsulation code.
 *
 * Copyright (c) 2007 Herbert Xu <herbert@gondor.apana.org.au>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/errno.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <net/dst.h>
#include <net/xfrm.h>

static int xfrm_output2(struct sk_buff *skb);

static int xfrm_state_check_space(struct xfrm_state *x, struct sk_buff *skb)
{
	struct dst_entry *dst = skb->dst;
	int nhead = dst->header_len + LL_RESERVED_SPACE(dst->dev)
		- skb_headroom(skb);
	int ntail = dst->dev->needed_tailroom - skb_tailroom(skb);

	if (nhead <= 0) {
		if (ntail <= 0)
			return 0;
		nhead = 0;
	} else if (ntail < 0)
		ntail = 0;

	return pskb_expand_head(skb, nhead, ntail, GFP_ATOMIC);
}

static int xfrm_output_one(struct sk_buff *skb, int err)
{
	struct dst_entry *dst = skb->dst;
	struct xfrm_state *x = dst->xfrm;
	struct net *net = xs_net(x);

	if (err <= 0)
		goto resume;

	do {
		err = xfrm_state_check_space(x, skb);
		if (err) {
			XFRM_INC_STATS(net, LINUX_MIB_XFRMOUTERROR);
			goto error_nolock;
		}

		err = x->outer_mode->output(x, skb);
		if (err) {
			XFRM_INC_STATS(net, LINUX_MIB_XFRMOUTSTATEMODEERROR);
			goto error_nolock;
		}

		spin_lock_bh(&x->lock);
		err = xfrm_state_check_expire(x);
		if (err) {
			XFRM_INC_STATS(net, LINUX_MIB_XFRMOUTSTATEEXPIRED);
			goto error;
		}

		if (x->type->flags & XFRM_TYPE_REPLAY_PROT) {
			XFRM_SKB_CB(skb)->seq.output = ++x->replay.oseq;
			if (unlikely(x->replay.oseq == 0)) {
				XFRM_INC_STATS(net, LINUX_MIB_XFRMOUTSTATESEQERROR);
				x->replay.oseq--;
				xfrm_audit_state_replay_overflow(x, skb);
				err = -EOVERFLOW;
				goto error;
			}
			if (xfrm_aevent_is_on(net))
				xfrm_replay_notify(x, XFRM_REPLAY_UPDATE);
		}

		x->curlft.bytes += skb->len;
		x->curlft.packets++;

		spin_unlock_bh(&x->lock);

		err = x->type->output(x, skb);
		if (err == -EINPROGRESS)
			goto out_exit;

resume:
		if (err) {
			XFRM_INC_STATS(net, LINUX_MIB_XFRMOUTSTATEPROTOERROR);
			goto error_nolock;
		}

		if (!(skb->dst = dst_pop(dst))) {
			XFRM_INC_STATS(net, LINUX_MIB_XFRMOUTERROR);
			err = -EHOSTUNREACH;
			goto error_nolock;
		}
		dst = skb->dst;
		x = dst->xfrm;
	} while (x && !(x->outer_mode->flags & XFRM_MODE_FLAG_TUNNEL));

	err = 0;

out_exit:
	return err;
error:
	spin_unlock_bh(&x->lock);
error_nolock:
	kfree_skb(skb);
	goto out_exit;
}

int xfrm_output_resume(struct sk_buff *skb, int err)
{
	while (likely((err = xfrm_output_one(skb, err)) == 0)) {
		nf_reset(skb);

		err = skb->dst->ops->local_out(skb);
		if (unlikely(err != 1))
			goto out;

		if (!skb->dst->xfrm)
			return dst_output(skb);

		err = nf_hook(skb->dst->ops->family,
			      NF_INET_POST_ROUTING, skb,
			      NULL, skb->dst->dev, xfrm_output2);
		if (unlikely(err != 1))
			goto out;
	}

	if (err == -EINPROGRESS)
		err = 0;

out:
	return err;
}
EXPORT_SYMBOL_GPL(xfrm_output_resume);

static int xfrm_output2(struct sk_buff *skb)
{
	return xfrm_output_resume(skb, 1);
}

static int xfrm_output_gso(struct sk_buff *skb)
{
	struct sk_buff *segs;

	segs = skb_gso_segment(skb, 0);
	kfree_skb(skb);
	if (IS_ERR(segs))
		return PTR_ERR(segs);

	do {
		struct sk_buff *nskb = segs->next;
		int err;

		segs->next = NULL;
		err = xfrm_output2(segs);

		if (unlikely(err)) {
			while ((segs = nskb)) {
				nskb = segs->next;
				segs->next = NULL;
				kfree_skb(segs);
			}
			return err;
		}

		segs = nskb;
	} while (segs);

	return 0;
}

int xfrm_output(struct sk_buff *skb)
{
	struct net *net = dev_net(skb->dst->dev);
	int err;

	if (skb_is_gso(skb))
		return xfrm_output_gso(skb);

	if (skb->ip_summed == CHECKSUM_PARTIAL) {
		err = skb_checksum_help(skb);
		if (err) {
			XFRM_INC_STATS(net, LINUX_MIB_XFRMOUTERROR);
			kfree_skb(skb);
			return err;
		}
	}

	return xfrm_output2(skb);
}

int xfrm_inner_extract_output(struct xfrm_state *x, struct sk_buff *skb)
{
	struct xfrm_mode *inner_mode;
	if (x->sel.family == AF_UNSPEC)
		inner_mode = xfrm_ip2inner_mode(x,
				xfrm_af2proto(skb->dst->ops->family));
	else
		inner_mode = x->inner_mode;

	if (inner_mode == NULL)
		return -EAFNOSUPPORT;
	return inner_mode->afinfo->extract_output(x, skb);
}

EXPORT_SYMBOL_GPL(xfrm_output);
EXPORT_SYMBOL_GPL(xfrm_inner_extract_output);
