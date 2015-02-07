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

/**
  * This file contains definitions of WLAN commands.
  */

#ifndef _LBS_HOST_H_
#define _LBS_HOST_H_

/** PUBLIC DEFINITIONS */
#define DEFAULT_AD_HOC_CHANNEL			6
#define	DEFAULT_AD_HOC_CHANNEL_A		36

#define CMD_OPTION_WAITFORRSP			0x0002

/** Host command IDs */

/* Return command are almost always the same as the host command, but with
 * bit 15 set high.  There are a few exceptions, though...
 */
#define CMD_RET(cmd)			(0x8000 | cmd)

/* Return command convention exceptions: */
#define CMD_RET_802_11_ASSOCIATE		0x8012

/* Command codes */
#define CMD_GET_HW_SPEC				0x0003
#define	CMD_EEPROM_UPDATE			0x0004
#define CMD_802_11_RESET			0x0005
#define	CMD_802_11_SCAN				0x0006
#define CMD_802_11_GET_LOG			0x000b
#define CMD_MAC_MULTICAST_ADR			0x0010
#define CMD_802_11_AUTHENTICATE			0x0011
#define CMD_802_11_EEPROM_ACCESS		0x0059
#define CMD_802_11_ASSOCIATE			0x0050
#define CMD_802_11_SET_WEP			0x0013
#define CMD_802_11_GET_STAT			0x0014
#define CMD_802_3_GET_STAT			0x0015
#define CMD_802_11_SNMP_MIB			0x0016
#define CMD_MAC_REG_MAP				0x0017
#define CMD_BBP_REG_MAP				0x0018
#define CMD_MAC_REG_ACCESS			0x0019
#define CMD_BBP_REG_ACCESS			0x001a
#define CMD_RF_REG_ACCESS			0x001b
#define CMD_802_11_RADIO_CONTROL		0x001c
#define CMD_802_11_RF_CHANNEL			0x001d
#define CMD_802_11_RF_TX_POWER			0x001e
#define CMD_802_11_RSSI				0x001f
#define CMD_802_11_RF_ANTENNA			0x0020
#define CMD_802_11_PS_MODE			0x0021
#define CMD_802_11_DATA_RATE			0x0022
#define CMD_RF_REG_MAP				0x0023
#define CMD_802_11_DEAUTHENTICATE		0x0024
#define CMD_802_11_REASSOCIATE			0x0025
#define CMD_MAC_CONTROL				0x0028
#define CMD_802_11_AD_HOC_START			0x002b
#define CMD_802_11_AD_HOC_JOIN			0x002c
#define CMD_802_11_QUERY_TKIP_REPLY_CNTRS	0x002e
#define CMD_802_11_ENABLE_RSN			0x002f
#define CMD_802_11_SET_AFC			0x003c
#define CMD_802_11_GET_AFC			0x003d
#define CMD_802_11_AD_HOC_STOP			0x0040
#define CMD_802_11_HOST_SLEEP_CFG		0x0043
#define CMD_802_11_WAKEUP_CONFIRM		0x0044
#define CMD_802_11_HOST_SLEEP_ACTIVATE		0x0045
#define CMD_802_11_BEACON_STOP			0x0049
#define CMD_802_11_MAC_ADDRESS			0x004d
#define CMD_802_11_LED_GPIO_CTRL		0x004e
#define CMD_802_11_EEPROM_ACCESS		0x0059
#define CMD_802_11_BAND_CONFIG			0x0058
#define CMD_802_11D_DOMAIN_INFO			0x005b
#define CMD_802_11_KEY_MATERIAL			0x005e
#define CMD_802_11_SLEEP_PARAMS			0x0066
#define CMD_802_11_INACTIVITY_TIMEOUT		0x0067
#define CMD_802_11_SLEEP_PERIOD			0x0068
#define CMD_802_11_TPC_CFG			0x0072
#define CMD_802_11_PA_CFG			0x0073
#define CMD_802_11_FW_WAKE_METHOD		0x0074
#define CMD_802_11_SUBSCRIBE_EVENT		0x0075
#define CMD_802_11_RATE_ADAPT_RATESET		0x0076
#define CMD_802_11_TX_RATE_QUERY		0x007f
#define	CMD_GET_TSF				0x0080
#define CMD_BT_ACCESS				0x0087
#define CMD_FWT_ACCESS				0x0095
#define CMD_802_11_MONITOR_MODE			0x0098
#define CMD_MESH_ACCESS				0x009b
#define CMD_MESH_CONFIG				0x00a3
#define	CMD_SET_BOOT2_VER			0x00a5
#define CMD_802_11_BEACON_CTRL			0x00b0

