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
 * arch/arm/mach-mv78xx0/common.h
 *
 * Core functions for Marvell MV78xx0 SoCs
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef __ARCH_MV78XX0_COMMON_H
#define __ARCH_MV78XX0_COMMON_H

struct mv643xx_eth_platform_data;
struct mv_sata_platform_data;

/*
 * Basic MV78xx0 init functions used early by machine-setup.
 */
int mv78xx0_core_index(void);
void mv78xx0_map_io(void);
void mv78xx0_init(void);
void mv78xx0_init_irq(void);

extern struct mbus_dram_target_info mv78xx0_mbus_dram_info;
void mv78xx0_setup_cpu_mbus(void);
void mv78xx0_setup_pcie_io_win(int window, u32 base, u32 size,
			       int maj, int min);
void mv78xx0_setup_pcie_mem_win(int window, u32 base, u32 size,
				int maj, int min);

void mv78xx0_ehci0_init(void);
void mv78xx0_ehci1_init(void);
void mv78xx0_ehci2_init(void);
void mv78xx0_ge00_init(struct mv643xx_eth_platform_data *eth_data);
void mv78xx0_ge01_init(struct mv643xx_eth_platform_data *eth_data);
void mv78xx0_ge10_init(struct mv643xx_eth_platform_data *eth_data);
void mv78xx0_ge11_init(struct mv643xx_eth_platform_data *eth_data);
void mv78xx0_pcie_init(int init_port0, int init_port1);
void mv78xx0_sata_init(struct mv_sata_platform_data *sata_data);
void mv78xx0_uart0_init(void);
void mv78xx0_uart1_init(void);
void mv78xx0_uart2_init(void);
void mv78xx0_uart3_init(void);

extern struct sys_timer mv78xx0_timer;


#endif
