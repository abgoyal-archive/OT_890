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
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#ifndef _BR_PRIVATE_STP_H
#define _BR_PRIVATE_STP_H

#define BPDU_TYPE_CONFIG 0
#define BPDU_TYPE_TCN 0x80

struct br_config_bpdu
{
	unsigned	topology_change:1;
	unsigned	topology_change_ack:1;
	bridge_id	root;
	int		root_path_cost;
	bridge_id	bridge_id;
	port_id		port_id;
	int		message_age;
	int		max_age;
	int		hello_time;
	int		forward_delay;
};

/* called under bridge lock */
static inline int br_is_designated_port(const struct net_bridge_port *p)
{
	return !memcmp(&p->designated_bridge, &p->br->bridge_id, 8) &&
		(p->designated_port == p->port_id);
}


/* br_stp.c */
extern void br_become_root_bridge(struct net_bridge *br);
extern void br_config_bpdu_generation(struct net_bridge *);
extern void br_configuration_update(struct net_bridge *);
extern void br_port_state_selection(struct net_bridge *);
extern void br_received_config_bpdu(struct net_bridge_port *p, struct br_config_bpdu *bpdu);
extern void br_received_tcn_bpdu(struct net_bridge_port *p);
extern void br_transmit_config(struct net_bridge_port *p);
extern void br_transmit_tcn(struct net_bridge *br);
extern void br_topology_change_detection(struct net_bridge *br);

/* br_stp_bpdu.c */
extern void br_send_config_bpdu(struct net_bridge_port *, struct br_config_bpdu *);
extern void br_send_tcn_bpdu(struct net_bridge_port *);

#endif