/* For the IEEE Power Save */
#define CMD_SUBCMD_ENTER_PS		0x0030
#define CMD_SUBCMD_EXIT_PS		0x0031
#define CMD_SUBCMD_SLEEP_CONFIRMED	0x0034
#define CMD_SUBCMD_FULL_POWERDOWN	0x0035
#define CMD_SUBCMD_FULL_POWERUP		0x0036

#define CMD_ENABLE_RSN			0x0001
#define CMD_DISABLE_RSN			0x0000

#define CMD_ACT_GET			0x0000
#define CMD_ACT_SET			0x0001
#define CMD_ACT_GET_AES			0x0002
#define CMD_ACT_SET_AES			0x0003
#define CMD_ACT_REMOVE_AES		0x0004

/* Define action or option for CMD_802_11_SET_WEP */
#define CMD_ACT_ADD			0x0002
#define CMD_ACT_REMOVE			0x0004
#define CMD_ACT_USE_DEFAULT		0x0008

#define CMD_TYPE_WEP_40_BIT		0x01
#define CMD_TYPE_WEP_104_BIT		0x02

#define CMD_NUM_OF_WEP_KEYS		4

#define CMD_WEP_KEY_INDEX_MASK		0x3fff

/* Define action or option for CMD_802_11_RESET */
#define CMD_ACT_HALT			0x0003

/* Define action or option for CMD_802_11_SCAN */
#define CMD_BSS_TYPE_BSS		0x0001
#define CMD_BSS_TYPE_IBSS		0x0002
#define CMD_BSS_TYPE_ANY		0x0003

/* Define action or option for CMD_802_11_SCAN */
#define CMD_SCAN_TYPE_ACTIVE		0x0000
#define CMD_SCAN_TYPE_PASSIVE		0x0001

#define CMD_SCAN_RADIO_TYPE_BG		0

#define	CMD_SCAN_PROBE_DELAY_TIME	0

/* Define action or option for CMD_MAC_CONTROL */
#define CMD_ACT_MAC_RX_ON			0x0001
#define CMD_ACT_MAC_TX_ON			0x0002
#define CMD_ACT_MAC_LOOPBACK_ON			0x0004
#define CMD_ACT_MAC_WEP_ENABLE			0x0008
#define CMD_ACT_MAC_INT_ENABLE			0x0010
#define CMD_ACT_MAC_MULTICAST_ENABLE		0x0020
#define CMD_ACT_MAC_BROADCAST_ENABLE		0x0040
#define CMD_ACT_MAC_PROMISCUOUS_ENABLE		0x0080
#define CMD_ACT_MAC_ALL_MULTICAST_ENABLE	0x0100
#define CMD_ACT_MAC_STRICT_PROTECTION_ENABLE	0x0400

/* Event flags for CMD_802_11_SUBSCRIBE_EVENT */
#define CMD_SUBSCRIBE_RSSI_LOW		0x0001
#define CMD_SUBSCRIBE_SNR_LOW		0x0002
#define CMD_SUBSCRIBE_FAILCOUNT		0x0004
#define CMD_SUBSCRIBE_BCNMISS		0x0008
#define CMD_SUBSCRIBE_RSSI_HIGH		0x0010
#define CMD_SUBSCRIBE_SNR_HIGH		0x0020

#define RADIO_PREAMBLE_LONG	0x00
#define RADIO_PREAMBLE_SHORT	0x02
#define RADIO_PREAMBLE_AUTO	0x04

