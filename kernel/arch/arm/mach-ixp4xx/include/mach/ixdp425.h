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
 * arch/arm/mach-ixp4xx/include/mach/ixdp425.h
 *
 * IXDP425 platform specific definitions
 *
 * Author: Deepak Saxena <dsaxena@plexity.net>
 *
 * Copyright 2004 (c) MontaVista, Software, Inc. 
 * 
 * This file is licensed under  the terms of the GNU General Public 
 * License version 2. This program is licensed "as is" without any 
 * warranty of any kind, whether express or implied.
 */

#ifndef __ASM_ARCH_HARDWARE_H__
#error "Do not include this directly, instead #include <mach/hardware.h>"
#endif

#define	IXDP425_SDA_PIN		7
#define	IXDP425_SCL_PIN		6

/*
 * IXDP425 PCI IRQs
 */
#define IXDP425_PCI_MAX_DEV	4
#define IXDP425_PCI_IRQ_LINES	4


/* PCI controller GPIO to IRQ pin mappings */
#define IXDP425_PCI_INTA_PIN	11
#define IXDP425_PCI_INTB_PIN	10
#define	IXDP425_PCI_INTC_PIN	9
#define	IXDP425_PCI_INTD_PIN	8

/* NAND Flash pins */
#define	IXDP425_NAND_NCE_PIN	12

#define	IXDP425_NAND_CMD_BYTE	0x01
#define	IXDP425_NAND_ADDR_BYTE	0x02
