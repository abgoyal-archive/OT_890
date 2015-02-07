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

#ifndef __ARCH_ORION5X_COMMON_H
#define __ARCH_ORION5X_COMMON_H

struct dsa_platform_data;
struct mv643xx_eth_platform_data;
struct mv_sata_platform_data;

/*
 * Basic Orion init functions used early by machine-setup.
 */
void orion5x_map_io(void);
void orion5x_init_irq(void);
void orion5x_init(void);
extern int orion5x_tclk;
extern struct sys_timer orion5x_timer;

/*
 * Enumerations and functions for Orion windows mapping. Used by Orion core
 * functions to map its interfaces and by the machine-setup to map its on-
 * board devices. Details in /mach-orion/addr-map.c
 */
extern struct mbus_dram_target_info orion5x_mbus_dram_info;
void orion5x_setup_cpu_mbus_bridge(void);
void orion5x_setup_dev_boot_win(u32 base, u32 size);
void orion5x_setup_dev0_win(u32 base, u32 size);
void orion5x_setup_dev1_win(u32 base, u32 size);
void orion5x_setup_dev2_win(u32 base, u32 size);
void orion5x_setup_pcie_wa_win(u32 base, u32 size);

void orion5x_ehci0_init(void);
void orion5x_ehci1_init(void);
void orion5x_eth_init(struct mv643xx_eth_platform_data *eth_data);
void orion5x_eth_switch_init(struct dsa_platform_data *d, int irq);
void orion5x_i2c_init(void);
void orion5x_sata_init(struct mv_sata_platform_data *sata_data);
void orion5x_spi_init(void);
void orion5x_uart0_init(void);
void orion5x_uart1_init(void);
void orion5x_xor_init(void);

/*
 * PCIe/PCI functions.
 */
struct pci_bus;
struct pci_sys_data;

void orion5x_pcie_id(u32 *dev, u32 *rev);
void orion5x_pci_disable(void);
void orion5x_pci_set_cardbus_mode(void);
int orion5x_pci_sys_setup(int nr, struct pci_sys_data *sys);
struct pci_bus *orion5x_pci_sys_scan_bus(int nr, struct pci_sys_data *sys);
int orion5x_pci_map_irq(struct pci_dev *dev, u8 slot, u8 pin);

struct machine_desc;
struct meminfo;
struct tag;
extern void __init tag_fixup_mem32(struct machine_desc *, struct tag *,
				   char **, struct meminfo *);


#endif
