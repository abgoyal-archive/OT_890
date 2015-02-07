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
 * Filename:      timer.h
 * Version:       
 * Description:   
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Sat Aug 16 00:59:29 1997
 * Modified at:   Thu Oct  7 12:25:24 1999
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 * 
 *     Copyright (c) 1997, 1998-1999 Dag Brattli <dagb@cs.uit.no>, 
 *     All Rights Reserved.
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

#ifndef TIMER_H
#define TIMER_H

#include <linux/timer.h>
#include <linux/jiffies.h>

#include <asm/param.h>  /* for HZ */

#include <net/irda/irda.h>

/* A few forward declarations (to make compiler happy) */
struct irlmp_cb;
struct irlap_cb;
struct lsap_cb;
struct lap_cb;

/* 
 *  Timeout definitions, some defined in IrLAP 6.13.5 - p. 92
 */
#define POLL_TIMEOUT        (450*HZ/1000)    /* Must never exceed 500 ms */
#define FINAL_TIMEOUT       (500*HZ/1000)    /* Must never exceed 500 ms */

/* 
 *  Normally twice of p-timer. Note 3, IrLAP 6.3.11.2 - p. 60 suggests
 *  at least twice duration of the P-timer.
 */
#define WD_TIMEOUT          (POLL_TIMEOUT*2)

#define MEDIABUSY_TIMEOUT   (500*HZ/1000)    /* 500 msec */
#define SMALLBUSY_TIMEOUT   (100*HZ/1000)    /* 100 msec - IrLAP 6.13.4 */

/*
 *  Slot timer must never exceed 85 ms, and must always be at least 25 ms, 
 *  suggested to  75-85 msec by IrDA lite. This doesn't work with a lot of
 *  devices, and other stackes uses a lot more, so it's best we do it as well
 *  (Note : this is the default value and sysctl overides it - Jean II)
 */
#define SLOT_TIMEOUT            (90*HZ/1000)

/* 
 *  The latest discovery frame (XID) is longer due to the extra discovery
 *  information (hints, device name...). This is its extra length.
 *  We use that when setting the query timeout. Jean II
 */
#define XIDEXTRA_TIMEOUT        (34*HZ/1000)  /* 34 msec */

#define WATCHDOG_TIMEOUT        (20*HZ)       /* 20 sec */

typedef void (*TIMER_CALLBACK)(void *);

static inline void irda_start_timer(struct timer_list *ptimer, int timeout, 
				    void* data, TIMER_CALLBACK callback)
{
	ptimer->function = (void (*)(unsigned long)) callback;
	ptimer->data = (unsigned long) data;
	
	/* Set new value for timer (update or add timer).
	 * We use mod_timer() because it's more efficient and also
	 * safer with respect to race conditions - Jean II */
	mod_timer(ptimer, jiffies + timeout);
}


void irlap_start_slot_timer(struct irlap_cb *self, int timeout);
void irlap_start_query_timer(struct irlap_cb *self, int S, int s);
void irlap_start_final_timer(struct irlap_cb *self, int timeout);
void irlap_start_wd_timer(struct irlap_cb *self, int timeout);
void irlap_start_backoff_timer(struct irlap_cb *self, int timeout);

void irlap_start_mbusy_timer(struct irlap_cb *self, int timeout);
void irlap_stop_mbusy_timer(struct irlap_cb *);

void irlmp_start_watchdog_timer(struct lsap_cb *, int timeout);
void irlmp_start_discovery_timer(struct irlmp_cb *, int timeout);
void irlmp_start_idle_timer(struct lap_cb *, int timeout);
void irlmp_stop_idle_timer(struct lap_cb *self);

#endif

