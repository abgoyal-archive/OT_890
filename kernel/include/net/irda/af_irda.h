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
 * Filename:      af_irda.h
 * Version:       1.0
 * Description:   IrDA sockets declarations
 * Status:        Stable
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Tue Dec  9 21:13:12 1997
 * Modified at:   Fri Jan 28 13:16:32 2000
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 * 
 *     Copyright (c) 1998-2000 Dag Brattli, All Rights Reserved.
 *     Copyright (c) 2000-2002 Jean Tourrilhes <jt@hpl.hp.com>
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

#ifndef AF_IRDA_H
#define AF_IRDA_H

#include <linux/irda.h>
#include <net/irda/irda.h>
#include <net/irda/iriap.h>		/* struct iriap_cb */
#include <net/irda/irias_object.h>	/* struct ias_value */
#include <net/irda/irlmp.h>		/* struct lsap_cb */
#include <net/irda/irttp.h>		/* struct tsap_cb */
#include <net/irda/discovery.h>		/* struct discovery_t */
#include <net/sock.h>

/* IrDA Socket */
struct irda_sock {
	/* struct sock has to be the first member of irda_sock */
	struct sock sk;
	__u32 saddr;          /* my local address */
	__u32 daddr;          /* peer address */

	struct lsap_cb *lsap; /* LSAP used by Ultra */
	__u8  pid;            /* Protocol IP (PID) used by Ultra */

	struct tsap_cb *tsap; /* TSAP used by this connection */
	__u8 dtsap_sel;       /* remote TSAP address */
	__u8 stsap_sel;       /* local TSAP address */
	
	__u32 max_sdu_size_rx;
	__u32 max_sdu_size_tx;
	__u32 max_data_size;
	__u8  max_header_size;
	struct qos_info qos_tx;

	__u16_host_order mask;           /* Hint bits mask */
	__u16_host_order hints;          /* Hint bits */

	void *ckey;           /* IrLMP client handle */
	void *skey;           /* IrLMP service handle */

	struct ias_object *ias_obj;   /* Our service name + lsap in IAS */
	struct iriap_cb *iriap;	      /* Used to query remote IAS */
	struct ias_value *ias_result; /* Result of remote IAS query */

	hashbin_t *cachelog;		/* Result of discovery query */
	__u32 cachedaddr;	/* Result of selective discovery query */

	int nslots;           /* Number of slots to use for discovery */

	int errno;            /* status of the IAS query */

	wait_queue_head_t query_wait;	/* Wait for the answer to a query */
	struct timer_list watchdog;	/* Timeout for discovery */

	LOCAL_FLOW tx_flow;
	LOCAL_FLOW rx_flow;
};

static inline struct irda_sock *irda_sk(struct sock *sk)
{
	return (struct irda_sock *)sk;
}

#endif /* AF_IRDA_H */
