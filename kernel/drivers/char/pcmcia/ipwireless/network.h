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
 * IPWireless 3G PCMCIA Network Driver
 *
 * Original code
 *   by Stephen Blackheath <stephen@blacksapphire.com>,
 *      Ben Martel <benm@symmetric.co.nz>
 *
 * Copyrighted as follows:
 *   Copyright (C) 2004 by Symmetric Systems Ltd (NZ)
 *
 * Various driver changes and rewrites, port to new kernels
 *   Copyright (C) 2006-2007 Jiri Kosina
 *
 * Misc code cleanups and updates
 *   Copyright (C) 2007 David Sterba
 */

#ifndef _IPWIRELESS_CS_NETWORK_H_
#define _IPWIRELESS_CS_NETWORK_H_

#include <linux/types.h>

struct ipw_network;
struct ipw_tty;
struct ipw_hardware;

/* Definitions of the different channels on the PCMCIA UE */
#define IPW_CHANNEL_RAS      0
#define IPW_CHANNEL_DIALLER  1
#define IPW_CHANNEL_CONSOLE  2
#define NO_OF_IPW_CHANNELS   5

void ipwireless_network_notify_control_line_change(struct ipw_network *net,
		unsigned int channel_idx, unsigned int control_lines,
		unsigned int control_mask);
void ipwireless_network_packet_received(struct ipw_network *net,
		unsigned int channel_idx, unsigned char *data,
		unsigned int length);
struct ipw_network *ipwireless_network_create(struct ipw_hardware *hw);
void ipwireless_network_free(struct ipw_network *net);
void ipwireless_associate_network_tty(struct ipw_network *net,
		unsigned int channel_idx, struct ipw_tty *tty);
void ipwireless_disassociate_network_ttys(struct ipw_network *net,
		unsigned int channel_idx);

void ipwireless_ppp_open(struct ipw_network *net);

void ipwireless_ppp_close(struct ipw_network *net);
int ipwireless_ppp_channel_index(struct ipw_network *net);
int ipwireless_ppp_unit_number(struct ipw_network *net);
int ipwireless_ppp_mru(const struct ipw_network *net);

#endif
