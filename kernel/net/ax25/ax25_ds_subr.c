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
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Copyright (C) Jonathan Naylor G4KLX (g4klx@g4klx.demon.co.uk)
 * Copyright (C) Joerg Reuter DL1BKE (jreuter@yaina.de)
 */
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/string.h>
#include <linux/sockios.h>
#include <linux/spinlock.h>
#include <linux/net.h>
#include <net/ax25.h>
#include <linux/inet.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/interrupt.h>

void ax25_ds_nr_error_recovery(ax25_cb *ax25)
{
	ax25_ds_establish_data_link(ax25);
}

/*
 *	dl1bke 960114: transmit I frames on DAMA poll
 */
void ax25_ds_enquiry_response(ax25_cb *ax25)
{
	ax25_cb *ax25o;
	struct hlist_node *node;

	/* Please note that neither DK4EG's nor DG2FEF's
	 * DAMA spec mention the following behaviour as seen
	 * with TheFirmware:
	 *
	 * 	DB0ACH->DL1BKE <RR C P R0> [DAMA]
	 *	DL1BKE->DB0ACH <I NR=0 NS=0>
	 *	DL1BKE-7->DB0PRA-6 DB0ACH <I C S3 R5>
	 *	DL1BKE->DB0ACH <RR R F R0>
	 *
	 * The Flexnet DAMA Master implementation apparently
	 * insists on the "proper" AX.25 behaviour:
	 *
	 * 	DB0ACH->DL1BKE <RR C P R0> [DAMA]
	 *	DL1BKE->DB0ACH <RR R F R0>
	 *	DL1BKE->DB0ACH <I NR=0 NS=0>
	 *	DL1BKE-7->DB0PRA-6 DB0ACH <I C S3 R5>
	 *
	 * Flexnet refuses to send us *any* I frame if we send
	 * a REJ in case AX25_COND_REJECT is set. It is superfluous in
	 * this mode anyway (a RR or RNR invokes the retransmission).
	 * Is this a Flexnet bug?
	 */

	ax25_std_enquiry_response(ax25);

	if (!(ax25->condition & AX25_COND_PEER_RX_BUSY)) {
		ax25_requeue_frames(ax25);
		ax25_kick(ax25);
	}

	if (ax25->state == AX25_STATE_1 || ax25->state == AX25_STATE_2 || skb_peek(&ax25->ack_queue) != NULL)
		ax25_ds_t1_timeout(ax25);
	else
		ax25->n2count = 0;

	ax25_start_t3timer(ax25);
	ax25_ds_set_timer(ax25->ax25_dev);

	spin_lock(&ax25_list_lock);
	ax25_for_each(ax25o, node, &ax25_list) {
		if (ax25o == ax25)
			continue;

		if (ax25o->ax25_dev != ax25->ax25_dev)
			continue;

		if (ax25o->state == AX25_STATE_1 || ax25o->state == AX25_STATE_2) {
			ax25_ds_t1_timeout(ax25o);
			continue;
		}

		if (!(ax25o->condition & AX25_COND_PEER_RX_BUSY) && ax25o->state == AX25_STATE_3) {
			ax25_requeue_frames(ax25o);
			ax25_kick(ax25o);
		}

		if (ax25o->state == AX25_STATE_1 || ax25o->state == AX25_STATE_2 || skb_peek(&ax25o->ack_queue) != NULL)
			ax25_ds_t1_timeout(ax25o);

		/* do not start T3 for listening sockets (tnx DD8NE) */

		if (ax25o->state != AX25_STATE_0)
			ax25_start_t3timer(ax25o);
	}
	spin_unlock(&ax25_list_lock);
}

void ax25_ds_establish_data_link(ax25_cb *ax25)
{
	ax25->condition &= AX25_COND_DAMA_MODE;
	ax25->n2count    = 0;
	ax25_calculate_t1(ax25);
	ax25_start_t1timer(ax25);
	ax25_stop_t2timer(ax25);
	ax25_start_t3timer(ax25);
}

/*
 *	:::FIXME:::
 *	This is a kludge. Not all drivers recognize kiss commands.
 *	We need a driver level  request to switch duplex mode, that does
 *	either SCC changing, PI config or KISS as required. Currently
 *	this request isn't reliable.
 */
static void ax25_kiss_cmd(ax25_dev *ax25_dev, unsigned char cmd, unsigned char param)
{
	struct sk_buff *skb;
	unsigned char *p;

	if (ax25_dev->dev == NULL)
		return;

	if ((skb = alloc_skb(2, GFP_ATOMIC)) == NULL)
		return;

	skb_reset_network_header(skb);
	p = skb_put(skb, 2);

	*p++ = cmd;
	*p++ = param;

	skb->protocol = ax25_type_trans(skb, ax25_dev->dev);

	dev_queue_xmit(skb);
}

/*
 *	A nasty problem arises if we count the number of DAMA connections
 *	wrong, especially when connections on the device already existed
 *	and our network node (or the sysop) decides to turn on DAMA Master
 *	mode. We thus flag the 'real' slave connections with
 *	ax25->dama_slave=1 and look on every disconnect if still slave
 *	connections exist.
 */
static int ax25_check_dama_slave(ax25_dev *ax25_dev)
{
	ax25_cb *ax25;
	int res = 0;
	struct hlist_node *node;

	spin_lock(&ax25_list_lock);
	ax25_for_each(ax25, node, &ax25_list)
		if (ax25->ax25_dev == ax25_dev && (ax25->condition & AX25_COND_DAMA_MODE) && ax25->state > AX25_STATE_1) {
			res = 1;
			break;
		}
	spin_unlock(&ax25_list_lock);

	return res;
}

static void ax25_dev_dama_on(ax25_dev *ax25_dev)
{
	if (ax25_dev == NULL)
		return;

	if (ax25_dev->dama.slave == 0)
		ax25_kiss_cmd(ax25_dev, 5, 1);

	ax25_dev->dama.slave = 1;
	ax25_ds_set_timer(ax25_dev);
}

void ax25_dev_dama_off(ax25_dev *ax25_dev)
{
	if (ax25_dev == NULL)
		return;

	if (ax25_dev->dama.slave && !ax25_check_dama_slave(ax25_dev)) {
		ax25_kiss_cmd(ax25_dev, 5, 0);
		ax25_dev->dama.slave = 0;
		ax25_ds_del_timer(ax25_dev);
	}
}

void ax25_dama_on(ax25_cb *ax25)
{
	ax25_dev_dama_on(ax25->ax25_dev);
	ax25->condition |= AX25_COND_DAMA_MODE;
}

void ax25_dama_off(ax25_cb *ax25)
{
	ax25->condition &= ~AX25_COND_DAMA_MODE;
	ax25_dev_dama_off(ax25->ax25_dev);
}

