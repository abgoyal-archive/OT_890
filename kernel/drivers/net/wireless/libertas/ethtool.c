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

#include <linux/netdevice.h>
#include <linux/ethtool.h>
#include <linux/delay.h>

#include "host.h"
#include "decl.h"
#include "defs.h"
#include "dev.h"
#include "wext.h"
#include "cmd.h"

static const char * mesh_stat_strings[]= {
			"drop_duplicate_bcast",
			"drop_ttl_zero",
			"drop_no_fwd_route",
			"drop_no_buffers",
			"fwded_unicast_cnt",
			"fwded_bcast_cnt",
			"drop_blind_table",
			"tx_failed_cnt"
};

static void lbs_ethtool_get_drvinfo(struct net_device *dev,
					 struct ethtool_drvinfo *info)
{
	struct lbs_private *priv = dev->ml_priv;

	snprintf(info->fw_version, 32, "%u.%u.%u.p%u",
		priv->fwrelease >> 24 & 0xff,
		priv->fwrelease >> 16 & 0xff,
		priv->fwrelease >>  8 & 0xff,
		priv->fwrelease       & 0xff);
	strcpy(info->driver, "libertas");
	strcpy(info->version, lbs_driver_version);
}

/* All 8388 parts have 16KiB EEPROM size at the time of writing.
 * In case that changes this needs fixing.
 */
#define LBS_EEPROM_LEN 16384

static int lbs_ethtool_get_eeprom_len(struct net_device *dev)
{
	return LBS_EEPROM_LEN;
}

static int lbs_ethtool_get_eeprom(struct net_device *dev,
                                  struct ethtool_eeprom *eeprom, u8 * bytes)
{
	struct lbs_private *priv = dev->ml_priv;
	struct cmd_ds_802_11_eeprom_access cmd;
	int ret;

	lbs_deb_enter(LBS_DEB_ETHTOOL);

	if (eeprom->offset + eeprom->len > LBS_EEPROM_LEN ||
	    eeprom->len > LBS_EEPROM_READ_LEN) {
		ret = -EINVAL;
		goto out;
	}

	cmd.hdr.size = cpu_to_le16(sizeof(struct cmd_ds_802_11_eeprom_access) -
		LBS_EEPROM_READ_LEN + eeprom->len);
	cmd.action = cpu_to_le16(CMD_ACT_GET);
	cmd.offset = cpu_to_le16(eeprom->offset);
	cmd.len    = cpu_to_le16(eeprom->len);
	ret = lbs_cmd_with_response(priv, CMD_802_11_EEPROM_ACCESS, &cmd);
	if (!ret)
		memcpy(bytes, cmd.value, eeprom->len);

out:
	lbs_deb_leave_args(LBS_DEB_ETHTOOL, "ret %d", ret);
        return ret;
}

static void lbs_ethtool_get_stats(struct net_device *dev,
				  struct ethtool_stats *stats, uint64_t *data)
{
	struct lbs_private *priv = dev->ml_priv;
	struct cmd_ds_mesh_access mesh_access;
	int ret;

	lbs_deb_enter(LBS_DEB_ETHTOOL);

	/* Get Mesh Statistics */
	ret = lbs_mesh_access(priv, CMD_ACT_MESH_GET_STATS, &mesh_access);

	if (ret) {
		memset(data, 0, MESH_STATS_NUM*(sizeof(uint64_t)));
		return;
	}

	priv->mstats.fwd_drop_rbt = le32_to_cpu(mesh_access.data[0]);
	priv->mstats.fwd_drop_ttl = le32_to_cpu(mesh_access.data[1]);
	priv->mstats.fwd_drop_noroute = le32_to_cpu(mesh_access.data[2]);
	priv->mstats.fwd_drop_nobuf = le32_to_cpu(mesh_access.data[3]);
	priv->mstats.fwd_unicast_cnt = le32_to_cpu(mesh_access.data[4]);
	priv->mstats.fwd_bcast_cnt = le32_to_cpu(mesh_access.data[5]);
	priv->mstats.drop_blind = le32_to_cpu(mesh_access.data[6]);
	priv->mstats.tx_failed_cnt = le32_to_cpu(mesh_access.data[7]);

