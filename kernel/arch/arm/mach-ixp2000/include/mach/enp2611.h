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
 * arch/arm/mach-ixp2000/include/mach/enp2611.h
 *
 * Register and other defines for Radisys ENP-2611
 *
 * Created 2004 by Lennert Buytenhek from the ixdp2x01 code.  The
 * original version carries the following notices:
 *
 * Original Author: Naeem Afzal <naeem.m.afzal@intel.com>
 * Maintainer: Deepak Saxena <dsaxena@plexity.net>
 *
 * Copyright (C) 2002 Intel Corp.
 * Copyright (C) 2003-2004 MontaVista Software, Inc.
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 */

#ifndef __ENP2611_H
#define __ENP2611_H

#define ENP2611_CALEB_PHYS_BASE		0xc5000000
#define ENP2611_CALEB_VIRT_BASE		0xfe000000
#define ENP2611_CALEB_SIZE		0x00100000

#define ENP2611_PM3386_0_PHYS_BASE	0xc6000000
#define ENP2611_PM3386_0_VIRT_BASE	0xfe100000
#define ENP2611_PM3386_0_SIZE		0x00100000

#define ENP2611_PM3386_1_PHYS_BASE	0xc6400000
#define ENP2611_PM3386_1_VIRT_BASE	0xfe200000
#define ENP2611_PM3386_1_SIZE		0x00100000

#define ENP2611_GPIO_SCL		7
#define ENP2611_GPIO_SDA		6

#define IRQ_ENP2611_THERMAL		IRQ_IXP2000_GPIO4
#define IRQ_ENP2611_OPTION_BOARD	IRQ_IXP2000_GPIO3
#define IRQ_ENP2611_CALEB		IRQ_IXP2000_GPIO2
#define IRQ_ENP2611_PM3386_1		IRQ_IXP2000_GPIO1
#define IRQ_ENP2611_PM3386_0		IRQ_IXP2000_GPIO0


#endif
