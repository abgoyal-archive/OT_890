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
 * H-TCP congestion control. The algorithm is detailed in:
 * R.N.Shorten, D.J.Leith:
 *   "H-TCP: TCP for high-speed and long-distance networks"
 *   Proc. PFLDnet, Argonne, 2004.
 * http://www.hamilton.ie/net/htcp3.pdf
 */

#include <linux/mm.h>
#include <linux/module.h>
#include <net/tcp.h>

#define ALPHA_BASE	(1<<7)	/* 1.0 with shift << 7 */
#define BETA_MIN	(1<<6)	/* 0.5 with shift << 7 */
#define BETA_MAX	102	/* 0.8 with shift << 7 */

static int use_rtt_scaling __read_mostly = 1;
module_param(use_rtt_scaling, int, 0644);
MODULE_PARM_DESC(use_rtt_scaling, "turn on/off RTT scaling");

static int use_bandwidth_switch __read_mostly = 1;
module_param(use_bandwidth_switch, int, 0644);
MODULE_PARM_DESC(use_bandwidth_switch, "turn on/off bandwidth switcher");

struct htcp {
	u32	alpha;		/* Fixed point arith, << 7 */
	u8	beta;           /* Fixed point arith, << 7 */
	u8	modeswitch;	/* Delay modeswitch
				   until we had at least one congestion event */
	u16	pkts_acked;
	u32	packetcount;
	u32	minRTT;
	u32	maxRTT;
	u32	last_cong;	/* Time since last congestion event end */
	u32	undo_last_cong;

	u32	undo_maxRTT;
	u32	undo_old_maxB;

	/* Bandwidth estimation */
	u32	minB;
	u32	maxB;
	u32	old_maxB;
	u32	Bi;
	u32	lasttime;
};

static inline u32 htcp_cong_time(const struct htcp *ca)
{
	return jiffies - ca->last_cong;
}

static inline u32 htcp_ccount(const struct htcp *ca)
{
	return htcp_cong_time(ca) / ca->minRTT;
}

static inline void htcp_reset(struct htcp *ca)
{
	ca->undo_last_cong = ca->last_cong;
	ca->undo_maxRTT = ca->maxRTT;
	ca->undo_old_maxB = ca->old_maxB;

	ca->last_cong = jiffies;
}

static u32 htcp_cwnd_undo(struct sock *sk)
{
	const struct tcp_sock *tp = tcp_sk(sk);
	struct htcp *ca = inet_csk_ca(sk);

	if (ca->undo_last_cong) {
		ca->last_cong = ca->undo_last_cong;
		ca->maxRTT = ca->undo_maxRTT;
		ca->old_maxB = ca->undo_old_maxB;
		ca->undo_last_cong = 0;
	}

	return max(tp->snd_cwnd, (tp->snd_ssthresh << 7) / ca->beta);
}

static inline void measure_rtt(struct sock *sk, u32 srtt)
{
	const struct inet_connection_sock *icsk = inet_csk(sk);
	struct htcp *ca = inet_csk_ca(sk);

	/* keep track of minimum RTT seen so far, minRTT is zero at first */
	if (ca->minRTT > srtt || !ca->minRTT)
		ca->minRTT = srtt;

	/* max RTT */
	if (icsk->icsk_ca_state == TCP_CA_Open) {
		if (ca->maxRTT < ca->minRTT)
			ca->maxRTT = ca->minRTT;
		if (ca->maxRTT < srtt
		    && srtt <= ca->maxRTT + msecs_to_jiffies(20))
			ca->maxRTT = srtt;
	}
}

static void measure_achieved_throughput(struct sock *sk, u32 pkts_acked, s32 rtt)
{
	const struct inet_connection_sock *icsk = inet_csk(sk);
	const struct tcp_sock *tp = tcp_sk(sk);
	struct htcp *ca = inet_csk_ca(sk);
	u32 now = tcp_time_stamp;

	if (icsk->icsk_ca_state == TCP_CA_Open)
		ca->pkts_acked = pkts_acked;

	if (rtt > 0)
		measure_rtt(sk, usecs_to_jiffies(rtt));

	if (!use_bandwidth_switch)
		return;

	/* achieved throughput calculations */
	if (icsk->icsk_ca_state != TCP_CA_Open &&
	    icsk->icsk_ca_state != TCP_CA_Disorder) {
		ca->packetcount = 0;
		ca->lasttime = now;
		return;
	}

	ca->packetcount += pkts_acked;

	if (ca->packetcount >= tp->snd_cwnd - (ca->alpha >> 7 ? : 1)
	    && now - ca->lasttime >= ca->minRTT
	    && ca->minRTT > 0) {
		__u32 cur_Bi = ca->packetcount * HZ / (now - ca->lasttime);

		if (htcp_ccount(ca) <= 3) {
			/* just after backoff */
			ca->minB = ca->maxB = ca->Bi = cur_Bi;
		} else {
			ca->Bi = (3 * ca->Bi + cur_Bi) / 4;
			if (ca->Bi > ca->maxB)
				ca->maxB = ca->Bi;
			if (ca->minB > ca->maxB)
				ca->minB = ca->maxB;
		}
		ca->packetcount = 0;
		ca->lasttime = now;
	}
}

