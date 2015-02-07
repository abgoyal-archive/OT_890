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
 * @file me8200_di_reg.h
 *
 * @brief ME-8200 digital input subdevice register definitions.
 * @note Copyright (C) 2007 Meilhaus Electronic GmbH (support@meilhaus.de)
 * @author Guenter Gebhardt
 */

/*
 * Copyright (C) 2007 Meilhaus Electronic GmbH (support@meilhaus.de)
 *
 * This file is free software; you can redistribute it and/or modify
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _ME8200_DI_REG_H_
#define _ME8200_DI_REG_H_

#ifdef __KERNEL__

// Common registry for whole family.
#define ME8200_DI_PORT_0_REG					0x3	// R
#define ME8200_DI_PORT_1_REG					0x4	// R

#define ME8200_DI_MASK_0_REG					0x5	// R/W
#define ME8200_DI_MASK_1_REG					0x6	// R/W

#define ME8200_DI_COMPARE_0_REG					0xA	// R/W
#define ME8200_DI_COMPARE_1_REG					0xB	// R/W

#define ME8200_DI_IRQ_CTRL_REG					0xC	// R/W

#ifndef ME8200_IRQ_MODE_REG
# define ME8200_IRQ_MODE_REG					0xD	// R/W
#endif

// This registry are for all versions
#define ME8200_DI_CHANGE_0_REG					0xE	// R
#define ME8200_DI_CHANGE_1_REG					0xF	// R

#define ME8200_DI_IRQ_CTRL_BIT_CLEAR			0x4
#define ME8200_DI_IRQ_CTRL_BIT_ENABLE			0x8

// This registry are for firmware versions 7 and later
#define ME8200_DI_EXTEND_CHANGE_0_LOW_REG		0x10	// R
#define ME8200_DI_EXTEND_CHANGE_0_HIGH_REG		0x11	// R
#define ME8200_DI_EXTEND_CHANGE_1_LOW_REG		0x12	// R
#define ME8200_DI_EXTEND_CHANGE_1_HIGH_REG		0x13	// R

#ifndef ME8200_FIRMWARE_VERSION_REG
# define ME8200_FIRMWARE_VERSION_REG			0x14	// R
#endif

// Bit definitions
#define ME8200_DI_IRQ_CTRL_MASK_EDGE			0x3
#define ME8200_DI_IRQ_CTRL_MASK_EDGE_RISING		0x0
#define ME8200_DI_IRQ_CTRL_MASK_EDGE_FALLING	0x1
#define ME8200_DI_IRQ_CTRL_MASK_EDGE_ANY		0x3

// Others
#define ME8200_DI_IRQ_CTRL_SHIFT				4

#endif
#endif
