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
 * arch/arm/mach-ixp23xx/include/mach/platform.h
 *
 * Various bits of code used by platform-level code.
 *
 * Author: Deepak Saxena <dsaxena@plexity.net>
 *
 * Copyright 2005 (c) MontaVista Software, Inc.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef __ASSEMBLY__

static inline unsigned long ixp2000_reg_read(volatile void *reg)
{
	return *((volatile unsigned long *)reg);
}

static inline void ixp2000_reg_write(volatile void *reg, unsigned long val)
{
	*((volatile unsigned long *)reg) = val;
}

static inline void ixp2000_reg_wrb(volatile void *reg, unsigned long val)
{
	*((volatile unsigned long *)reg) = val;
}

struct pci_sys_data;

void ixp23xx_map_io(void);
void ixp23xx_init_irq(void);
void ixp23xx_sys_init(void);
int ixp23xx_pci_setup(int, struct pci_sys_data *);
void ixp23xx_pci_preinit(void);
struct pci_bus *ixp23xx_pci_scan_bus(int, struct pci_sys_data*);
void ixp23xx_pci_slave_init(void);

extern struct sys_timer ixp23xx_timer;

#define IXP23XX_UART_XTAL		14745600

#ifndef __ASSEMBLY__
/*
 * Is system memory on the XSI or CPP bus?
 */
static inline unsigned ixp23xx_cpp_boot(void)
{
	return (*IXP23XX_EXP_CFG0 & IXP23XX_EXP_CFG0_XSI_NOT_PRES);
}
#endif


#endif
