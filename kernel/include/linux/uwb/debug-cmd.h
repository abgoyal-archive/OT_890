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
 * Ultra Wide Band
 * Debug interface commands
 *
 * Copyright (C) 2008 Cambridge Silicon Radio Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __LINUX__UWB__DEBUG_CMD_H__
#define __LINUX__UWB__DEBUG_CMD_H__

#include <linux/types.h>

/*
 * Debug interface commands
 *
 * UWB_DBG_CMD_RSV_ESTABLISH: Establish a new unicast reservation.
 *
 * UWB_DBG_CMD_RSV_TERMINATE: Terminate the Nth reservation.
 */

enum uwb_dbg_cmd_type {
	UWB_DBG_CMD_RSV_ESTABLISH = 1,
	UWB_DBG_CMD_RSV_TERMINATE = 2,
	UWB_DBG_CMD_IE_ADD = 3,
	UWB_DBG_CMD_IE_RM = 4,
	UWB_DBG_CMD_RADIO_START = 5,
	UWB_DBG_CMD_RADIO_STOP = 6,
};

struct uwb_dbg_cmd_rsv_establish {
	__u8  target[6];
	__u8  type;
	__u16 max_mas;
	__u16 min_mas;
	__u8  max_interval;
};

struct uwb_dbg_cmd_rsv_terminate {
	int index;
};

struct uwb_dbg_cmd_ie {
	__u8 data[128];
	int len;
};

struct uwb_dbg_cmd {
	__u32 type;
	union {
		struct uwb_dbg_cmd_rsv_establish rsv_establish;
		struct uwb_dbg_cmd_rsv_terminate rsv_terminate;
		struct uwb_dbg_cmd_ie ie_add;
		struct uwb_dbg_cmd_ie ie_rm;
	};
};

#endif /* #ifndef __LINUX__UWB__DEBUG_CMD_H__ */
