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

  Intel PRO/10GbE Linux driver
  Copyright(c) 1999 - 2008 Intel Corporation.

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
  Linux NICS <linux.nics@intel.com>
  e1000-devel Mailing List <e1000-devel@lists.sourceforge.net>
  Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497

*******************************************************************************/

#ifndef _IXGB_EE_H_
#define _IXGB_EE_H_

#define IXGB_EEPROM_SIZE    64	/* Size in words */

#define IXGB_ETH_LENGTH_OF_ADDRESS   6

/* EEPROM Commands */
#define EEPROM_READ_OPCODE  0x6	/* EEPROM read opcode */
#define EEPROM_WRITE_OPCODE 0x5	/* EEPROM write opcode */
#define EEPROM_ERASE_OPCODE 0x7	/* EEPROM erase opcode */
#define EEPROM_EWEN_OPCODE  0x13	/* EEPROM erase/write enable */
#define EEPROM_EWDS_OPCODE  0x10	/* EEPROM erase/write disable */

/* EEPROM MAP (Word Offsets) */
#define EEPROM_IA_1_2_REG        0x0000
#define EEPROM_IA_3_4_REG        0x0001
#define EEPROM_IA_5_6_REG        0x0002
#define EEPROM_COMPATIBILITY_REG 0x0003
#define EEPROM_PBA_1_2_REG       0x0008
#define EEPROM_PBA_3_4_REG       0x0009
#define EEPROM_INIT_CONTROL1_REG 0x000A
#define EEPROM_SUBSYS_ID_REG     0x000B
#define EEPROM_SUBVEND_ID_REG    0x000C
#define EEPROM_DEVICE_ID_REG     0x000D
#define EEPROM_VENDOR_ID_REG     0x000E
#define EEPROM_INIT_CONTROL2_REG 0x000F
#define EEPROM_SWDPINS_REG       0x0020
#define EEPROM_CIRCUIT_CTRL_REG  0x0021
#define EEPROM_D0_D3_POWER_REG   0x0022
#define EEPROM_FLASH_VERSION     0x0032
#define EEPROM_CHECKSUM_REG      0x003F

/* Mask bits for fields in Word 0x0a of the EEPROM */

#define EEPROM_ICW1_SIGNATURE_MASK  0xC000
#define EEPROM_ICW1_SIGNATURE_VALID 0x4000
#define EEPROM_ICW1_SIGNATURE_CLEAR 0x0000

/* For checksumming, the sum of all words in the EEPROM should equal 0xBABA. */
#define EEPROM_SUM 0xBABA

/* EEPROM Map Sizes (Byte Counts) */
#define PBA_SIZE 4

/* EEPROM Map defines (WORD OFFSETS)*/

/* EEPROM structure */
struct ixgb_ee_map_type {
	u8 mac_addr[IXGB_ETH_LENGTH_OF_ADDRESS];
	__le16 compatibility;
	__le16 reserved1[4];
	__le32 pba_number;
	__le16 init_ctrl_reg_1;
	__le16 subsystem_id;
	__le16 subvendor_id;
	__le16 device_id;
	__le16 vendor_id;
	__le16 init_ctrl_reg_2;
	__le16 oem_reserved[16];
	__le16 swdpins_reg;
	__le16 circuit_ctrl_reg;
	u8 d3_power;
	u8 d0_power;
	__le16 reserved2[28];
	__le16 checksum;
};

/* EEPROM Functions */
u16 ixgb_read_eeprom(struct ixgb_hw *hw, u16 reg);

bool ixgb_validate_eeprom_checksum(struct ixgb_hw *hw);

void ixgb_update_eeprom_checksum(struct ixgb_hw *hw);

void ixgb_write_eeprom(struct ixgb_hw *hw, u16 reg, u16 data);

#endif				/* IXGB_EE_H */
