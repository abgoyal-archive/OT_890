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

/* ZD1211 USB-WLAN driver for Linux
 *
 * Copyright (C) 2005-2007 Ulrich Kunitz <kune@deine-taler.de>
 * Copyright (C) 2006-2007 Daniel Drake <dsd@gentoo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/errno.h>
#include <linux/string.h>

#include "zd_def.h"
#include "zd_rf.h"
#include "zd_mac.h"
#include "zd_chip.h"

static const char * const rfs[] = {
	[0]		= "unknown RF0",
	[1]		= "unknown RF1",
	[UW2451_RF]	= "UW2451_RF",
	[UCHIP_RF]	= "UCHIP_RF",
	[AL2230_RF]	= "AL2230_RF",
	[AL7230B_RF]	= "AL7230B_RF",
	[THETA_RF]	= "THETA_RF",
	[AL2210_RF]	= "AL2210_RF",
	[MAXIM_NEW_RF]	= "MAXIM_NEW_RF",
	[UW2453_RF]	= "UW2453_RF",
	[AL2230S_RF]	= "AL2230S_RF",
	[RALINK_RF]	= "RALINK_RF",
	[INTERSIL_RF]	= "INTERSIL_RF",
	[RF2959_RF]	= "RF2959_RF",
	[MAXIM_NEW2_RF]	= "MAXIM_NEW2_RF",
	[PHILIPS_RF]	= "PHILIPS_RF",
};

const char *zd_rf_name(u8 type)
{
	if (type & 0xf0)
		type = 0;
	return rfs[type];
}

void zd_rf_init(struct zd_rf *rf)
{
	memset(rf, 0, sizeof(*rf));

	/* default to update channel integration, as almost all RF's do want
	 * this */
	rf->update_channel_int = 1;
}

void zd_rf_clear(struct zd_rf *rf)
{
	if (rf->clear)
		rf->clear(rf);
	ZD_MEMCLEAR(rf, sizeof(*rf));
}

int zd_rf_init_hw(struct zd_rf *rf, u8 type)
{
	int r = 0;
	int t;
	struct zd_chip *chip = zd_rf_to_chip(rf);

	ZD_ASSERT(mutex_is_locked(&chip->mutex));
	switch (type) {
	case RF2959_RF:
		r = zd_rf_init_rf2959(rf);
		break;
	case AL2230_RF:
	case AL2230S_RF:
		r = zd_rf_init_al2230(rf);
		break;
	case AL7230B_RF:
		r = zd_rf_init_al7230b(rf);
		break;
	case MAXIM_NEW_RF:
	case UW2453_RF:
		r = zd_rf_init_uw2453(rf);
		break;
	default:
		dev_err(zd_chip_dev(chip),
			"RF %s %#x is not supported\n", zd_rf_name(type), type);
		rf->type = 0;
		return -ENODEV;
	}

	if (r)
		return r;

	rf->type = type;

	r = zd_chip_lock_phy_regs(chip);
	if (r)
		return r;
	t = rf->init_hw(rf);
	r = zd_chip_unlock_phy_regs(chip);
	if (t)
		r = t;
	return r;
}

int zd_rf_scnprint_id(struct zd_rf *rf, char *buffer, size_t size)
{
	return scnprintf(buffer, size, "%s", zd_rf_name(rf->type));
}

int zd_rf_set_channel(struct zd_rf *rf, u8 channel)
{
	int r;

	ZD_ASSERT(mutex_is_locked(&zd_rf_to_chip(rf)->mutex));
	if (channel < MIN_CHANNEL24)
		return -EINVAL;
	if (channel > MAX_CHANNEL24)
		return -EINVAL;
	dev_dbg_f(zd_chip_dev(zd_rf_to_chip(rf)), "channel: %d\n", channel);

	r = rf->set_channel(rf, channel);
	if (r >= 0)
		rf->channel = channel;
	return r;
}

int zd_switch_radio_on(struct zd_rf *rf)
{
	int r, t;
	struct zd_chip *chip = zd_rf_to_chip(rf);

	ZD_ASSERT(mutex_is_locked(&chip->mutex));
	r = zd_chip_lock_phy_regs(chip);
	if (r)
		return r;
	t = rf->switch_radio_on(rf);
	r = zd_chip_unlock_phy_regs(chip);
	if (t)
		r = t;
	return r;
}

int zd_switch_radio_off(struct zd_rf *rf)
{
	int r, t;
	struct zd_chip *chip = zd_rf_to_chip(rf);

	/* TODO: move phy regs handling to zd_chip */
	ZD_ASSERT(mutex_is_locked(&chip->mutex));
	r = zd_chip_lock_phy_regs(chip);
	if (r)
		return r;
	t = rf->switch_radio_off(rf);
	r = zd_chip_unlock_phy_regs(chip);
	if (t)
		r = t;
	return r;
}

int zd_rf_patch_6m_band_edge(struct zd_rf *rf, u8 channel)
{
	if (!rf->patch_6m_band_edge)
		return 0;

	return rf->patch_6m_band_edge(rf, channel);
}

int zd_rf_generic_patch_6m(struct zd_rf *rf, u8 channel)
{
	return zd_chip_generic_patch_6m_band(zd_rf_to_chip(rf), channel);
}

