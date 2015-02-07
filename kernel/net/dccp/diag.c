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
 *  net/dccp/diag.c
 *
 *  An implementation of the DCCP protocol
 *  Arnaldo Carvalho de Melo <acme@mandriva.com>
 *
 *	This program is free software; you can redistribute it and/or modify it
 *	under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 */


#include <linux/module.h>
#include <linux/inet_diag.h>

#include "ccid.h"
#include "dccp.h"

static void dccp_get_info(struct sock *sk, struct tcp_info *info)
{
	struct dccp_sock *dp = dccp_sk(sk);
	const struct inet_connection_sock *icsk = inet_csk(sk);

	memset(info, 0, sizeof(*info));

	info->tcpi_state	= sk->sk_state;
	info->tcpi_retransmits	= icsk->icsk_retransmits;
	info->tcpi_probes	= icsk->icsk_probes_out;
	info->tcpi_backoff	= icsk->icsk_backoff;
	info->tcpi_pmtu		= icsk->icsk_pmtu_cookie;

	if (dp->dccps_hc_rx_ackvec != NULL)
		info->tcpi_options |= TCPI_OPT_SACK;

	if (dp->dccps_hc_rx_ccid != NULL)
		ccid_hc_rx_get_info(dp->dccps_hc_rx_ccid, sk, info);

	if (dp->dccps_hc_tx_ccid != NULL)
		ccid_hc_tx_get_info(dp->dccps_hc_tx_ccid, sk, info);
}

static void dccp_diag_get_info(struct sock *sk, struct inet_diag_msg *r,
			       void *_info)
{
	r->idiag_rqueue = r->idiag_wqueue = 0;

	if (_info != NULL)
		dccp_get_info(sk, _info);
}

static const struct inet_diag_handler dccp_diag_handler = {
	.idiag_hashinfo	 = &dccp_hashinfo,
	.idiag_get_info	 = dccp_diag_get_info,
	.idiag_type	 = DCCPDIAG_GETSOCK,
	.idiag_info_size = sizeof(struct tcp_info),
};

static int __init dccp_diag_init(void)
{
	return inet_diag_register(&dccp_diag_handler);
}

static void __exit dccp_diag_fini(void)
{
	inet_diag_unregister(&dccp_diag_handler);
}

module_init(dccp_diag_init);
module_exit(dccp_diag_fini);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arnaldo Carvalho de Melo <acme@mandriva.com>");
MODULE_DESCRIPTION("DCCP inet_diag handler");
MODULE_ALIAS_NET_PF_PROTO_TYPE(PF_NETLINK, NETLINK_INET_DIAG, DCCPDIAG_GETSOCK);
