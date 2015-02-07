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
 * net/sched/em_nbyte.c	N-Byte ematch
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Thomas Graf <tgraf@suug.ch>
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/skbuff.h>
#include <linux/tc_ematch/tc_em_nbyte.h>
#include <net/pkt_cls.h>

struct nbyte_data
{
	struct tcf_em_nbyte	hdr;
	char			pattern[0];
};

static int em_nbyte_change(struct tcf_proto *tp, void *data, int data_len,
			   struct tcf_ematch *em)
{
	struct tcf_em_nbyte *nbyte = data;

	if (data_len < sizeof(*nbyte) ||
	    data_len < (sizeof(*nbyte) + nbyte->len))
		return -EINVAL;

	em->datalen = sizeof(*nbyte) + nbyte->len;
	em->data = (unsigned long)kmemdup(data, em->datalen, GFP_KERNEL);
	if (em->data == 0UL)
		return -ENOBUFS;

	return 0;
}

static int em_nbyte_match(struct sk_buff *skb, struct tcf_ematch *em,
			  struct tcf_pkt_info *info)
{
	struct nbyte_data *nbyte = (struct nbyte_data *) em->data;
	unsigned char *ptr = tcf_get_base_ptr(skb, nbyte->hdr.layer);

	ptr += nbyte->hdr.off;

	if (!tcf_valid_offset(skb, ptr, nbyte->hdr.len))
		return 0;

	return !memcmp(ptr + nbyte->hdr.off, nbyte->pattern, nbyte->hdr.len);
}

static struct tcf_ematch_ops em_nbyte_ops = {
	.kind	  = TCF_EM_NBYTE,
	.change	  = em_nbyte_change,
	.match	  = em_nbyte_match,
	.owner	  = THIS_MODULE,
	.link	  = LIST_HEAD_INIT(em_nbyte_ops.link)
};

static int __init init_em_nbyte(void)
{
	return tcf_em_register(&em_nbyte_ops);
}

static void __exit exit_em_nbyte(void)
{
	tcf_em_unregister(&em_nbyte_ops);
}

MODULE_LICENSE("GPL");

module_init(init_em_nbyte);
module_exit(exit_em_nbyte);

MODULE_ALIAS_TCF_EMATCH(TCF_EM_NBYTE);
