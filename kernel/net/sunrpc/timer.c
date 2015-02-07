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
 * linux/net/sunrpc/timer.c
 *
 * Estimate RPC request round trip time.
 *
 * Based on packet round-trip and variance estimator algorithms described
 * in appendix A of "Congestion Avoidance and Control" by Van Jacobson
 * and Michael J. Karels (ACM Computer Communication Review; Proceedings
 * of the Sigcomm '88 Symposium in Stanford, CA, August, 1988).
 *
 * This RTT estimator is used only for RPC over datagram protocols.
 *
 * Copyright (C) 2002 Trond Myklebust <trond.myklebust@fys.uio.no>
 */

#include <asm/param.h>

#include <linux/types.h>
#include <linux/unistd.h>
#include <linux/module.h>

#include <linux/sunrpc/clnt.h>

#define RPC_RTO_MAX (60*HZ)
#define RPC_RTO_INIT (HZ/5)
#define RPC_RTO_MIN (HZ/10)

void
rpc_init_rtt(struct rpc_rtt *rt, unsigned long timeo)
{
	unsigned long init = 0;
	unsigned i;

	rt->timeo = timeo;

	if (timeo > RPC_RTO_INIT)
		init = (timeo - RPC_RTO_INIT) << 3;
	for (i = 0; i < 5; i++) {
		rt->srtt[i] = init;
		rt->sdrtt[i] = RPC_RTO_INIT;
		rt->ntimeouts[i] = 0;
	}
}
EXPORT_SYMBOL_GPL(rpc_init_rtt);

/*
 * NB: When computing the smoothed RTT and standard deviation,
 *     be careful not to produce negative intermediate results.
 */
void
rpc_update_rtt(struct rpc_rtt *rt, unsigned timer, long m)
{
	long *srtt, *sdrtt;

	if (timer-- == 0)
		return;

	/* jiffies wrapped; ignore this one */
	if (m < 0)
		return;

	if (m == 0)
		m = 1L;

	srtt = (long *)&rt->srtt[timer];
	m -= *srtt >> 3;
	*srtt += m;

	if (m < 0)
		m = -m;

	sdrtt = (long *)&rt->sdrtt[timer];
	m -= *sdrtt >> 2;
	*sdrtt += m;

	/* Set lower bound on the variance */
	if (*sdrtt < RPC_RTO_MIN)
		*sdrtt = RPC_RTO_MIN;
}
EXPORT_SYMBOL_GPL(rpc_update_rtt);

/*
 * Estimate rto for an nfs rpc sent via. an unreliable datagram.
 * Use the mean and mean deviation of rtt for the appropriate type of rpc
 * for the frequent rpcs and a default for the others.
 * The justification for doing "other" this way is that these rpcs
 * happen so infrequently that timer est. would probably be stale.
 * Also, since many of these rpcs are
 * non-idempotent, a conservative timeout is desired.
 * getattr, lookup,
 * read, write, commit     - A+4D
 * other                   - timeo
 */

unsigned long
rpc_calc_rto(struct rpc_rtt *rt, unsigned timer)
{
	unsigned long res;

	if (timer-- == 0)
		return rt->timeo;

	res = ((rt->srtt[timer] + 7) >> 3) + rt->sdrtt[timer];
	if (res > RPC_RTO_MAX)
		res = RPC_RTO_MAX;

	return res;
}
EXPORT_SYMBOL_GPL(rpc_calc_rto);
