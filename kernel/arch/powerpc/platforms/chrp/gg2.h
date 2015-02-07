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
 *  include/asm-ppc/gg2.h -- VLSI VAS96011/12 `Golden Gate 2' register definitions
 *
 *  Copyright (C) 1997 Geert Uytterhoeven
 *
 *  This file is based on the following documentation:
 *
 *	The VAS96011/12 Chipset, Data Book, Edition 1.0
 *	VLSI Technology, Inc.
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive
 *  for more details.
 */

#ifndef _ASMPPC_GG2_H
#define _ASMPPC_GG2_H

    /*
     *  Memory Map (CHRP mode)
     */

#define GG2_PCI_MEM_BASE	0xc0000000	/* Peripheral memory space */
#define GG2_ISA_MEM_BASE	0xf7000000	/* Peripheral memory alias */
#define GG2_ISA_IO_BASE		0xf8000000	/* Peripheral I/O space */
#define GG2_PCI_CONFIG_BASE	0xfec00000	/* PCI configuration space */
#define GG2_INT_ACK_SPECIAL	0xfec80000	/* Interrupt acknowledge and */
						/* special PCI cycles */
#define GG2_ROM_BASE0		0xff000000	/* ROM bank 0 */
#define GG2_ROM_BASE1		0xff800000	/* ROM bank 1 */


    /*
     *  GG2 specific PCI Registers
     */

extern void __iomem *gg2_pci_config_base;	/* kernel virtual address */

#define GG2_PCI_BUSNO		0x40	/* Bus number */
#define GG2_PCI_SUBBUSNO	0x41	/* Subordinate bus number */
#define GG2_PCI_DISCCTR		0x42	/* Disconnect counter */
#define GG2_PCI_PPC_CTRL	0x50	/* PowerPC interface control register */
#define GG2_PCI_ADDR_MAP	0x5c	/* Address map */
#define GG2_PCI_PCI_CTRL	0x60	/* PCI interface control register */
#define GG2_PCI_ROM_CTRL	0x70	/* ROM interface control register */
#define GG2_PCI_ROM_TIME	0x74	/* ROM timing */
#define GG2_PCI_CC_CTRL		0x80	/* Cache controller control register */
#define GG2_PCI_DRAM_BANK0	0x90	/* Control register for DRAM bank #0 */
#define GG2_PCI_DRAM_BANK1	0x94	/* Control register for DRAM bank #1 */
#define GG2_PCI_DRAM_BANK2	0x98	/* Control register for DRAM bank #2 */
#define GG2_PCI_DRAM_BANK3	0x9c	/* Control register for DRAM bank #3 */
#define GG2_PCI_DRAM_BANK4	0xa0	/* Control register for DRAM bank #4 */
#define GG2_PCI_DRAM_BANK5	0xa4	/* Control register for DRAM bank #5 */
#define GG2_PCI_DRAM_TIME0	0xb0	/* Timing parameters set #0 */
#define GG2_PCI_DRAM_TIME1	0xb4	/* Timing parameters set #1 */
#define GG2_PCI_DRAM_CTRL	0xc0	/* DRAM control */
#define GG2_PCI_ERR_CTRL	0xd0	/* Error control register */
#define GG2_PCI_ERR_STATUS	0xd4	/* Error status register */
					/* Cleared when read */

#endif /* _ASMPPC_GG2_H */
