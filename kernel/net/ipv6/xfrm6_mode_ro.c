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
 * xfrm6_mode_ro.c - Route optimization mode for IPv6.
 *
 * Copyright (C)2003-2006 Helsinki University of Technology
 * Copyright (C)2003-2006 USAGI/WIDE Project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/*
 * Authors:
 *	Noriaki TAKAMIYA @USAGI
 *	Masahide NAKAMURA @USAGI
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/stringify.h>
#include <linux/time.h>
#include <net/ipv6.h>
#include <net/xfrm.h>

/* Add route optimization header space.
 *
 * The IP header and mutable extension headers will be moved forward to make
 * space for the route optimization header.
 */
static int xfrm6_ro_output(struct xfrm_state *x, struct sk_buff *skb)
{
	struct ipv6hdr *iph;
	u8 *prevhdr;
	int hdr_len;

	iph = ipv6_hdr(skb);

	hdr_len = x->type->hdr_offset(x, skb, &prevhdr);
	skb_set_mac_header(skb, (prevhdr - x->props.header_len) - skb->data);
	skb_set_network_header(skb, -x->props.header_len);
	skb->transport_header = skb->network_header + hdr_len;
	__skb_pull(skb, hdr_len);
	memmove(ipv6_hdr(skb), iph, hdr_len);

	x->lastused = get_seconds();

	return 0;
}

static struct xfrm_mode xfrm6_ro_mode = {
	.output = xfrm6_ro_output,
	.owner = THIS_MODULE,
	.encap = XFRM_MODE_ROUTEOPTIMIZATION,
};

static int __init xfrm6_ro_init(void)
{
	return xfrm_register_mode(&xfrm6_ro_mode, AF_INET6);
}

static void __exit xfrm6_ro_exit(void)
{
	int err;

	err = xfrm_unregister_mode(&xfrm6_ro_mode, AF_INET6);
	BUG_ON(err);
}

module_init(xfrm6_ro_init);
module_exit(xfrm6_ro_exit);
MODULE_LICENSE("GPL");
MODULE_ALIAS_XFRM_MODE(AF_INET6, XFRM_MODE_ROUTEOPTIMIZATION);
