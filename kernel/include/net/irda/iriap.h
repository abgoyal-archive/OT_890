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
 * Filename:      iriap.h
 * Version:       0.5
 * Description:   Information Access Protocol (IAP)
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Thu Aug 21 00:02:07 1997
 * Modified at:   Sat Dec 25 16:42:09 1999
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 * 
 *     Copyright (c) 1997-1999 Dag Brattli <dagb@cs.uit.no>, 
 *     All Rights Reserved.
 *     
 *     This program is free software; you can redistribute it and/or 
 *     modify it under the terms of the GNU General Public License as 
 *     published by the Free Software Foundation; either version 2 of 
 *     the License, or (at your option) any later version.
 *
 *     Neither Dag Brattli nor University of Tromsø admit liability nor
 *     provide warranty for any of this software. This material is 
 *     provided "AS-IS" and at no charge.
 *
 ********************************************************************/

#ifndef IRIAP_H
#define IRIAP_H

#include <linux/types.h>
#include <linux/skbuff.h>

#include <net/irda/iriap_event.h>
#include <net/irda/irias_object.h>
#include <net/irda/irqueue.h>		/* irda_queue_t */
#include <net/irda/timer.h>		/* struct timer_list */

#define IAP_LST 0x80
#define IAP_ACK 0x40

#define IAS_SERVER 0
#define IAS_CLIENT 1

/* IrIAP Op-codes */
#define GET_INFO_BASE      0x01
#define GET_OBJECTS        0x02
#define GET_VALUE          0x03
#define GET_VALUE_BY_CLASS 0x04
#define GET_OBJECT_INFO    0x05
#define GET_ATTRIB_NAMES   0x06

#define IAS_SUCCESS        0
#define IAS_CLASS_UNKNOWN  1
#define IAS_ATTRIB_UNKNOWN 2
#define IAS_DISCONNECT     10

typedef void (*CONFIRM_CALLBACK)(int result, __u16 obj_id, 
				 struct ias_value *value, void *priv);

struct iriap_cb {
	irda_queue_t q; /* Must be first */	
	magic_t magic;  /* Magic cookie */

	int          mode;   /* Client or server */

	__u32        saddr;
	__u32        daddr;
	__u8         operation;

	struct sk_buff *request_skb;
	struct lsap_cb *lsap;
	__u8 slsap_sel;

	/* Client states */
	IRIAP_STATE client_state;
	IRIAP_STATE call_state;
	
	/* Server states */
	IRIAP_STATE server_state;
	IRIAP_STATE r_connect_state;
	
	CONFIRM_CALLBACK confirm;
	void *priv;                /* Used to identify client */

	__u8 max_header_size;
	__u32 max_data_size;
	
	struct timer_list watchdog_timer;
};

int  iriap_init(void);
void iriap_cleanup(void);

struct iriap_cb *iriap_open(__u8 slsap_sel, int mode, void *priv, 
			    CONFIRM_CALLBACK callback);
void iriap_close(struct iriap_cb *self);

int iriap_getvaluebyclass_request(struct iriap_cb *self, 
				  __u32 saddr, __u32 daddr,
				  char *name, char *attr);
void iriap_connect_request(struct iriap_cb *self);
void iriap_send_ack( struct iriap_cb *self);
void iriap_call_indication(struct iriap_cb *self, struct sk_buff *skb);

void iriap_register_server(void);

#endif


