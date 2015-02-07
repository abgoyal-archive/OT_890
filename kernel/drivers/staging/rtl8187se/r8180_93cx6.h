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
	This is part of rtl8180 OpenSource driver
	Copyright (C) Andrea Merello 2004-2005  <andreamrl@tiscali.it>
	Released under the terms of GPL (General Public Licence)

	Parts of this driver are based on the GPL part of the official realtek driver
	Parts of this driver are based on the rtl8180 driver skeleton from Patric Schenke & Andres Salomon
	Parts of this driver are based on the Intel Pro Wireless 2100 GPL driver

	We want to tanks the Authors of such projects and the Ndiswrapper project Authors.
*/

/*This files contains card eeprom (93c46 or 93c56) programming routines*/
/*memory is addressed by WORDS*/

#include "r8180.h"
#include "r8180_hw.h"

#define EPROM_DELAY 10

#define EPROM_ANAPARAM_ADDRLWORD 0xd
#define EPROM_ANAPARAM_ADDRHWORD 0xe

#define RFCHIPID 0x6
#define	RFCHIPID_INTERSIL 1
#define	RFCHIPID_RFMD 2
#define	RFCHIPID_PHILIPS 3
#define	RFCHIPID_MAXIM 4
#define	RFCHIPID_GCT 5
#define RFCHIPID_RTL8225 9
#ifdef CONFIG_RTL8185B
#define RF_ZEBRA2 11
#define EPROM_TXPW_BASE 0x05
#define RF_ZEBRA4 12
#endif
#define RFCHIPID_RTL8255 0xa
#define RF_PARAM 0x19
#define RF_PARAM_DIGPHY_SHIFT 0
#define RF_PARAM_ANTBDEFAULT_SHIFT 1
#define RF_PARAM_CARRIERSENSE_SHIFT 2
#define RF_PARAM_CARRIERSENSE_MASK (3<<2)
#define ENERGY_TRESHOLD 0x17
#define EPROM_VERSION 0x1E
#define MAC_ADR 0x7

#define CIS 0x18

#define	EPROM_TXPW_OFDM_CH1_2 0x20

//#define	EPROM_TXPW_CH1_2 0x10
#define  EPROM_TXPW_CH1_2 0x30
#define	EPROM_TXPW_CH3_4 0x11
#define	EPROM_TXPW_CH5_6 0x12
#define	EPROM_TXPW_CH7_8 0x13
#define	EPROM_TXPW_CH9_10 0x14
#define	EPROM_TXPW_CH11_12 0x15
#define	EPROM_TXPW_CH13_14 0x16

u32 eprom_read(struct net_device *dev,u32 addr); //reads a 16 bits word
