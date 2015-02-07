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
 * arch/sh/drivers/pci/fixups-r7780rp.c
 *
 * Highlander R7780RP-1 PCI fixups
 *
 * Copyright (C) 2003  Lineo uSolutions, Inc.
 * Copyright (C) 2004 - 2006  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/pci.h>
#include "pci-sh4.h"
#include <asm/io.h>

int pci_fixup_pcic(void)
{
	pci_write_reg(0x000043ff, SH4_PCIINTM);
	pci_write_reg(0x0000380f, SH4_PCIAINTM);

	pci_write_reg(0xfbb00047, SH7780_PCICMD);
	pci_write_reg(0x00000000, SH7780_PCIIBAR);

	pci_write_reg(0x00011912, SH7780_PCISVID);
	pci_write_reg(0x08000000, SH7780_PCICSCR0);
	pci_write_reg(0x0000001b, SH7780_PCICSAR0);
	pci_write_reg(0xfd000000, SH7780_PCICSCR1);
	pci_write_reg(0x0000000f, SH7780_PCICSAR1);

	pci_write_reg(0xfd000000, SH7780_PCIMBR0);
	pci_write_reg(0x00fc0000, SH7780_PCIMBMR0);

#ifdef CONFIG_32BIT
	pci_write_reg(0xc0000000, SH7780_PCIMBR2);
	pci_write_reg(0x20000000 - SH7780_PCI_IO_SIZE, SH7780_PCIMBMR2);
#endif

	/* Set IOBR for windows containing area specified in pci.h */
	pci_write_reg((PCIBIOS_MIN_IO & ~(SH7780_PCI_IO_SIZE - 1)),
		      SH7780_PCIIOBR);
	pci_write_reg(((SH7780_PCI_IO_SIZE-1) & (7<<18)), SH7780_PCIIOBMR);

	return 0;
}
