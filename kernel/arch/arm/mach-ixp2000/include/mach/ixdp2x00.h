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
 * arch/arm/mach-ixp2000/include/mach/ixdp2x00.h
 *
 * Register and other defines for IXDP2[48]00 platforms
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
#ifndef _IXDP2X00_H_
#define _IXDP2X00_H_

/*
 * On board CPLD memory map
 */
#define IXDP2X00_PHYS_CPLD_BASE		0xc7000000
#define IXDP2X00_VIRT_CPLD_BASE		0xfe000000
#define IXDP2X00_CPLD_SIZE		0x00100000


#define IXDP2X00_CPLD_REG(x)  	\
	(volatile unsigned long *)(IXDP2X00_VIRT_CPLD_BASE | x)

/*
 * IXDP2400 CPLD registers
 */
#define IXDP2400_CPLD_SYSLED		IXDP2X00_CPLD_REG(0x0)  
#define IXDP2400_CPLD_DISP_DATA		IXDP2X00_CPLD_REG(0x4)
#define IXDP2400_CPLD_CLOCK_SPEED	IXDP2X00_CPLD_REG(0x8)
#define IXDP2400_CPLD_INT_STAT		IXDP2X00_CPLD_REG(0xc)
#define IXDP2400_CPLD_REV		IXDP2X00_CPLD_REG(0x10)
#define IXDP2400_CPLD_SYS_CLK_M		IXDP2X00_CPLD_REG(0x14)
#define IXDP2400_CPLD_SYS_CLK_N		IXDP2X00_CPLD_REG(0x18)
#define IXDP2400_CPLD_INT_MASK		IXDP2X00_CPLD_REG(0x48)

/*
 * IXDP2800 CPLD registers
 */
#define IXDP2800_CPLD_INT_STAT		IXDP2X00_CPLD_REG(0x0)
#define IXDP2800_CPLD_INT_MASK		IXDP2X00_CPLD_REG(0x140)


#define	IXDP2X00_GPIO_I2C_ENABLE	0x02
#define	IXDP2X00_GPIO_SCL		0x07
#define	IXDP2X00_GPIO_SDA		0x06

/*
 * PCI devfns for on-board devices. We need these to be able to
 * properly translate IRQs and for device removal.
 */
#define	IXDP2400_SLAVE_ENET_DEVFN	0x18	/* Bus 1 */
#define	IXDP2400_MASTER_ENET_DEVFN	0x20	/* Bus 1 */
#define	IXDP2400_MEDIA_DEVFN		0x28	/* Bus 1 */
#define	IXDP2400_SWITCH_FABRIC_DEVFN	0x30	/* Bus 1 */

#define	IXDP2800_SLAVE_ENET_DEVFN	0x20	/* Bus 1 */
#define	IXDP2800_MASTER_ENET_DEVFN	0x18	/* Bus 1 */
#define	IXDP2800_SWITCH_FABRIC_DEVFN	0x30	/* Bus 1 */

#define	IXDP2X00_P2P_DEVFN		0x20	/* Bus 0 */
#define	IXDP2X00_21555_DEVFN		0x30	/* Bus 0 */
#define IXDP2X00_SLAVE_NPU_DEVFN	0x28	/* Bus 1 */
#define	IXDP2X00_PMC_DEVFN		0x38	/* Bus 1 */
#define IXDP2X00_MASTER_NPU_DEVFN	0x38	/* Bus 1 */

#ifndef __ASSEMBLY__
/*
 * The master NPU is always PCI master.
 */
static inline unsigned int ixdp2x00_master_npu(void)
{
	return !!ixp2000_is_pcimaster();
}

/*
 * Helper functions used by ixdp2400 and ixdp2800 specific code
 */
void ixdp2x00_init_irq(volatile unsigned long*, volatile unsigned long *, unsigned long);
void ixdp2x00_slave_pci_postinit(void);
void ixdp2x00_init_machine(void);
void ixdp2x00_map_io(void);

#endif

#endif /*_IXDP2X00_H_ */
