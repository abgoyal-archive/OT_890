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
 * Copyright (C) 1996  SpellCaster Telecommunications Inc.
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 * For more information, please contact gpl-info@spellcast.com or write:
 *
 *     SpellCaster Telecommunications Inc.
 *     5621 Finch Avenue East, Unit #3
 *     Scarborough, Ontario  Canada
 *     M1B 2T9
 *     +1 (416) 297-8565
 *     +1 (416) 297-6433 Facsimile
 */

#include "includes.h"
#include "hardware.h"
#include "message.h"
#include "card.h"

#ifdef DEBUG
static char *events[] = { "ISDN_STAT_STAVAIL",
			  "ISDN_STAT_ICALL",
			  "ISDN_STAT_RUN",
			  "ISDN_STAT_STOP",
			  "ISDN_STAT_DCONN",
			  "ISDN_STAT_BCONN",
			  "ISDN_STAT_DHUP",
			  "ISDN_STAT_BHUP",
			  "ISDN_STAT_CINF",
			  "ISDN_STAT_LOAD",
			  "ISDN_STAT_UNLOAD",
			  "ISDN_STAT_BSENT",
			  "ISDN_STAT_NODCH",
			  "ISDN_STAT_ADDCH",
			  "ISDN_STAT_CAUSE" };
#endif

int indicate_status(int card, int event,ulong Channel,char *Data)
{
	isdn_ctrl cmd;

#ifdef DEBUG
	pr_debug("%s: Indicating event %s on Channel %d\n",
		sc_adapter[card]->devicename, events[event-256], Channel);
#endif
	if (Data != NULL){
		pr_debug("%s: Event data: %s\n", sc_adapter[card]->devicename,
			Data);
		switch (event) {
			case ISDN_STAT_BSENT:
				memcpy(&cmd.parm.length, Data, sizeof(cmd.parm.length));
				break;
			case ISDN_STAT_ICALL:
				memcpy(&cmd.parm.setup, Data, sizeof(cmd.parm.setup));
				break;
			default:
				strcpy(cmd.parm.num, Data);
		}
	}

	cmd.command = event;
	cmd.driver = sc_adapter[card]->driverId;
	cmd.arg = Channel;
	return sc_adapter[card]->card->statcallb(&cmd);
}
