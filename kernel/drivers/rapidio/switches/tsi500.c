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
 * RapidIO Tsi500 switch support
 *
 * Copyright 2005 MontaVista Software, Inc.
 * Matt Porter <mporter@kernel.crashing.org>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/rio.h>
#include <linux/rio_drv.h>
#include <linux/rio_ids.h>
#include "../rio.h"

static int
tsi500_route_add_entry(struct rio_mport *mport, u16 destid, u8 hopcount, u16 table, u16 route_destid, u8 route_port)
{
	int i;
	u32 offset = 0x10000 + 0xa00 + ((route_destid / 2)&~0x3);
	u32 result;

	if (table == 0xff) {
		rio_mport_read_config_32(mport, destid, hopcount, offset, &result);
		result &= ~(0xf << (4*(route_destid & 0x7)));
		for (i=0;i<4;i++)
			rio_mport_write_config_32(mport, destid, hopcount, offset + (0x20000*i), result | (route_port << (4*(route_destid & 0x7))));
	}
	else {
		rio_mport_read_config_32(mport, destid, hopcount, offset + (0x20000*table), &result);
		result &= ~(0xf << (4*(route_destid & 0x7)));
		rio_mport_write_config_32(mport, destid, hopcount, offset + (0x20000*table), result | (route_port << (4*(route_destid & 0x7))));
	}

	return 0;
}

static int
tsi500_route_get_entry(struct rio_mport *mport, u16 destid, u8 hopcount, u16 table, u16 route_destid, u8 *route_port)
{
	int ret = 0;
	u32 offset = 0x10000 + 0xa00 + ((route_destid / 2)&~0x3);
	u32 result;

	if (table == 0xff)
		rio_mport_read_config_32(mport, destid, hopcount, offset, &result);
	else
		rio_mport_read_config_32(mport, destid, hopcount, offset + (0x20000*table), &result);

	result &= 0xf << (4*(route_destid & 0x7));
	*route_port = result >> (4*(route_destid & 0x7));
	if (*route_port > 3)
		ret = -1;

	return ret;
}

DECLARE_RIO_ROUTE_OPS(RIO_VID_TUNDRA, RIO_DID_TSI500, tsi500_route_add_entry, tsi500_route_get_entry);