	data[0] = priv->mstats.fwd_drop_rbt;
	data[1] = priv->mstats.fwd_drop_ttl;
	data[2] = priv->mstats.fwd_drop_noroute;
	data[3] = priv->mstats.fwd_drop_nobuf;
	data[4] = priv->mstats.fwd_unicast_cnt;
	data[5] = priv->mstats.fwd_bcast_cnt;
	data[6] = priv->mstats.drop_blind;
	data[7] = priv->mstats.tx_failed_cnt;

	lbs_deb_enter(LBS_DEB_ETHTOOL);
}

static int lbs_ethtool_get_sset_count(struct net_device *dev, int sset)
{
	struct lbs_private *priv = dev->ml_priv;

	if (sset == ETH_SS_STATS && dev == priv->mesh_dev)
		return MESH_STATS_NUM;

	return -EOPNOTSUPP;
}

static void lbs_ethtool_get_strings(struct net_device *dev,
				    uint32_t stringset, uint8_t *s)
{
	int i;

	lbs_deb_enter(LBS_DEB_ETHTOOL);

	switch (stringset) {
        case ETH_SS_STATS:
		for (i=0; i < MESH_STATS_NUM; i++) {
			memcpy(s + i * ETH_GSTRING_LEN,
					mesh_stat_strings[i],
					ETH_GSTRING_LEN);
		}
		break;
        }
	lbs_deb_enter(LBS_DEB_ETHTOOL);
}

static void lbs_ethtool_get_wol(struct net_device *dev,
				struct ethtool_wolinfo *wol)
{
	struct lbs_private *priv = dev->ml_priv;

	if (priv->wol_criteria == 0xffffffff) {
		/* Interface driver didn't configure wake */
		wol->supported = wol->wolopts = 0;
		return;
	}

	wol->supported = WAKE_UCAST|WAKE_MCAST|WAKE_BCAST|WAKE_PHY;

	if (priv->wol_criteria & EHS_WAKE_ON_UNICAST_DATA)
		wol->wolopts |= WAKE_UCAST;
	if (priv->wol_criteria & EHS_WAKE_ON_MULTICAST_DATA)
		wol->wolopts |= WAKE_MCAST;
	if (priv->wol_criteria & EHS_WAKE_ON_BROADCAST_DATA)
		wol->wolopts |= WAKE_BCAST;
	if (priv->wol_criteria & EHS_WAKE_ON_MAC_EVENT)
		wol->wolopts |= WAKE_PHY;
}

static int lbs_ethtool_set_wol(struct net_device *dev,
			       struct ethtool_wolinfo *wol)
{
	struct lbs_private *priv = dev->ml_priv;
	uint32_t criteria = 0;

	if (priv->wol_criteria == 0xffffffff && wol->wolopts)
		return -EOPNOTSUPP;

	if (wol->wolopts & ~(WAKE_UCAST|WAKE_MCAST|WAKE_BCAST|WAKE_PHY))
		return -EOPNOTSUPP;

	if (wol->wolopts & WAKE_UCAST) criteria |= EHS_WAKE_ON_UNICAST_DATA;
	if (wol->wolopts & WAKE_MCAST) criteria |= EHS_WAKE_ON_MULTICAST_DATA;
	if (wol->wolopts & WAKE_BCAST) criteria |= EHS_WAKE_ON_BROADCAST_DATA;
	if (wol->wolopts & WAKE_PHY)   criteria |= EHS_WAKE_ON_MAC_EVENT;

	return lbs_host_sleep_cfg(priv, criteria, (struct wol_config *)NULL);
}

struct ethtool_ops lbs_ethtool_ops = {
	.get_drvinfo = lbs_ethtool_get_drvinfo,
	.get_eeprom =  lbs_ethtool_get_eeprom,
	.get_eeprom_len = lbs_ethtool_get_eeprom_len,
	.get_sset_count = lbs_ethtool_get_sset_count,
	.get_ethtool_stats = lbs_ethtool_get_stats,
	.get_strings = lbs_ethtool_get_strings,
	.get_wol = lbs_ethtool_get_wol,
	.set_wol = lbs_ethtool_set_wol,
};

