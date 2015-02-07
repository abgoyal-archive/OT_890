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

/* $Id$
 *
 * Ansteuerung ARCOFI 2165
 *
 * Author       Karsten Keil
 * Copyright    by Karsten Keil      <keil@isdn4linux.de>
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */
 
#include "hisax.h"
#include "isdnl1.h"
#include "isac.h"
#include "arcofi.h"

#define ARCOFI_TIMER_VALUE	20

static void
add_arcofi_timer(struct IsdnCardState *cs) {
	if (test_and_set_bit(FLG_ARCOFI_TIMER, &cs->HW_Flags)) {
		del_timer(&cs->dc.isac.arcofitimer);
	}	
	init_timer(&cs->dc.isac.arcofitimer);
	cs->dc.isac.arcofitimer.expires = jiffies + ((ARCOFI_TIMER_VALUE * HZ)/1000);
	add_timer(&cs->dc.isac.arcofitimer);
}

static void
send_arcofi(struct IsdnCardState *cs) {
	u_char val;
	
	add_arcofi_timer(cs);
	cs->dc.isac.mon_txp = 0;
	cs->dc.isac.mon_txc = cs->dc.isac.arcofi_list->len;
	memcpy(cs->dc.isac.mon_tx, cs->dc.isac.arcofi_list->msg, cs->dc.isac.mon_txc);
	switch(cs->dc.isac.arcofi_bc) {
		case 0: break;
		case 1: cs->dc.isac.mon_tx[1] |= 0x40;
			break;
		default: break;
	}
	cs->dc.isac.mocr &= 0x0f;
	cs->dc.isac.mocr |= 0xa0;
	cs->writeisac(cs, ISAC_MOCR, cs->dc.isac.mocr);
	val = cs->readisac(cs, ISAC_MOSR);
	cs->writeisac(cs, ISAC_MOX1, cs->dc.isac.mon_tx[cs->dc.isac.mon_txp++]);
	cs->dc.isac.mocr |= 0x10;
	cs->writeisac(cs, ISAC_MOCR, cs->dc.isac.mocr);
}

int
arcofi_fsm(struct IsdnCardState *cs, int event, void *data) {
	if (cs->debug & L1_DEB_MONITOR) {
		debugl1(cs, "arcofi state %d event %d", cs->dc.isac.arcofi_state, event);
	}
	if (event == ARCOFI_TIMEOUT) {
		cs->dc.isac.arcofi_state = ARCOFI_NOP;
		test_and_set_bit(FLG_ARCOFI_ERROR, &cs->HW_Flags);
		wake_up(&cs->dc.isac.arcofi_wait);
 		return(1);
	}
	switch (cs->dc.isac.arcofi_state) {
		case ARCOFI_NOP:
			if (event == ARCOFI_START) {
				cs->dc.isac.arcofi_list = data;
				cs->dc.isac.arcofi_state = ARCOFI_TRANSMIT;
				send_arcofi(cs);
			}
			break;
		case ARCOFI_TRANSMIT:
			if (event == ARCOFI_TX_END) {
				if (cs->dc.isac.arcofi_list->receive) {
					add_arcofi_timer(cs);
					cs->dc.isac.arcofi_state = ARCOFI_RECEIVE;
				} else {
					if (cs->dc.isac.arcofi_list->next) {
						cs->dc.isac.arcofi_list =
							cs->dc.isac.arcofi_list->next;
						send_arcofi(cs);
					} else {
						if (test_and_clear_bit(FLG_ARCOFI_TIMER, &cs->HW_Flags)) {
							del_timer(&cs->dc.isac.arcofitimer);
						}
						cs->dc.isac.arcofi_state = ARCOFI_NOP;
						wake_up(&cs->dc.isac.arcofi_wait);
					}
				}
			}
			break;
		case ARCOFI_RECEIVE:
			if (event == ARCOFI_RX_END) {
				if (cs->dc.isac.arcofi_list->next) {
					cs->dc.isac.arcofi_list =
						cs->dc.isac.arcofi_list->next;
					cs->dc.isac.arcofi_state = ARCOFI_TRANSMIT;
					send_arcofi(cs);
				} else {
					if (test_and_clear_bit(FLG_ARCOFI_TIMER, &cs->HW_Flags)) {
						del_timer(&cs->dc.isac.arcofitimer);
					}
					cs->dc.isac.arcofi_state = ARCOFI_NOP;
					wake_up(&cs->dc.isac.arcofi_wait);
				}
			}
			break;
		default:
			debugl1(cs, "Arcofi unknown state %x", cs->dc.isac.arcofi_state);
			return(2);
	}
	return(0);
}

static void
arcofi_timer(struct IsdnCardState *cs) {
	arcofi_fsm(cs, ARCOFI_TIMEOUT, NULL);
}

void
clear_arcofi(struct IsdnCardState *cs) {
	if (test_and_clear_bit(FLG_ARCOFI_TIMER, &cs->HW_Flags)) {
		del_timer(&cs->dc.isac.arcofitimer);
	}
}

void
init_arcofi(struct IsdnCardState *cs) {
	cs->dc.isac.arcofitimer.function = (void *) arcofi_timer;
	cs->dc.isac.arcofitimer.data = (long) cs;
	init_timer(&cs->dc.isac.arcofitimer);
	init_waitqueue_head(&cs->dc.isac.arcofi_wait);
	test_and_set_bit(HW_ARCOFI, &cs->HW_Flags);
}
