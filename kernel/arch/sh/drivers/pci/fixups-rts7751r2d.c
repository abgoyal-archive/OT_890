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
 * arch/sh/drivers/pci/fixups-rts7751r2d.c
 *
 * RTS7751R2D PCI fixups
 *
 * Copyright (C) 2003  Lineo uSolutions, Inc.
 * Copyright (C) 2004  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include "pci-sh4.h"

#define PCIMCR_MRSET_OFF	0xBFFFFFFF
#define PCIMCR_RFSH_OFF		0xFFFFFFFB

int pci_fixup_pcic(void)
{
	unsigned long bcr1, mcr;

	bcr1 = ctrl_inl(SH7751_BCR1);
	bcr1 |= 0x40080000;	/* Enable Bit 19 BREQEN, set PCIC to slave */
	pci_write_reg(bcr1, SH4_PCIBCR1);

	/* Enable all interrupts, so we known what to fix */
	pci_write_reg(0x0000c3ff, SH4_PCIINTM);
	pci_write_reg(0x0000380f, SH4_PCIAINTM);

	pci_write_reg(0xfb900047, SH7751_PCICONF1);
	pci_write_reg(0xab000001, SH7751_PCICONF4);

	mcr = ctrl_inl(SH7751_MCR);
	mcr = (mcr & PCIMCR_MRSET_OFF) & PCIMCR_RFSH_OFF;
	pci_write_reg(mcr, SH4_PCIMCR);

	pci_write_reg(0x0c000000, SH7751_PCICONF5);
	pci_write_reg(0xd0000000, SH7751_PCICONF6);
	pci_write_reg(0x0c000000, SH4_PCILAR0);
	pci_write_reg(0x00000000, SH4_PCILAR1);

	return 0;
}
