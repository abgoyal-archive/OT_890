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
 * arch/arm/plat-omap/include/mach/board-fsample.h
 *
 * Board-specific goodies for TI F-Sample.
 *
 * Copyright (C) 2006 Google, Inc.
 * Author: Brian Swetland <swetland@google.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_ARCH_OMAP_FSAMPLE_H
#define __ASM_ARCH_OMAP_FSAMPLE_H

/* fsample is pretty close to p2-sample */
#include <mach/board-perseus2.h>

#define fsample_cpld_read(reg) __raw_readb(reg)
#define fsample_cpld_write(val, reg) __raw_writeb(val, reg)

#define FSAMPLE_CPLD_BASE    0xE8100000
#define FSAMPLE_CPLD_SIZE    SZ_4K
#define FSAMPLE_CPLD_START   0x05080000

#define FSAMPLE_CPLD_REG_A   (FSAMPLE_CPLD_BASE + 0x00)
#define FSAMPLE_CPLD_SWITCH  (FSAMPLE_CPLD_BASE + 0x02)
#define FSAMPLE_CPLD_UART    (FSAMPLE_CPLD_BASE + 0x02)
#define FSAMPLE_CPLD_REG_B   (FSAMPLE_CPLD_BASE + 0x04)
#define FSAMPLE_CPLD_VERSION (FSAMPLE_CPLD_BASE + 0x06)
#define FSAMPLE_CPLD_SET_CLR (FSAMPLE_CPLD_BASE + 0x06)

#define FSAMPLE_CPLD_BIT_BT_RESET         0
#define FSAMPLE_CPLD_BIT_LCD_RESET        1
#define FSAMPLE_CPLD_BIT_CAM_PWDN         2
#define FSAMPLE_CPLD_BIT_CHARGER_ENABLE   3
#define FSAMPLE_CPLD_BIT_SD_MMC_EN        4
#define FSAMPLE_CPLD_BIT_aGPS_PWREN       5
#define FSAMPLE_CPLD_BIT_BACKLIGHT        6
#define FSAMPLE_CPLD_BIT_aGPS_EN_RESET    7
#define FSAMPLE_CPLD_BIT_aGPS_SLEEPx_N    8
#define FSAMPLE_CPLD_BIT_OTG_RESET        9

#define fsample_cpld_set(bit) \
    fsample_cpld_write((((bit) & 15) << 4) | 0x0f, FSAMPLE_CPLD_SET_CLR)

#define fsample_cpld_clear(bit) \
    fsample_cpld_write(0xf0 | ((bit) & 15), FSAMPLE_CPLD_SET_CLR)

#endif
