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
 * DSM-G600 platform specific definitions
 *
 * Copyright (C) 2006 Tower Technologies
 * Author: Alessandro Zummo <a.zummo@towertech.it>
 *
 * based on ixdp425.h:
 *	Copyright 2004 (C) MontaVista, Software, Inc.
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef __ASM_ARCH_HARDWARE_H__
#error "Do not include this directly, instead #include <mach/hardware.h>"
#endif

#define DSMG600_SDA_PIN		5
#define DSMG600_SCL_PIN		4

/*
 * DSMG600 PCI IRQs
 */
#define DSMG600_PCI_MAX_DEV	4
#define DSMG600_PCI_IRQ_LINES	3


/* PCI controller GPIO to IRQ pin mappings */
#define DSMG600_PCI_INTA_PIN	11
#define DSMG600_PCI_INTB_PIN	10
#define DSMG600_PCI_INTC_PIN	9
#define DSMG600_PCI_INTD_PIN	8
#define DSMG600_PCI_INTE_PIN	7
#define DSMG600_PCI_INTF_PIN	6

/* DSM-G600 Timer Setting */
#define DSMG600_FREQ 66000000

/* Buttons */

#define DSMG600_PB_GPIO		15	/* power button */
#define DSMG600_RB_GPIO		3	/* reset button */

/* Power control */

#define DSMG600_PO_GPIO		2	/* power off */

/* LEDs */

#define DSMG600_LED_PWR_GPIO	0
#define DSMG600_LED_WLAN_GPIO	14