/* Define action or option for CMD_802_11_RF_CHANNEL */
#define CMD_OPT_802_11_RF_CHANNEL_GET	0x00
#define CMD_OPT_802_11_RF_CHANNEL_SET	0x01

/* Define action or option for CMD_802_11_DATA_RATE */
#define CMD_ACT_SET_TX_AUTO		0x0000
#define CMD_ACT_SET_TX_FIX_RATE		0x0001
#define CMD_ACT_GET_TX_RATE		0x0002

#define CMD_ACT_SET_RX			0x0001
#define	CMD_ACT_SET_TX			0x0002
#define CMD_ACT_SET_BOTH		0x0003
#define	CMD_ACT_GET_RX			0x0004
#define CMD_ACT_GET_TX			0x0008
#define	CMD_ACT_GET_BOTH		0x000c

/* Define action or option for CMD_802_11_PS_MODE */
#define CMD_TYPE_CAM			0x0000
#define	CMD_TYPE_MAX_PSP		0x0001
#define CMD_TYPE_FAST_PSP		0x0002

/* Options for CMD_802_11_FW_WAKE_METHOD */
#define CMD_WAKE_METHOD_UNCHANGED	0x0000
#define CMD_WAKE_METHOD_COMMAND_INT	0x0001
#define CMD_WAKE_METHOD_GPIO		0x0002

/* Object IDs for CMD_802_11_SNMP_MIB */
#define SNMP_MIB_OID_BSS_TYPE		0x0000
#define SNMP_MIB_OID_OP_RATE_SET	0x0001
#define SNMP_MIB_OID_BEACON_PERIOD	0x0002  /* Reserved on v9+ */
#define SNMP_MIB_OID_DTIM_PERIOD	0x0003  /* Reserved on v9+ */
#define SNMP_MIB_OID_ASSOC_TIMEOUT	0x0004  /* Reserved on v9+ */
#define SNMP_MIB_OID_RTS_THRESHOLD	0x0005
#define SNMP_MIB_OID_SHORT_RETRY_LIMIT	0x0006
#define SNMP_MIB_OID_LONG_RETRY_LIMIT	0x0007
#define SNMP_MIB_OID_FRAG_THRESHOLD	0x0008
#define SNMP_MIB_OID_11D_ENABLE		0x0009
#define SNMP_MIB_OID_11H_ENABLE		0x000A

/* Define action or option for CMD_BT_ACCESS */
enum cmd_bt_access_opts {
	/* The bt commands start at 5 instead of 1 because the old dft commands
	 * are mapped to 1-4.  These old commands are no longer maintained and
	 * should not be called.
	 */
	CMD_ACT_BT_ACCESS_ADD = 5,
	CMD_ACT_BT_ACCESS_DEL,
	CMD_ACT_BT_ACCESS_LIST,
	CMD_ACT_BT_ACCESS_RESET,
	CMD_ACT_BT_ACCESS_SET_INVERT,
	CMD_ACT_BT_ACCESS_GET_INVERT
};

/* Define action or option for CMD_FWT_ACCESS */
enum cmd_fwt_access_opts {
	CMD_ACT_FWT_ACCESS_ADD = 1,
	CMD_ACT_FWT_ACCESS_DEL,
	CMD_ACT_FWT_ACCESS_LOOKUP,
	CMD_ACT_FWT_ACCESS_LIST,
	CMD_ACT_FWT_ACCESS_LIST_ROUTE,
	CMD_ACT_FWT_ACCESS_LIST_NEIGHBOR,
	CMD_ACT_FWT_ACCESS_RESET,
	CMD_ACT_FWT_ACCESS_CLEANUP,
	CMD_ACT_FWT_ACCESS_TIME,
};

/* Define action or option for CMD_802_11_HOST_SLEEP_CFG */
enum cmd_wol_cfg_opts {
	CMD_ACT_ACTION_NONE = 0,
	CMD_ACT_SET_WOL_RULE,
	CMD_ACT_GET_WOL_RULE,
	CMD_ACT_RESET_WOL_RULE,
};

