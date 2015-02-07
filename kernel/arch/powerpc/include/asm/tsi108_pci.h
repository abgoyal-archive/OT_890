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
 * Copyright 2007 IBM Corp
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _ASM_POWERPC_TSI108_PCI_H
#define _ASM_POWERPC_TSI108_PCI_H

#include <asm/tsi108.h>

/* Register definitions */
#define TSI108_PCI_P2O_BAR0 (TSI108_PCI_OFFSET + 0x10)
#define TSI108_PCI_P2O_BAR0_UPPER (TSI108_PCI_OFFSET + 0x14)
#define TSI108_PCI_P2O_BAR2 (TSI108_PCI_OFFSET + 0x18)
#define TSI108_PCI_P2O_BAR2_UPPER (TSI108_PCI_OFFSET + 0x1c)
#define TSI108_PCI_P2O_PAGE_SIZES (TSI108_PCI_OFFSET + 0x4c)
#define TSI108_PCI_PFAB_BAR0 (TSI108_PCI_OFFSET + 0x204)
#define TSI108_PCI_PFAB_BAR0_UPPER (TSI108_PCI_OFFSET + 0x208)
#define TSI108_PCI_PFAB_IO (TSI108_PCI_OFFSET + 0x20c)
#define TSI108_PCI_PFAB_IO_UPPER (TSI108_PCI_OFFSET + 0x210)
#define TSI108_PCI_PFAB_MEM32 (TSI108_PCI_OFFSET + 0x214)
#define TSI108_PCI_PFAB_PFM3 (TSI108_PCI_OFFSET + 0x220)
#define TSI108_PCI_PFAB_PFM4 (TSI108_PCI_OFFSET + 0x230)

extern int tsi108_setup_pci(struct device_node *dev, u32 cfg_phys, int primary);
extern void tsi108_pci_int_init(struct device_node *node);
extern void tsi108_irq_cascade(unsigned int irq, struct irq_desc *desc);
extern void tsi108_clear_pci_cfg_error(void);

#endif				/*  _ASM_POWERPC_TSI108_PCI_H */
