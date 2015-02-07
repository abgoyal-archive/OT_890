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

#ifndef _IPWIRELESS_CS_HARDWARE_H_
#define _IPWIRELESS_CS_HARDWARE_H_

#include <linux/types.h>
#include <linux/sched.h>
#include <linux/interrupt.h>

#define IPW_CONTROL_LINE_CTS 0x0001
#define IPW_CONTROL_LINE_DCD 0x0002
#define IPW_CONTROL_LINE_DSR 0x0004
#define IPW_CONTROL_LINE_RI  0x0008
#define IPW_CONTROL_LINE_DTR 0x0010
#define IPW_CONTROL_LINE_RTS 0x0020

struct ipw_hardware;
struct ipw_network;

struct ipw_hardware *ipwireless_hardware_create(void);
void ipwireless_hardware_free(struct ipw_hardware *hw);
irqreturn_t ipwireless_interrupt(int irq, void *dev_id);
int ipwireless_set_DTR(struct ipw_hardware *hw, unsigned int channel_idx,
		int state);
int ipwireless_set_RTS(struct ipw_hardware *hw, unsigned int channel_idx,
		int state);
int ipwireless_send_packet(struct ipw_hardware *hw,
			    unsigned int channel_idx,
			    const unsigned char *data,
			    unsigned int length,
			    void (*packet_sent_callback) (void *cb,
							  unsigned int length),
			    void *sent_cb_data);
void ipwireless_associate_network(struct ipw_hardware *hw,
		struct ipw_network *net);
void ipwireless_stop_interrupts(struct ipw_hardware *hw);
void ipwireless_init_hardware_v1(struct ipw_hardware *hw,
				 unsigned int base_port,
				 void __iomem *attr_memory,
				 void __iomem *common_memory,
				 int is_v2_card,
				 void (*reboot_cb) (void *data),
				 void *reboot_cb_data);
void ipwireless_init_hardware_v2_v3(struct ipw_hardware *hw);
void ipwireless_sleep(unsigned int tenths);

#endif
