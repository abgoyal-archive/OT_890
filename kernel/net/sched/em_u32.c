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
 * net/sched/em_u32.c	U32 Ematch
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Thomas Graf <tgraf@suug.ch>
 *		Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *
 * Based on net/sched/cls_u32.c
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <net/pkt_cls.h>

static int em_u32_match(struct sk_buff *skb, struct tcf_ematch *em,
			struct tcf_pkt_info *info)
{
	struct tc_u32_key *key = (struct tc_u32_key *) em->data;
	const unsigned char *ptr = skb_network_header(skb);

	if (info) {
		if (info->ptr)
			ptr = info->ptr;
		ptr += (info->nexthdr & key->offmask);
	}

	ptr += key->off;

	if (!tcf_valid_offset(skb, ptr, sizeof(u32)))
		return 0;

	return !(((*(__be32*) ptr)  ^ key->val) & key->mask);
}

static struct tcf_ematch_ops em_u32_ops = {
	.kind	  = TCF_EM_U32,
	.datalen  = sizeof(struct tc_u32_key),
	.match	  = em_u32_match,
	.owner	  = THIS_MODULE,
	.link	  = LIST_HEAD_INIT(em_u32_ops.link)
};

static int __init init_em_u32(void)
{
	return tcf_em_register(&em_u32_ops);
}

static void __exit exit_em_u32(void)
{
	tcf_em_unregister(&em_u32_ops);
}

MODULE_LICENSE("GPL");

module_init(init_em_u32);
module_exit(exit_em_u32);

MODULE_ALIAS_TCF_EMATCH(TCF_EM_U32);
