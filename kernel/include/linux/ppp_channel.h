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

#ifndef _PPP_CHANNEL_H_
#define _PPP_CHANNEL_H_
/*
 * Definitions for the interface between the generic PPP code
 * and a PPP channel.
 *
 * A PPP channel provides a way for the generic PPP code to send
 * and receive packets over some sort of communications medium.
 * Packets are stored in sk_buffs and have the 2-byte PPP protocol
 * number at the start, but not the address and control bytes.
 *
 * Copyright 1999 Paul Mackerras.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 *
 * ==FILEVERSION 20000322==
 */

#include <linux/list.h>
#include <linux/skbuff.h>
#include <linux/poll.h>

struct ppp_channel;

struct ppp_channel_ops {
	/* Send a packet (or multilink fragment) on this channel.
	   Returns 1 if it was accepted, 0 if not. */
	int	(*start_xmit)(struct ppp_channel *, struct sk_buff *);
	/* Handle an ioctl call that has come in via /dev/ppp. */
	int	(*ioctl)(struct ppp_channel *, unsigned int, unsigned long);
};

struct ppp_channel {
	void		*private;	/* channel private data */
	struct ppp_channel_ops *ops;	/* operations for this channel */
	int		mtu;		/* max transmit packet size */
	int		hdrlen;		/* amount of headroom channel needs */
	void		*ppp;		/* opaque to channel */
	/* the following are not used at present */
	int		speed;		/* transfer rate (bytes/second) */
	int		latency;	/* overhead time in milliseconds */
};

#ifdef __KERNEL__
/* Called by the channel when it can send some more data. */
extern void ppp_output_wakeup(struct ppp_channel *);

/* Called by the channel to process a received PPP packet.
   The packet should have just the 2-byte PPP protocol header. */
extern void ppp_input(struct ppp_channel *, struct sk_buff *);

/* Called by the channel when an input error occurs, indicating
   that we may have missed a packet. */
extern void ppp_input_error(struct ppp_channel *, int code);

/* Attach a channel to a given PPP unit. */
extern int ppp_register_channel(struct ppp_channel *);

/* Detach a channel from its PPP unit (e.g. on hangup). */
extern void ppp_unregister_channel(struct ppp_channel *);

/* Get the channel number for a channel */
extern int ppp_channel_index(struct ppp_channel *);

/* Get the unit number associated with a channel, or -1 if none */
extern int ppp_unit_number(struct ppp_channel *);

/*
 * SMP locking notes:
 * The channel code must ensure that when it calls ppp_unregister_channel,
 * nothing is executing in any of the procedures above, for that
 * channel.  The generic layer will ensure that nothing is executing
 * in the start_xmit and ioctl routines for the channel by the time
 * that ppp_unregister_channel returns.
 */

#endif /* __KERNEL__ */
#endif
