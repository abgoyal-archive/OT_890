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
 *	LAPB release 002
 *
 *	This code REQUIRES 2.1.15 or higher/ NET3.038
 *
 *	This module:
 *		This module is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 *	History
 *	LAPB 001	Jonathan Naylor	Started Coding
 *	LAPB 002	Jonathan Naylor	New timer architecture.
 */

#include <linux/errno.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/string.h>
#include <linux/sockios.h>
#include <linux/net.h>
#include <linux/inet.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <net/lapb.h>

static void lapb_t1timer_expiry(unsigned long);
static void lapb_t2timer_expiry(unsigned long);

void lapb_start_t1timer(struct lapb_cb *lapb)
{
	del_timer(&lapb->t1timer);

	lapb->t1timer.data     = (unsigned long)lapb;
	lapb->t1timer.function = &lapb_t1timer_expiry;
	lapb->t1timer.expires  = jiffies + lapb->t1;

	add_timer(&lapb->t1timer);
}

void lapb_start_t2timer(struct lapb_cb *lapb)
{
	del_timer(&lapb->t2timer);

	lapb->t2timer.data     = (unsigned long)lapb;
	lapb->t2timer.function = &lapb_t2timer_expiry;
	lapb->t2timer.expires  = jiffies + lapb->t2;

	add_timer(&lapb->t2timer);
}

void lapb_stop_t1timer(struct lapb_cb *lapb)
{
	del_timer(&lapb->t1timer);
}

void lapb_stop_t2timer(struct lapb_cb *lapb)
{
	del_timer(&lapb->t2timer);
}

int lapb_t1timer_running(struct lapb_cb *lapb)
{
	return timer_pending(&lapb->t1timer);
}

static void lapb_t2timer_expiry(unsigned long param)
{
	struct lapb_cb *lapb = (struct lapb_cb *)param;

	if (lapb->condition & LAPB_ACK_PENDING_CONDITION) {
		lapb->condition &= ~LAPB_ACK_PENDING_CONDITION;
		lapb_timeout_response(lapb);
	}
}

static void lapb_t1timer_expiry(unsigned long param)
{
	struct lapb_cb *lapb = (struct lapb_cb *)param;

	switch (lapb->state) {

		/*
		 *	If we are a DCE, keep going DM .. DM .. DM
		 */
		case LAPB_STATE_0:
			if (lapb->mode & LAPB_DCE)
				lapb_send_control(lapb, LAPB_DM, LAPB_POLLOFF, LAPB_RESPONSE);
			break;

		/*
		 *	Awaiting connection state, send SABM(E), up to N2 times.
		 */
		case LAPB_STATE_1:
			if (lapb->n2count == lapb->n2) {
				lapb_clear_queues(lapb);
				lapb->state = LAPB_STATE_0;
				lapb_disconnect_indication(lapb, LAPB_TIMEDOUT);
#if LAPB_DEBUG > 0
				printk(KERN_DEBUG "lapb: (%p) S1 -> S0\n", lapb->dev);
#endif
				return;
			} else {
				lapb->n2count++;
				if (lapb->mode & LAPB_EXTENDED) {
#if LAPB_DEBUG > 1
					printk(KERN_DEBUG "lapb: (%p) S1 TX SABME(1)\n", lapb->dev);
#endif
					lapb_send_control(lapb, LAPB_SABME, LAPB_POLLON, LAPB_COMMAND);
				} else {
#if LAPB_DEBUG > 1
					printk(KERN_DEBUG "lapb: (%p) S1 TX SABM(1)\n", lapb->dev);
#endif
					lapb_send_control(lapb, LAPB_SABM, LAPB_POLLON, LAPB_COMMAND);
				}
			}
			break;

		/*
		 *	Awaiting disconnection state, send DISC, up to N2 times.
		 */
		case LAPB_STATE_2:
			if (lapb->n2count == lapb->n2) {
				lapb_clear_queues(lapb);
				lapb->state = LAPB_STATE_0;
				lapb_disconnect_confirmation(lapb, LAPB_TIMEDOUT);
#if LAPB_DEBUG > 0
				printk(KERN_DEBUG "lapb: (%p) S2 -> S0\n", lapb->dev);
#endif
				return;
			} else {
				lapb->n2count++;
#if LAPB_DEBUG > 1
				printk(KERN_DEBUG "lapb: (%p) S2 TX DISC(1)\n", lapb->dev);
#endif
				lapb_send_control(lapb, LAPB_DISC, LAPB_POLLON, LAPB_COMMAND);
			}
			break;

		/*
		 *	Data transfer state, restransmit I frames, up to N2 times.
		 */
		case LAPB_STATE_3:
			if (lapb->n2count == lapb->n2) {
				lapb_clear_queues(lapb);
				lapb->state = LAPB_STATE_0;
				lapb_stop_t2timer(lapb);
				lapb_disconnect_indication(lapb, LAPB_TIMEDOUT);
#if LAPB_DEBUG > 0
				printk(KERN_DEBUG "lapb: (%p) S3 -> S0\n", lapb->dev);
#endif
				return;
			} else {
				lapb->n2count++;
				lapb_requeue_frames(lapb);
			}
			break;

		/*
		 *	Frame reject state, restransmit FRMR frames, up to N2 times.
		 */
		case LAPB_STATE_4:
			if (lapb->n2count == lapb->n2) {
				lapb_clear_queues(lapb);
				lapb->state = LAPB_STATE_0;
				lapb_disconnect_indication(lapb, LAPB_TIMEDOUT);
#if LAPB_DEBUG > 0
				printk(KERN_DEBUG "lapb: (%p) S4 -> S0\n", lapb->dev);
#endif
				return;
			} else {
				lapb->n2count++;
				lapb_transmit_frmr(lapb);
			}
			break;
	}

	lapb_start_t1timer(lapb);
}