static inline void htcp_beta_update(struct htcp *ca, u32 minRTT, u32 maxRTT)
{
	if (use_bandwidth_switch) {
		u32 maxB = ca->maxB;
		u32 old_maxB = ca->old_maxB;
		ca->old_maxB = ca->maxB;

		if (!between(5 * maxB, 4 * old_maxB, 6 * old_maxB)) {
			ca->beta = BETA_MIN;
			ca->modeswitch = 0;
			return;
		}
	}

	if (ca->modeswitch && minRTT > msecs_to_jiffies(10) && maxRTT) {
		ca->beta = (minRTT << 7) / maxRTT;
		if (ca->beta < BETA_MIN)
			ca->beta = BETA_MIN;
		else if (ca->beta > BETA_MAX)
			ca->beta = BETA_MAX;
	} else {
		ca->beta = BETA_MIN;
		ca->modeswitch = 1;
	}
}

static inline void htcp_alpha_update(struct htcp *ca)
{
	u32 minRTT = ca->minRTT;
	u32 factor = 1;
	u32 diff = htcp_cong_time(ca);

	if (diff > HZ) {
		diff -= HZ;
		factor = 1 + (10 * diff + ((diff / 2) * (diff / 2) / HZ)) / HZ;
	}

	if (use_rtt_scaling && minRTT) {
		u32 scale = (HZ << 3) / (10 * minRTT);

		/* clamping ratio to interval [0.5,10]<<3 */
		scale = min(max(scale, 1U << 2), 10U << 3);
		factor = (factor << 3) / scale;
		if (!factor)
			factor = 1;
	}

	ca->alpha = 2 * factor * ((1 << 7) - ca->beta);
	if (!ca->alpha)
		ca->alpha = ALPHA_BASE;
}

/*
 * After we have the rtt data to calculate beta, we'd still prefer to wait one
 * rtt before we adjust our beta to ensure we are working from a consistent
 * data.
 *
 * This function should be called when we hit a congestion event since only at
 * that point do we really have a real sense of maxRTT (the queues en route
 * were getting just too full now).
 */
static void htcp_param_update(struct sock *sk)
{
	struct htcp *ca = inet_csk_ca(sk);
	u32 minRTT = ca->minRTT;
	u32 maxRTT = ca->maxRTT;

	htcp_beta_update(ca, minRTT, maxRTT);
	htcp_alpha_update(ca);

	/* add slowly fading memory for maxRTT to accommodate routing changes */
	if (minRTT > 0 && maxRTT > minRTT)
		ca->maxRTT = minRTT + ((maxRTT - minRTT) * 95) / 100;
}

static u32 htcp_recalc_ssthresh(struct sock *sk)
{
	const struct tcp_sock *tp = tcp_sk(sk);
	const struct htcp *ca = inet_csk_ca(sk);

	htcp_param_update(sk);
	return max((tp->snd_cwnd * ca->beta) >> 7, 2U);
}

static void htcp_cong_avoid(struct sock *sk, u32 ack, u32 in_flight)
{
	struct tcp_sock *tp = tcp_sk(sk);
	struct htcp *ca = inet_csk_ca(sk);

	if (!tcp_is_cwnd_limited(sk, in_flight))
		return;

	if (tp->snd_cwnd <= tp->snd_ssthresh)
		tcp_slow_start(tp);
	else {
		/* In dangerous area, increase slowly.
		 * In theory this is tp->snd_cwnd += alpha / tp->snd_cwnd
		 */
		if ((tp->snd_cwnd_cnt * ca->alpha)>>7 >= tp->snd_cwnd) {
			if (tp->snd_cwnd < tp->snd_cwnd_clamp)
				tp->snd_cwnd++;
			tp->snd_cwnd_cnt = 0;
			htcp_alpha_update(ca);
		} else
			tp->snd_cwnd_cnt += ca->pkts_acked;

		ca->pkts_acked = 1;
	}
}

static void htcp_init(struct sock *sk)
{
	struct htcp *ca = inet_csk_ca(sk);

	memset(ca, 0, sizeof(struct htcp));
	ca->alpha = ALPHA_BASE;
	ca->beta = BETA_MIN;
	ca->pkts_acked = 1;
	ca->last_cong = jiffies;
}

static void htcp_state(struct sock *sk, u8 new_state)
{
	switch (new_state) {
	case TCP_CA_Open:
		{
			struct htcp *ca = inet_csk_ca(sk);
			if (ca->undo_last_cong) {
				ca->last_cong = jiffies;
				ca->undo_last_cong = 0;
			}
		}
		break;
	case TCP_CA_CWR:
	case TCP_CA_Recovery:
	case TCP_CA_Loss:
		htcp_reset(inet_csk_ca(sk));
		break;
	}
}

static struct tcp_congestion_ops htcp = {
	.init		= htcp_init,
	.ssthresh	= htcp_recalc_ssthresh,
	.cong_avoid	= htcp_cong_avoid,
	.set_state	= htcp_state,
	.undo_cwnd	= htcp_cwnd_undo,
	.pkts_acked	= measure_achieved_throughput,
	.owner		= THIS_MODULE,
	.name		= "htcp",
};

static int __init htcp_register(void)
{
	BUILD_BUG_ON(sizeof(struct htcp) > ICSK_CA_PRIV_SIZE);
	BUILD_BUG_ON(BETA_MIN >= BETA_MAX);
	return tcp_register_congestion_control(&htcp);
}

static void __exit htcp_unregister(void)
{
	tcp_unregister_congestion_control(&htcp);
}

module_init(htcp_register);
module_exit(htcp_unregister);

MODULE_AUTHOR("Baruch Even");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("H-TCP");