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
 * Serial Attached SCSI (SAS) Event processing
 *
 * Copyright (C) 2005 Adaptec, Inc.  All rights reserved.
 * Copyright (C) 2005 Luben Tuikov <luben_tuikov@adaptec.com>
 *
 * This file is licensed under GPLv2.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <scsi/scsi_host.h>
#include "sas_internal.h"
#include "sas_dump.h"

static void notify_ha_event(struct sas_ha_struct *sas_ha, enum ha_event event)
{
	BUG_ON(event >= HA_NUM_EVENTS);

	sas_queue_event(event, &sas_ha->event_lock, &sas_ha->pending,
			&sas_ha->ha_events[event].work, sas_ha);
}

static void notify_port_event(struct asd_sas_phy *phy, enum port_event event)
{
	struct sas_ha_struct *ha = phy->ha;

	BUG_ON(event >= PORT_NUM_EVENTS);

	sas_queue_event(event, &ha->event_lock, &phy->port_events_pending,
			&phy->port_events[event].work, ha);
}

static void notify_phy_event(struct asd_sas_phy *phy, enum phy_event event)
{
	struct sas_ha_struct *ha = phy->ha;

	BUG_ON(event >= PHY_NUM_EVENTS);

	sas_queue_event(event, &ha->event_lock, &phy->phy_events_pending,
			&phy->phy_events[event].work, ha);
}

int sas_init_events(struct sas_ha_struct *sas_ha)
{
	static const work_func_t sas_ha_event_fns[HA_NUM_EVENTS] = {
		[HAE_RESET] = sas_hae_reset,
	};

	int i;

	spin_lock_init(&sas_ha->event_lock);

	for (i = 0; i < HA_NUM_EVENTS; i++) {
		INIT_WORK(&sas_ha->ha_events[i].work, sas_ha_event_fns[i]);
		sas_ha->ha_events[i].ha = sas_ha;
	}

	sas_ha->notify_ha_event = notify_ha_event;
	sas_ha->notify_port_event = notify_port_event;
	sas_ha->notify_phy_event = notify_phy_event;

	return 0;
}
