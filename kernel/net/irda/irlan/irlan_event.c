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
 * Filename:      irlan_event.c
 * Version:
 * Description:
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Tue Oct 20 09:10:16 1998
 * Modified at:   Sat Oct 30 12:59:01 1999
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 *
 *     Copyright (c) 1998-1999 Dag Brattli, All Rights Reserved.
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

#include <net/irda/irlan_event.h>

char *irlan_state[] = {
	"IRLAN_IDLE",
	"IRLAN_QUERY",
	"IRLAN_CONN",
	"IRLAN_INFO",
	"IRLAN_MEDIA",
	"IRLAN_OPEN",
	"IRLAN_WAIT",
	"IRLAN_ARB",
	"IRLAN_DATA",
	"IRLAN_CLOSE",
	"IRLAN_SYNC",
};

void irlan_next_client_state(struct irlan_cb *self, IRLAN_STATE state)
{
	IRDA_DEBUG(2, "%s(), %s\n", __func__ , irlan_state[state]);

	IRDA_ASSERT(self != NULL, return;);
	IRDA_ASSERT(self->magic == IRLAN_MAGIC, return;);

	self->client.state = state;
}

void irlan_next_provider_state(struct irlan_cb *self, IRLAN_STATE state)
{
	IRDA_DEBUG(2, "%s(), %s\n", __func__ , irlan_state[state]);

	IRDA_ASSERT(self != NULL, return;);
	IRDA_ASSERT(self->magic == IRLAN_MAGIC, return;);

	self->provider.state = state;
}

