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

/* Tom Kelly's Scalable TCP
 *
 * See http://www.deneholme.net/tom/scalable/
 *
 * John Heffner <jheffner@sc.edu>
 */

#include <linux/module.h>
#include <net/tcp.h>

/* These factors derived from the recommended values in the aer:
 * .01 and and 7/8. We use 50 instead of 100 to account for
 * delayed ack.
 */
#define TCP_SCALABLE_AI_CNT	50U
#define TCP_SCALABLE_MD_SCALE	3

static void tcp_scalable_cong_avoid(struct sock *sk, u32 ack, u32 in_flight)
{
	struct tcp_sock *tp = tcp_sk(sk);

	if (!tcp_is_cwnd_limited(sk, in_flight))
		return;

	if (tp->snd_cwnd <= tp->snd_ssthresh)
		tcp_slow_start(tp);
	else {
		tp->snd_cwnd_cnt++;
		if (tp->snd_cwnd_cnt > min(tp->snd_cwnd, TCP_SCALABLE_AI_CNT)){
			if (tp->snd_cwnd < tp->snd_cwnd_clamp)
				tp->snd_cwnd++;
			tp->snd_cwnd_cnt = 0;
		}
	}
}

static u32 tcp_scalable_ssthresh(struct sock *sk)
{
	const struct tcp_sock *tp = tcp_sk(sk);
	return max(tp->snd_cwnd - (tp->snd_cwnd>>TCP_SCALABLE_MD_SCALE), 2U);
}


static struct tcp_congestion_ops tcp_scalable = {
	.ssthresh	= tcp_scalable_ssthresh,
	.cong_avoid	= tcp_scalable_cong_avoid,
	.min_cwnd	= tcp_reno_min_cwnd,

	.owner		= THIS_MODULE,
	.name		= "scalable",
};

static int __init tcp_scalable_register(void)
{
	return tcp_register_congestion_control(&tcp_scalable);
}

static void __exit tcp_scalable_unregister(void)
{
	tcp_unregister_congestion_control(&tcp_scalable);
}

module_init(tcp_scalable_register);
module_exit(tcp_scalable_unregister);

MODULE_AUTHOR("John Heffner");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Scalable TCP");
