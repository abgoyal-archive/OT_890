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

#include <linux/sysctl.h>
#include <net/net_namespace.h>
#include <net/xfrm.h>

static void __xfrm_sysctl_init(struct net *net)
{
	net->xfrm.sysctl_aevent_etime = XFRM_AE_ETIME;
	net->xfrm.sysctl_aevent_rseqth = XFRM_AE_SEQT_SIZE;
	net->xfrm.sysctl_larval_drop = 1;
	net->xfrm.sysctl_acq_expires = 30;
}

#ifdef CONFIG_SYSCTL
static struct ctl_table xfrm_table[] = {
	{
		.ctl_name	= NET_CORE_AEVENT_ETIME,
		.procname	= "xfrm_aevent_etime",
		.maxlen		= sizeof(u32),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.ctl_name	= NET_CORE_AEVENT_RSEQTH,
		.procname	= "xfrm_aevent_rseqth",
		.maxlen		= sizeof(u32),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.ctl_name	= CTL_UNNUMBERED,
		.procname	= "xfrm_larval_drop",
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.ctl_name	= CTL_UNNUMBERED,
		.procname	= "xfrm_acq_expires",
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{}
};

int __net_init xfrm_sysctl_init(struct net *net)
{
	struct ctl_table *table;

	__xfrm_sysctl_init(net);

	table = kmemdup(xfrm_table, sizeof(xfrm_table), GFP_KERNEL);
	if (!table)
		goto out_kmemdup;
	table[0].data = &net->xfrm.sysctl_aevent_etime;
	table[1].data = &net->xfrm.sysctl_aevent_rseqth;
	table[2].data = &net->xfrm.sysctl_larval_drop;
	table[3].data = &net->xfrm.sysctl_acq_expires;

	net->xfrm.sysctl_hdr = register_net_sysctl_table(net, net_core_path, table);
	if (!net->xfrm.sysctl_hdr)
		goto out_register;
	return 0;

out_register:
	kfree(table);
out_kmemdup:
	return -ENOMEM;
}

void xfrm_sysctl_fini(struct net *net)
{
	struct ctl_table *table;

	table = net->xfrm.sysctl_hdr->ctl_table_arg;
	unregister_net_sysctl_table(net->xfrm.sysctl_hdr);
	kfree(table);
}
#else
int __net_init xfrm_sysctl_init(struct net *net)
{
	__xfrm_sysctl_init(net);
	return 0;
}
#endif
