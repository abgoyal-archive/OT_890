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
 * ip_vs_est.c: simple rate estimator for IPVS
 *
 * Authors:     Wensong Zhang <wensong@linuxvirtualserver.org>
 *
 *              This program is free software; you can redistribute it and/or
 *              modify it under the terms of the GNU General Public License
 *              as published by the Free Software Foundation; either version
 *              2 of the License, or (at your option) any later version.
 *
 * Changes:
 *
 */
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/sysctl.h>
#include <linux/list.h>

#include <net/ip_vs.h>

/*
  This code is to estimate rate in a shorter interval (such as 8
  seconds) for virtual services and real servers. For measure rate in a
  long interval, it is easy to implement a user level daemon which
  periodically reads those statistical counters and measure rate.

  Currently, the measurement is activated by slow timer handler. Hope
  this measurement will not introduce too much load.

  We measure rate during the last 8 seconds every 2 seconds:

    avgrate = avgrate*(1-W) + rate*W

    where W = 2^(-2)

  NOTES.

  * The stored value for average bps is scaled by 2^5, so that maximal
    rate is ~2.15Gbits/s, average pps and cps are scaled by 2^10.

  * A lot code is taken from net/sched/estimator.c
 */


static void estimation_timer(unsigned long arg);

static LIST_HEAD(est_list);
static DEFINE_SPINLOCK(est_lock);
static DEFINE_TIMER(est_timer, estimation_timer, 0, 0);

static void estimation_timer(unsigned long arg)
{
	struct ip_vs_estimator *e;
	struct ip_vs_stats *s;
	u32 n_conns;
	u32 n_inpkts, n_outpkts;
	u64 n_inbytes, n_outbytes;
	u32 rate;

	spin_lock(&est_lock);
	list_for_each_entry(e, &est_list, list) {
		s = container_of(e, struct ip_vs_stats, est);

		spin_lock(&s->lock);
		n_conns = s->ustats.conns;
		n_inpkts = s->ustats.inpkts;
		n_outpkts = s->ustats.outpkts;
		n_inbytes = s->ustats.inbytes;
		n_outbytes = s->ustats.outbytes;

		/* scaled by 2^10, but divided 2 seconds */
		rate = (n_conns - e->last_conns)<<9;
		e->last_conns = n_conns;
		e->cps += ((long)rate - (long)e->cps)>>2;
		s->ustats.cps = (e->cps+0x1FF)>>10;

		rate = (n_inpkts - e->last_inpkts)<<9;
		e->last_inpkts = n_inpkts;
		e->inpps += ((long)rate - (long)e->inpps)>>2;
		s->ustats.inpps = (e->inpps+0x1FF)>>10;

		rate = (n_outpkts - e->last_outpkts)<<9;
		e->last_outpkts = n_outpkts;
		e->outpps += ((long)rate - (long)e->outpps)>>2;
		s->ustats.outpps = (e->outpps+0x1FF)>>10;

		rate = (n_inbytes - e->last_inbytes)<<4;
		e->last_inbytes = n_inbytes;
		e->inbps += ((long)rate - (long)e->inbps)>>2;
		s->ustats.inbps = (e->inbps+0xF)>>5;

		rate = (n_outbytes - e->last_outbytes)<<4;
		e->last_outbytes = n_outbytes;
		e->outbps += ((long)rate - (long)e->outbps)>>2;
		s->ustats.outbps = (e->outbps+0xF)>>5;
		spin_unlock(&s->lock);
	}
	spin_unlock(&est_lock);
	mod_timer(&est_timer, jiffies + 2*HZ);
}

void ip_vs_new_estimator(struct ip_vs_stats *stats)
{
	struct ip_vs_estimator *est = &stats->est;

	INIT_LIST_HEAD(&est->list);

	est->last_conns = stats->ustats.conns;
	est->cps = stats->ustats.cps<<10;

	est->last_inpkts = stats->ustats.inpkts;
	est->inpps = stats->ustats.inpps<<10;

	est->last_outpkts = stats->ustats.outpkts;
	est->outpps = stats->ustats.outpps<<10;

	est->last_inbytes = stats->ustats.inbytes;
	est->inbps = stats->ustats.inbps<<5;

	est->last_outbytes = stats->ustats.outbytes;
	est->outbps = stats->ustats.outbps<<5;

	spin_lock_bh(&est_lock);
	list_add(&est->list, &est_list);
	spin_unlock_bh(&est_lock);
}

void ip_vs_kill_estimator(struct ip_vs_stats *stats)
{
	struct ip_vs_estimator *est = &stats->est;

	spin_lock_bh(&est_lock);
	list_del(&est->list);
	spin_unlock_bh(&est_lock);
}

void ip_vs_zero_estimator(struct ip_vs_stats *stats)
{
	struct ip_vs_estimator *est = &stats->est;

	/* set counters zero, caller must hold the stats->lock lock */
	est->last_inbytes = 0;
	est->last_outbytes = 0;
	est->last_conns = 0;
	est->last_inpkts = 0;
	est->last_outpkts = 0;
	est->cps = 0;
	est->inpps = 0;
	est->outpps = 0;
	est->inbps = 0;
	est->outbps = 0;
}

int __init ip_vs_estimator_init(void)
{
	mod_timer(&est_timer, jiffies + 2 * HZ);
	return 0;
}

void ip_vs_estimator_cleanup(void)
{
	del_timer_sync(&est_timer);
}
