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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/* linux/include/asm-arm/arch-mt6516/board-wimax.h
 *
 * Devices Configuration for MT6516 WIMAX Phone
 *
 * Copyright (C) 2008,2009 MediaTek <www.mediatek.com>
 * Authors: Bryant Lu <bryant.lu@mediatek.com>  
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __ARCH_ARM_MACH_MT6516_BOARD_WIMAX_H
#define __ARCH_ARM_MACH_MT6516_BOARD_WIMAX_H

#include <linux/autoconf.h>
#include <mt6516_kpd.h>

/*=======================================================================*/
/* MT6516 SD                                                             */
/*=======================================================================*/
#define CFG_DEV_MSDC1
#define CFG_DEV_MSDC2

#define CFG_MSDC1_DATA_OFFSET   (0)
#define CFG_MSDC2_DATA_OFFSET   (0)

#define CFG_MSDC1_DATA_PINS     (4)
#define CFG_MSDC2_DATA_PINS     (4)

/*=======================================================================*/
/* MT6516 UART                                                           */
/*=======================================================================*/
#define CFG_DEV_UART1
#define CFG_DEV_UART2
#define CFG_DEV_UART3
#define CFG_DEV_UART4

#define CFG_UART_PORTS          (4)

/*=======================================================================*/
/* MT6516 I2C                                                            */
/*=======================================================================*/
#define CFG_DEV_I2C
//#define CFG_I2C_HIGH_SPEED_MODE
//#define CFG_I2C_DMA_MODE

/*=======================================================================*/
/* MT6516 TOUCH PANEL                                                    */
/*=======================================================================*/

#define TP_RES_X                240
#define TP_RES_Y                400
#define TP_PRESSURE_MIN         200
#define TP_PRESSURE_MAX         3000
#define TP_CALIBRATION_MATRIX   {-1183,6,1127068,-21,1969,-75047};
#define TP_GEMINI
#define TP_GEMINI_LEFT_KEY      KEY_MENU
#define TP_GEMINI_CENTER_KEY    KEY_HOME
#define TP_GEMINI_RIGHT_KEY     KEY_BACK

/*=======================================================================*/
/* MT6516 ADB                                                            */
/*=======================================================================*/
#define ADB_SERIAL "DS269"

/*=======================================================================*/
/* MT6516 NAND FLASH                                                     */
/*=======================================================================*/
#define USE_AHB_MODE 1

#define NFI_ACCESS_TIMING           0x10123222
//0x20022222//0x10123222//0x20022222//(0x10123222) 
// flashtool use 0x700fffff
#define NFI_BUS_WIDTH               (8)

#define NFI_CS_NUM                  (2)
#define NAND_SECTOR_SIZE            (512)
#define NAND_SECTOR_SHIFT           (9)

#define ECC_SIZE					(2048)
#define ECC_BYTES					(32)

#define NAND_ECC_MODE               NAND_ECC_HW

#endif /* __ARCH_ARM_MACH_MT6516_BOARD_WIMAX_H */