/* Define action or option for CMD_MESH_ACCESS */
enum cmd_mesh_access_opts {
	CMD_ACT_MESH_GET_TTL = 1,
	CMD_ACT_MESH_SET_TTL,
	CMD_ACT_MESH_GET_STATS,
	CMD_ACT_MESH_GET_ANYCAST,
	CMD_ACT_MESH_SET_ANYCAST,
	CMD_ACT_MESH_SET_LINK_COSTS,
	CMD_ACT_MESH_GET_LINK_COSTS,
	CMD_ACT_MESH_SET_BCAST_RATE,
	CMD_ACT_MESH_GET_BCAST_RATE,
	CMD_ACT_MESH_SET_RREQ_DELAY,
	CMD_ACT_MESH_GET_RREQ_DELAY,
	CMD_ACT_MESH_SET_ROUTE_EXP,
	CMD_ACT_MESH_GET_ROUTE_EXP,
	CMD_ACT_MESH_SET_AUTOSTART_ENABLED,
	CMD_ACT_MESH_GET_AUTOSTART_ENABLED,
	CMD_ACT_MESH_SET_GET_PRB_RSP_LIMIT = 17,
};

/* Define actions and types for CMD_MESH_CONFIG */
enum cmd_mesh_config_actions {
	CMD_ACT_MESH_CONFIG_STOP = 0,
	CMD_ACT_MESH_CONFIG_START,
	CMD_ACT_MESH_CONFIG_SET,
	CMD_ACT_MESH_CONFIG_GET,
};

enum cmd_mesh_config_types {
	CMD_TYPE_MESH_SET_BOOTFLAG = 1,
	CMD_TYPE_MESH_SET_BOOTTIME,
	CMD_TYPE_MESH_SET_DEF_CHANNEL,
	CMD_TYPE_MESH_SET_MESH_IE,
	CMD_TYPE_MESH_GET_DEFAULTS,
	CMD_TYPE_MESH_GET_MESH_IE, /* GET_DEFAULTS is superset of GET_MESHIE */
};

/** Card Event definition */
#define MACREG_INT_CODE_TX_PPA_FREE		0
#define MACREG_INT_CODE_TX_DMA_DONE		1
#define MACREG_INT_CODE_LINK_LOST_W_SCAN	2
#define MACREG_INT_CODE_LINK_LOST_NO_SCAN	3
#define MACREG_INT_CODE_LINK_SENSED		4
#define MACREG_INT_CODE_CMD_FINISHED		5
#define MACREG_INT_CODE_MIB_CHANGED		6
#define MACREG_INT_CODE_INIT_DONE		7
#define MACREG_INT_CODE_DEAUTHENTICATED		8
#define MACREG_INT_CODE_DISASSOCIATED		9
#define MACREG_INT_CODE_PS_AWAKE		10
#define MACREG_INT_CODE_PS_SLEEP		11
#define MACREG_INT_CODE_MIC_ERR_MULTICAST	13
#define MACREG_INT_CODE_MIC_ERR_UNICAST		14
#define MACREG_INT_CODE_WM_AWAKE		15
#define MACREG_INT_CODE_DEEP_SLEEP_AWAKE	16
#define MACREG_INT_CODE_ADHOC_BCN_LOST		17
#define MACREG_INT_CODE_HOST_AWAKE		18
#define MACREG_INT_CODE_STOP_TX			19
#define MACREG_INT_CODE_START_TX		20
#define MACREG_INT_CODE_CHANNEL_SWITCH		21
#define MACREG_INT_CODE_MEASUREMENT_RDY		22
#define MACREG_INT_CODE_WMM_CHANGE		23
#define MACREG_INT_CODE_BG_SCAN_REPORT		24
#define MACREG_INT_CODE_RSSI_LOW		25
#define MACREG_INT_CODE_SNR_LOW			26
#define MACREG_INT_CODE_MAX_FAIL		27
#define MACREG_INT_CODE_RSSI_HIGH		28
#define MACREG_INT_CODE_SNR_HIGH		29
#define MACREG_INT_CODE_MESH_AUTO_STARTED	35
#define MACREG_INT_CODE_FIRMWARE_READY		48

#endif