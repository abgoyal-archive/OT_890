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

/* RxRPC point-to-point transport session management
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/net.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <net/af_rxrpc.h>
#include "ar-internal.h"

static void rxrpc_transport_reaper(struct work_struct *work);

static LIST_HEAD(rxrpc_transports);
static DEFINE_RWLOCK(rxrpc_transport_lock);
static unsigned long rxrpc_transport_timeout = 3600 * 24;
static DECLARE_DELAYED_WORK(rxrpc_transport_reap, rxrpc_transport_reaper);

/*
 * allocate a new transport session manager
 */
static struct rxrpc_transport *rxrpc_alloc_transport(struct rxrpc_local *local,
						     struct rxrpc_peer *peer,
						     gfp_t gfp)
{
	struct rxrpc_transport *trans;

	_enter("");

	trans = kzalloc(sizeof(struct rxrpc_transport), gfp);
	if (trans) {
		trans->local = local;
		trans->peer = peer;
		INIT_LIST_HEAD(&trans->link);
		trans->bundles = RB_ROOT;
		trans->client_conns = RB_ROOT;
		trans->server_conns = RB_ROOT;
		skb_queue_head_init(&trans->error_queue);
		spin_lock_init(&trans->client_lock);
		rwlock_init(&trans->conn_lock);
		atomic_set(&trans->usage, 1);
		trans->debug_id = atomic_inc_return(&rxrpc_debug_id);

		if (peer->srx.transport.family == AF_INET) {
			switch (peer->srx.transport_type) {
			case SOCK_DGRAM:
				INIT_WORK(&trans->error_handler,
					  rxrpc_UDP_error_handler);
				break;
			default:
				BUG();
				break;
			}
		} else {
			BUG();
		}
	}

	_leave(" = %p", trans);
	return trans;
}

/*
 * obtain a transport session for the nominated endpoints
 */
struct rxrpc_transport *rxrpc_get_transport(struct rxrpc_local *local,
					    struct rxrpc_peer *peer,
					    gfp_t gfp)
{
	struct rxrpc_transport *trans, *candidate;
	const char *new = "old";
	int usage;

	_enter("{%pI4+%hu},{%pI4+%hu},",
	       &local->srx.transport.sin.sin_addr,
	       ntohs(local->srx.transport.sin.sin_port),
	       &peer->srx.transport.sin.sin_addr,
	       ntohs(peer->srx.transport.sin.sin_port));

	/* search the transport list first */
	read_lock_bh(&rxrpc_transport_lock);
	list_for_each_entry(trans, &rxrpc_transports, link) {
		if (trans->local == local && trans->peer == peer)
			goto found_extant_transport;
	}
	read_unlock_bh(&rxrpc_transport_lock);

	/* not yet present - create a candidate for a new record and then
	 * redo the search */
	candidate = rxrpc_alloc_transport(local, peer, gfp);
	if (!candidate) {
		_leave(" = -ENOMEM");
		return ERR_PTR(-ENOMEM);
	}

	write_lock_bh(&rxrpc_transport_lock);

	list_for_each_entry(trans, &rxrpc_transports, link) {
		if (trans->local == local && trans->peer == peer)
			goto found_extant_second;
	}

	/* we can now add the new candidate to the list */
	trans = candidate;
	candidate = NULL;

	rxrpc_get_local(trans->local);
	atomic_inc(&trans->peer->usage);
	list_add_tail(&trans->link, &rxrpc_transports);
	write_unlock_bh(&rxrpc_transport_lock);
	new = "new";

success:
	_net("TRANSPORT %s %d local %d -> peer %d",
	     new,
	     trans->debug_id,
	     trans->local->debug_id,
	     trans->peer->debug_id);

	_leave(" = %p {u=%d}", trans, atomic_read(&trans->usage));
	return trans;

	/* we found the transport in the list immediately */
found_extant_transport:
	usage = atomic_inc_return(&trans->usage);
	read_unlock_bh(&rxrpc_transport_lock);
	goto success;

	/* we found the transport on the second time through the list */
found_extant_second:
	usage = atomic_inc_return(&trans->usage);
	write_unlock_bh(&rxrpc_transport_lock);
	kfree(candidate);
	goto success;
}

