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

/* mb86943a.h: MB86943 SPARClite <-> PCI bridge registers
 *
 * Copyright (C) 2003 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef _ASM_MB86943A_H
#define _ASM_MB86943A_H

#include <asm/mb-regs.h>

#define __reg_MB86943_sl_ctl		*(volatile uint32_t *) (__region_CS1 + 0x00)

#define MB86943_SL_CTL_BUS_WIDTH_64	0x00000001
#define MB86943_SL_CTL_AS_HOST		0x00000002
#define MB86943_SL_CTL_DRCT_MASTER_SWAP	0x00000004
#define MB86943_SL_CTL_DRCT_SLAVE_SWAP	0x00000008
#define MB86943_SL_CTL_PCI_CONFIG_SWAP	0x00000010
#define MB86943_SL_CTL_ECS0_ENABLE	0x00000020
#define MB86943_SL_CTL_ECS1_ENABLE	0x00000040
#define MB86943_SL_CTL_ECS2_ENABLE	0x00000080

#define __reg_MB86943_ecs_ctl(N)	*(volatile uint32_t *) (__region_CS1 + 0x08 + (0x08*(N)))
#define __reg_MB86943_ecs_range(N)	*(volatile uint32_t *) (__region_CS1 + 0x20 + (0x10*(N)))
#define __reg_MB86943_ecs_base(N)	*(volatile uint32_t *) (__region_CS1 + 0x28 + (0x10*(N)))

#define __reg_MB86943_sl_pci_io_range	*(volatile uint32_t *) (__region_CS1 + 0x50)
#define __reg_MB86943_sl_pci_io_base	*(volatile uint32_t *) (__region_CS1 + 0x58)
#define __reg_MB86943_sl_pci_mem_range	*(volatile uint32_t *) (__region_CS1 + 0x60)
#define __reg_MB86943_sl_pci_mem_base	*(volatile uint32_t *) (__region_CS1 + 0x68)
#define __reg_MB86943_pci_sl_io_base	*(volatile uint32_t *) (__region_CS1 + 0x70)
#define __reg_MB86943_pci_sl_mem_base	*(volatile uint32_t *) (__region_CS1 + 0x78)

#define __reg_MB86943_pci_arbiter	*(volatile uint32_t *) (__region_CS2 + 0x01300014)
#define MB86943_PCIARB_EN		0x00000001

#endif /* _ASM_MB86943A_H */
