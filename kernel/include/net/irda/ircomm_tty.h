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

/*********************************************************************
 *                
 * Filename:      ircomm_tty.h
 * Version:       
 * Description:   
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Sun Jun  6 23:24:22 1999
 * Modified at:   Fri Jan 28 13:16:57 2000
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 * 
 *     Copyright (c) 1999-2000 Dag Brattli, All Rights Reserved.
 *     
 *     This program is free software; you can redistribute it and/or 
 *     modify it under the terms of the GNU General Public License as 
 *     published by the Free Software Foundation; either version 2 of 
 *     the License, or (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License 
 *     along with this program; if not, write to the Free Software 
 *     Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
 *     MA 02111-1307 USA
 *     
 ********************************************************************/

#ifndef IRCOMM_TTY_H
#define IRCOMM_TTY_H

#include <linux/serial.h>
#include <linux/termios.h>
#include <linux/timer.h>
#include <linux/tty.h>		/* struct tty_struct */

#include <net/irda/irias_object.h>
#include <net/irda/ircomm_core.h>
#include <net/irda/ircomm_param.h>

#define IRCOMM_TTY_PORTS 32
#define IRCOMM_TTY_MAGIC 0x3432
#define IRCOMM_TTY_MAJOR 161
#define IRCOMM_TTY_MINOR 0

/* This is used as an initial value to max_header_size before the proper
 * value is filled in (5 for ttp, 4 for lmp). This allow us to detect
 * the state of the underlying connection. - Jean II */
#define IRCOMM_TTY_HDR_UNINITIALISED	16
/* Same for payload size. See qos.c for the smallest max data size */
#define IRCOMM_TTY_DATA_UNINITIALISED	(64 - IRCOMM_TTY_HDR_UNINITIALISED)

/* Those are really defined in include/linux/serial.h - Jean II */
#define ASYNC_B_INITIALIZED	31	/* Serial port was initialized */
#define ASYNC_B_NORMAL_ACTIVE	29	/* Normal device is active */
#define ASYNC_B_CLOSING		27	/* Serial port is closing */

/*
 * IrCOMM TTY driver state
 */
struct ircomm_tty_cb {
	irda_queue_t queue;            /* Must be first */
	magic_t magic;

	int state;                /* Connect state */

	struct tty_struct *tty;
	struct ircomm_cb *ircomm; /* IrCOMM layer instance */

	struct sk_buff *tx_skb;   /* Transmit buffer */
	struct sk_buff *ctrl_skb; /* Control data buffer */

	/* Parameters */
	struct ircomm_params settings;

	__u8 service_type;        /* The service that we support */
	int client;               /* True if we are a client */
	LOCAL_FLOW flow;          /* IrTTP flow status */

	int line;
	unsigned long flags;

	__u8 dlsap_sel;
	__u8 slsap_sel;

	__u32 saddr;
	__u32 daddr;

	__u32 max_data_size;   /* Max data we can transmit in one packet */
	__u32 max_header_size; /* The amount of header space we must reserve */
	__u32 tx_data_size;	/* Max data size of current tx_skb */

	struct iriap_cb *iriap; /* Instance used for querying remote IAS */
	struct ias_object* obj;
	void *skey;
	void *ckey;

	wait_queue_head_t open_wait;
	wait_queue_head_t close_wait;
	struct timer_list watchdog_timer;
	struct work_struct  tqueue;

        unsigned short    close_delay;
        unsigned short    closing_wait; /* time to wait before closing */

	int  open_count;
	int  blocked_open;	/* # of blocked opens */

	/* Protect concurent access to :
	 *	o self->open_count
	 *	o self->ctrl_skb
	 *	o self->tx_skb
	 * Maybe other things may gain to be protected as well...
	 * Jean II */
	spinlock_t spinlock;
};

void ircomm_tty_start(struct tty_struct *tty);
void ircomm_tty_check_modem_status(struct ircomm_tty_cb *self);

extern int ircomm_tty_tiocmget(struct tty_struct *tty, struct file *file);
extern int ircomm_tty_tiocmset(struct tty_struct *tty, struct file *file,
			       unsigned int set, unsigned int clear);
extern int ircomm_tty_ioctl(struct tty_struct *tty, struct file *file, 
			    unsigned int cmd, unsigned long arg);
extern void ircomm_tty_set_termios(struct tty_struct *tty, 
				   struct ktermios *old_termios);

#endif