/*
 * find the transport connecting two endpoints
 */
struct rxrpc_transport *rxrpc_find_transport(struct rxrpc_local *local,
					     struct rxrpc_peer *peer)
{
	struct rxrpc_transport *trans;

	_enter("{%pI4+%hu},{%pI4+%hu},",
	       &local->srx.transport.sin.sin_addr,
	       ntohs(local->srx.transport.sin.sin_port),
	       &peer->srx.transport.sin.sin_addr,
	       ntohs(peer->srx.transport.sin.sin_port));

	/* search the transport list */
	read_lock_bh(&rxrpc_transport_lock);

	list_for_each_entry(trans, &rxrpc_transports, link) {
		if (trans->local == local && trans->peer == peer)
			goto found_extant_transport;
	}

	read_unlock_bh(&rxrpc_transport_lock);
	_leave(" = NULL");
	return NULL;

found_extant_transport:
	atomic_inc(&trans->usage);
	read_unlock_bh(&rxrpc_transport_lock);
	_leave(" = %p", trans);
	return trans;
}

/*
 * release a transport session
 */
void rxrpc_put_transport(struct rxrpc_transport *trans)
{
	_enter("%p{u=%d}", trans, atomic_read(&trans->usage));

	ASSERTCMP(atomic_read(&trans->usage), >, 0);

	trans->put_time = get_seconds();
	if (unlikely(atomic_dec_and_test(&trans->usage))) {
		_debug("zombie");
		/* let the reaper determine the timeout to avoid a race with
		 * overextending the timeout if the reaper is running at the
		 * same time */
		rxrpc_queue_delayed_work(&rxrpc_transport_reap, 0);
	}
	_leave("");
}

/*
 * clean up a transport session
 */
static void rxrpc_cleanup_transport(struct rxrpc_transport *trans)
{
	_net("DESTROY TRANS %d", trans->debug_id);

	rxrpc_purge_queue(&trans->error_queue);

	rxrpc_put_local(trans->local);
	rxrpc_put_peer(trans->peer);
	kfree(trans);
}

/*
 * reap dead transports that have passed their expiry date
 */
static void rxrpc_transport_reaper(struct work_struct *work)
{
	struct rxrpc_transport *trans, *_p;
	unsigned long now, earliest, reap_time;

	LIST_HEAD(graveyard);

	_enter("");

	now = get_seconds();
	earliest = ULONG_MAX;

	/* extract all the transports that have been dead too long */
	write_lock_bh(&rxrpc_transport_lock);
	list_for_each_entry_safe(trans, _p, &rxrpc_transports, link) {
		_debug("reap TRANS %d { u=%d t=%ld }",
		       trans->debug_id, atomic_read(&trans->usage),
		       (long) now - (long) trans->put_time);

		if (likely(atomic_read(&trans->usage) > 0))
			continue;

		reap_time = trans->put_time + rxrpc_transport_timeout;
		if (reap_time <= now)
			list_move_tail(&trans->link, &graveyard);
		else if (reap_time < earliest)
			earliest = reap_time;
	}
	write_unlock_bh(&rxrpc_transport_lock);

	if (earliest != ULONG_MAX) {
		_debug("reschedule reaper %ld", (long) earliest - now);
		ASSERTCMP(earliest, >, now);
		rxrpc_queue_delayed_work(&rxrpc_transport_reap,
					 (earliest - now) * HZ);
	}

	/* then destroy all those pulled out */
	while (!list_empty(&graveyard)) {
		trans = list_entry(graveyard.next, struct rxrpc_transport,
				   link);
		list_del_init(&trans->link);

		ASSERTCMP(atomic_read(&trans->usage), ==, 0);
		rxrpc_cleanup_transport(trans);
	}

	_leave("");
}

/*
 * preemptively destroy all the transport session records rather than waiting
 * for them to time out
 */
void __exit rxrpc_destroy_all_transports(void)
{
	_enter("");

	rxrpc_transport_timeout = 0;
	cancel_delayed_work(&rxrpc_transport_reap);
	rxrpc_queue_delayed_work(&rxrpc_transport_reap, 0);

	_leave("");
}
