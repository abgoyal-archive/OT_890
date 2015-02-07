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
 * IPVS:        Weighted Least-Connection Scheduling module
 *
 * Authors:     Wensong Zhang <wensong@linuxvirtualserver.org>
 *              Peter Kese <peter.kese@ijs.si>
 *
 *              This program is free software; you can redistribute it and/or
 *              modify it under the terms of the GNU General Public License
 *              as published by the Free Software Foundation; either version
 *              2 of the License, or (at your option) any later version.
 *
 * Changes:
 *     Wensong Zhang            :     changed the ip_vs_wlc_schedule to return dest
 *     Wensong Zhang            :     changed to use the inactconns in scheduling
 *     Wensong Zhang            :     changed some comestics things for debugging
 *     Wensong Zhang            :     changed for the d-linked destination list
 *     Wensong Zhang            :     added the ip_vs_wlc_update_svc
 *     Wensong Zhang            :     added any dest with weight=0 is quiesced
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>

#include <net/ip_vs.h>


static inline unsigned int
ip_vs_wlc_dest_overhead(struct ip_vs_dest *dest)
{
	/*
	 * We think the overhead of processing active connections is 256
	 * times higher than that of inactive connections in average. (This
	 * 256 times might not be accurate, we will change it later) We
	 * use the following formula to estimate the overhead now:
	 *		  dest->activeconns*256 + dest->inactconns
	 */
	return (atomic_read(&dest->activeconns) << 8) +
		atomic_read(&dest->inactconns);
}


/*
 *	Weighted Least Connection scheduling
 */
static struct ip_vs_dest *
ip_vs_wlc_schedule(struct ip_vs_service *svc, const struct sk_buff *skb)
{
	struct ip_vs_dest *dest, *least;
	unsigned int loh, doh;

	IP_VS_DBG(6, "ip_vs_wlc_schedule(): Scheduling...\n");

	/*
	 * We calculate the load of each dest server as follows:
	 *		  (dest overhead) / dest->weight
	 *
	 * Remember -- no floats in kernel mode!!!
	 * The comparison of h1*w2 > h2*w1 is equivalent to that of
	 *		  h1/w1 > h2/w2
	 * if every weight is larger than zero.
	 *
	 * The server with weight=0 is quiesced and will not receive any
	 * new connections.
	 */

	list_for_each_entry(dest, &svc->destinations, n_list) {
		if (!(dest->flags & IP_VS_DEST_F_OVERLOAD) &&
		    atomic_read(&dest->weight) > 0) {
			least = dest;
			loh = ip_vs_wlc_dest_overhead(least);
			goto nextstage;
		}
	}
	IP_VS_ERR_RL("WLC: no destination available\n");
	return NULL;

	/*
	 *    Find the destination with the least load.
	 */
  nextstage:
	list_for_each_entry_continue(dest, &svc->destinations, n_list) {
		if (dest->flags & IP_VS_DEST_F_OVERLOAD)
			continue;
		doh = ip_vs_wlc_dest_overhead(dest);
		if (loh * atomic_read(&dest->weight) >
		    doh * atomic_read(&least->weight)) {
			least = dest;
			loh = doh;
		}
	}

	IP_VS_DBG_BUF(6, "WLC: server %s:%u "
		      "activeconns %d refcnt %d weight %d overhead %d\n",
		      IP_VS_DBG_ADDR(svc->af, &least->addr), ntohs(least->port),
		      atomic_read(&least->activeconns),
		      atomic_read(&least->refcnt),
		      atomic_read(&least->weight), loh);

	return least;
}


static struct ip_vs_scheduler ip_vs_wlc_scheduler =
{
	.name =			"wlc",
	.refcnt =		ATOMIC_INIT(0),
	.module =		THIS_MODULE,
	.n_list =		LIST_HEAD_INIT(ip_vs_wlc_scheduler.n_list),
	.schedule =		ip_vs_wlc_schedule,
};


static int __init ip_vs_wlc_init(void)
{
	return register_ip_vs_scheduler(&ip_vs_wlc_scheduler);
}

static void __exit ip_vs_wlc_cleanup(void)
{
	unregister_ip_vs_scheduler(&ip_vs_wlc_scheduler);
}

module_init(ip_vs_wlc_init);
module_exit(ip_vs_wlc_cleanup);
MODULE_LICENSE("GPL");
