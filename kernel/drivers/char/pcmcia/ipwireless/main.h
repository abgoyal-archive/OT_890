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

#ifndef _IPWIRELESS_CS_H_
#define _IPWIRELESS_CS_H_

#include <linux/sched.h>
#include <linux/types.h>

#include <pcmcia/cs_types.h>
#include <pcmcia/cs.h>
#include <pcmcia/cistpl.h>
#include <pcmcia/ds.h>

#include "hardware.h"

#define IPWIRELESS_PCCARD_NAME		"ipwireless"
#define IPWIRELESS_PCMCIA_VERSION	"1.1"
#define IPWIRELESS_PCMCIA_AUTHOR        \
	"Stephen Blackheath, Ben Martel, Jiri Kosina and David Sterba"

#define IPWIRELESS_TX_QUEUE_SIZE  262144
#define IPWIRELESS_RX_QUEUE_SIZE  262144

#define IPWIRELESS_STATE_DEBUG

struct ipw_hardware;
struct ipw_network;
struct ipw_tty;

struct ipw_dev {
	struct pcmcia_device *link;
	int is_v2_card;

	window_handle_t handle_attr_memory;
	void __iomem *attr_memory;
	win_req_t request_attr_memory;

	window_handle_t handle_common_memory;
	void __iomem *common_memory;
	win_req_t request_common_memory;

	dev_node_t nodes[2];
	/* Reference to attribute memory, containing CIS data */
	void *attribute_memory;

	/* Hardware context */
	struct ipw_hardware *hardware;
	/* Network layer context */
	struct ipw_network *network;
	/* TTY device context */
	struct ipw_tty *tty;
	struct work_struct work_reboot;
};

/* Module parametres */
extern int ipwireless_debug;
extern int ipwireless_loopback;
extern int ipwireless_out_queue;

#endif
