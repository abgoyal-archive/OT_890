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
 * EDAC defs for Marvell MV64x60 bridge chip
 *
 * Author: Dave Jiang <djiang@mvista.com>
 *
 * 2007 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 *
 */
#ifndef _MV64X60_EDAC_H_
#define _MV64X60_EDAC_H_

#define MV64x60_REVISION " Ver: 2.0.0 " __DATE__
#define EDAC_MOD_STR	"MV64x60_edac"

#define mv64x60_printk(level, fmt, arg...) \
	edac_printk(level, "MV64x60", fmt, ##arg)

#define mv64x60_mc_printk(mci, level, fmt, arg...) \
	edac_mc_chipset_printk(mci, level, "MV64x60", fmt, ##arg)

/* CPU Error Report Registers */
#define MV64x60_CPU_ERR_ADDR_LO		0x00	/* 0x0070 */
#define MV64x60_CPU_ERR_ADDR_HI		0x08	/* 0x0078 */
#define MV64x60_CPU_ERR_DATA_LO		0x00	/* 0x0128 */
#define MV64x60_CPU_ERR_DATA_HI		0x08	/* 0x0130 */
#define MV64x60_CPU_ERR_PARITY		0x10	/* 0x0138 */
#define MV64x60_CPU_ERR_CAUSE		0x18	/* 0x0140 */
#define MV64x60_CPU_ERR_MASK		0x20	/* 0x0148 */

#define MV64x60_CPU_CAUSE_MASK		0x07ffffff

/* SRAM Error Report Registers */
#define MV64X60_SRAM_ERR_CAUSE		0x08	/* 0x0388 */
#define MV64X60_SRAM_ERR_ADDR_LO	0x10	/* 0x0390 */
#define MV64X60_SRAM_ERR_ADDR_HI	0x78	/* 0x03f8 */
#define MV64X60_SRAM_ERR_DATA_LO	0x18	/* 0x0398 */
#define MV64X60_SRAM_ERR_DATA_HI	0x20	/* 0x03a0 */
#define MV64X60_SRAM_ERR_PARITY		0x28	/* 0x03a8 */

/* SDRAM Controller Registers */
#define MV64X60_SDRAM_CONFIG		0x00	/* 0x1400 */
#define MV64X60_SDRAM_ERR_DATA_HI	0x40	/* 0x1440 */
#define MV64X60_SDRAM_ERR_DATA_LO	0x44	/* 0x1444 */
#define MV64X60_SDRAM_ERR_ECC_RCVD	0x48	/* 0x1448 */
#define MV64X60_SDRAM_ERR_ECC_CALC	0x4c	/* 0x144c */
#define MV64X60_SDRAM_ERR_ADDR		0x50	/* 0x1450 */
#define MV64X60_SDRAM_ERR_ECC_CNTL	0x54	/* 0x1454 */
#define MV64X60_SDRAM_ERR_ECC_ERR_CNT	0x58	/* 0x1458 */

#define MV64X60_SDRAM_REGISTERED	0x20000
#define MV64X60_SDRAM_ECC		0x40000

#ifdef CONFIG_PCI
/*
 * Bit 0 of MV64x60_PCIx_ERR_MASK does not exist on the 64360 and because of
 * errata FEr-#11 and FEr-##16 for the 64460, it should be 0 on that chip as
 * well.  IOW, don't set bit 0.
 */
#define MV64X60_PCIx_ERR_MASK_VAL	0x00a50c24

/* Register offsets from PCIx error address low register */
#define MV64X60_PCI_ERROR_ADDR_LO	0x00
#define MV64X60_PCI_ERROR_ADDR_HI	0x04
#define MV64X60_PCI_ERROR_ATTR		0x08
#define MV64X60_PCI_ERROR_CMD		0x10
#define MV64X60_PCI_ERROR_CAUSE		0x18
#define MV64X60_PCI_ERROR_MASK		0x1c

#define MV64X60_PCI_ERR_SWrPerr		0x0002
#define MV64X60_PCI_ERR_SRdPerr		0x0004
#define	MV64X60_PCI_ERR_MWrPerr		0x0020
#define MV64X60_PCI_ERR_MRdPerr		0x0040

#define MV64X60_PCI_PE_MASK	(MV64X60_PCI_ERR_SWrPerr | \
				MV64X60_PCI_ERR_SRdPerr | \
				MV64X60_PCI_ERR_MWrPerr | \
				MV64X60_PCI_ERR_MRdPerr)

struct mv64x60_pci_pdata {
	int pci_hose;
	void __iomem *pci_vbase;
	char *name;
	int irq;
	int edac_idx;
};

#endif				/* CONFIG_PCI */

struct mv64x60_mc_pdata {
	void __iomem *mc_vbase;
	int total_mem;
	char *name;
	int irq;
	int edac_idx;
};

struct mv64x60_cpu_pdata {
	void __iomem *cpu_vbase[2];
	char *name;
	int irq;
	int edac_idx;
};

struct mv64x60_sram_pdata {
	void __iomem *sram_vbase;
	char *name;
	int irq;
	int edac_idx;
};

#endif
