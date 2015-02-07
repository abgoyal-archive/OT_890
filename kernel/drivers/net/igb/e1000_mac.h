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

/*******************************************************************************

  Intel(R) Gigabit Ethernet Linux driver
  Copyright(c) 2007 Intel Corporation.

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

  Contact Information:
  e1000-devel Mailing List <e1000-devel@lists.sourceforge.net>
  Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497

*******************************************************************************/

#ifndef _E1000_MAC_H_
#define _E1000_MAC_H_

#include "e1000_hw.h"

#include "e1000_phy.h"
#include "e1000_nvm.h"
#include "e1000_defines.h"

/*
 * Functions that should not be called directly from drivers but can be used
 * by other files in this 'shared code'
 */
s32  igb_blink_led(struct e1000_hw *hw);
s32  igb_check_for_copper_link(struct e1000_hw *hw);
s32  igb_cleanup_led(struct e1000_hw *hw);
s32  igb_config_fc_after_link_up(struct e1000_hw *hw);
s32  igb_disable_pcie_master(struct e1000_hw *hw);
s32  igb_force_mac_fc(struct e1000_hw *hw);
s32  igb_get_auto_rd_done(struct e1000_hw *hw);
s32  igb_get_bus_info_pcie(struct e1000_hw *hw);
s32  igb_get_hw_semaphore(struct e1000_hw *hw);
s32  igb_get_speed_and_duplex_copper(struct e1000_hw *hw, u16 *speed,
				       u16 *duplex);
s32  igb_id_led_init(struct e1000_hw *hw);
s32  igb_led_off(struct e1000_hw *hw);
s32  igb_setup_link(struct e1000_hw *hw);
s32  igb_validate_mdi_setting(struct e1000_hw *hw);
s32  igb_write_8bit_ctrl_reg(struct e1000_hw *hw, u32 reg,
			       u32 offset, u8 data);

void igb_clear_hw_cntrs_base(struct e1000_hw *hw);
void igb_clear_vfta(struct e1000_hw *hw);
void igb_config_collision_dist(struct e1000_hw *hw);
void igb_mta_set(struct e1000_hw *hw, u32 hash_value);
void igb_put_hw_semaphore(struct e1000_hw *hw);
void igb_rar_set(struct e1000_hw *hw, u8 *addr, u32 index);
s32  igb_check_alt_mac_addr(struct e1000_hw *hw);
void igb_remove_device(struct e1000_hw *hw);
void igb_reset_adaptive(struct e1000_hw *hw);
void igb_update_adaptive(struct e1000_hw *hw);
void igb_write_vfta(struct e1000_hw *hw, u32 offset, u32 value);

bool igb_enable_mng_pass_thru(struct e1000_hw *hw);

enum e1000_mng_mode {
	e1000_mng_mode_none = 0,
	e1000_mng_mode_asf,
	e1000_mng_mode_pt,
	e1000_mng_mode_ipmi,
	e1000_mng_mode_host_if_only
};

#define E1000_FACTPS_MNGCG    0x20000000

#define E1000_FWSM_MODE_MASK  0xE
#define E1000_FWSM_MODE_SHIFT 1

#define E1000_MNG_DHCP_COMMAND_TIMEOUT       10
#define E1000_MNG_DHCP_COOKIE_STATUS_VLAN    0x2

#define E1000_HICR_EN              0x01  /* Enable bit - RO */
/* Driver sets this bit when done to put command in RAM */
#define E1000_HICR_C               0x02

extern void e1000_init_function_pointers_82575(struct e1000_hw *hw);
extern u32 igb_hash_mc_addr(struct e1000_hw *hw, u8 *mc_addr);

#endif
