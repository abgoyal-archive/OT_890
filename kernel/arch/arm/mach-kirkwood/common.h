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
 * arch/arm/mach-kirkwood/common.h
 *
 * Core functions for Marvell Kirkwood SoCs
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef __ARCH_KIRKWOOD_COMMON_H
#define __ARCH_KIRKWOOD_COMMON_H

struct dsa_platform_data;
struct mv643xx_eth_platform_data;
struct mv_sata_platform_data;

/*
 * Basic Kirkwood init functions used early by machine-setup.
 */
void kirkwood_map_io(void);
void kirkwood_init(void);
void kirkwood_init_irq(void);

extern struct mbus_dram_target_info kirkwood_mbus_dram_info;
void kirkwood_setup_cpu_mbus(void);
void kirkwood_setup_sram_win(u32 base, u32 size);

void kirkwood_pcie_id(u32 *dev, u32 *rev);

void kirkwood_ehci_init(void);
void kirkwood_ge00_init(struct mv643xx_eth_platform_data *eth_data);
void kirkwood_ge01_init(struct mv643xx_eth_platform_data *eth_data);
void kirkwood_ge00_switch_init(struct dsa_platform_data *d, int irq);
void kirkwood_pcie_init(void);
void kirkwood_rtc_init(void);
void kirkwood_sata_init(struct mv_sata_platform_data *sata_data);
void kirkwood_spi_init(void);
void kirkwood_uart0_init(void);
void kirkwood_uart1_init(void);
void kirkwood_xor0_init(void);
void kirkwood_xor1_init(void);

extern struct sys_timer kirkwood_timer;


#endif
