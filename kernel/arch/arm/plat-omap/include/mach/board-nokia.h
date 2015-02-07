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
 *  arch/arm/plat-omap/include/mach/board-nokia.h
 *
 *  Information structures for Nokia-specific board config data
 *
 *  Copyright (C) 2005	Nokia Corporation
 */

#ifndef _OMAP_BOARD_NOKIA_H
#define _OMAP_BOARD_NOKIA_H

#include <linux/types.h>

#define OMAP_TAG_NOKIA_BT	0x4e01
#define OMAP_TAG_WLAN_CX3110X	0x4e02
#define OMAP_TAG_CBUS		0x4e03
#define OMAP_TAG_EM_ASIC_BB5	0x4e04


#define BT_CHIP_CSR		1
#define BT_CHIP_TI		2

#define BT_SYSCLK_12		1
#define BT_SYSCLK_38_4		2

struct omap_bluetooth_config {
	u8    chip_type;
	u8    bt_wakeup_gpio;
	u8    host_wakeup_gpio;
	u8    reset_gpio;
	u8    bt_uart;
	u8    bd_addr[6];
	u8    bt_sysclk;
};

struct omap_wlan_cx3110x_config {
	u8  chip_type;
	s16 power_gpio;
	s16 irq_gpio;
	s16 spi_cs_gpio;
};

struct omap_cbus_config {
	s16 clk_gpio;
	s16 dat_gpio;
	s16 sel_gpio;
};

struct omap_em_asic_bb5_config {
	s16 retu_irq_gpio;
	s16 tahvo_irq_gpio;
};

#endif
