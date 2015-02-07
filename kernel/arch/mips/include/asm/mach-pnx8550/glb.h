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
 *
 * BRIEF MODULE DESCRIPTION
 *   PNX8550 global definitions
 *
 * Author: source@mvista.com
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */

#ifndef __PNX8550_GLB_H
#define __PNX8550_GLB_H

#define PNX8550_GLB1_BASE	0xBBE63000
#define PNX8550_GLB2_BASE	0xBBE4d000
#define PNX8550_RESET_BASE      0xBBE60000

/* PCI Inta Output Enable Registers */
#define PNX8550_GLB2_ENAB_INTA_O	*(volatile unsigned long *)(PNX8550_GLB2_BASE + 0x050)

/* Bit 1:Enable DAC Powerdown
     0:DACs are enabled and are working normally
     1:DACs are powerdown
*/
#define PNX8550_GLB_DAC_PD      0x2
/*   Bit 0:Enable of PCI inta output
     0 = Disable PCI inta output
     1 = Enable PCI inta output
*/
#define PNX8550_GLB_ENABLE_INTA_O 0x1

/* PCI Direct Mappings */
#define PNX8550_PCIMEM	        0x12000000
#define PNX8550_PCIMEM_SIZE	0x08000000
#define PNX8550_PCIIO	        0x1c000000
#define PNX8550_PCIIO_SIZE	0x02000000	/* 32M */

#define PNX8550_PORT_BASE	KSEG1

// GPIO def
#define PNX8550_GPIO_BASE	0x1Be00000

#define PNX8550_GPIO_DIRQ0	 (PNX8550_GPIO_BASE + 0x104500)
#define PNX8550_GPIO_MC1         (PNX8550_GPIO_BASE + 0x104004)
#define PNX8550_GPIO_MC_31_BIT   30
#define PNX8550_GPIO_MC_30_BIT   28
#define PNX8550_GPIO_MC_29_BIT   26
#define PNX8550_GPIO_MC_28_BIT   24
#define PNX8550_GPIO_MC_27_BIT   22
#define PNX8550_GPIO_MC_26_BIT   20
#define PNX8550_GPIO_MC_25_BIT   18
#define PNX8550_GPIO_MC_24_BIT   16
#define PNX8550_GPIO_MC_23_BIT   14
#define PNX8550_GPIO_MC_22_BIT   12
#define PNX8550_GPIO_MC_21_BIT   10
#define PNX8550_GPIO_MC_20_BIT   8
#define PNX8550_GPIO_MC_19_BIT   6
#define PNX8550_GPIO_MC_18_BIT   4
#define PNX8550_GPIO_MC_17_BIT   2
#define PNX8550_GPIO_MC_16_BIT   0

#define PNX8550_GPIO_MODE_PRIMOP    0x1
#define PNX8550_GPIO_MODE_NO_OPENDR 0x2
#define PNX8550_GPIO_MODE_OPENDR    0x3

// RESET module
#define PNX8550_RST_CTL             *(volatile unsigned long *)(PNX8550_RESET_BASE + 0x0)
#define PNX8550_RST_CAUSE           *(volatile unsigned long *)(PNX8550_RESET_BASE + 0x4)
#define PNX8550_RST_EN_WATCHDOG     *(volatile unsigned long *)(PNX8550_RESET_BASE + 0x8)

#define PNX8550_RST_REL_MIPS_RST_N     0x8
#define PNX8550_RST_DO_SW_RST          0x4
#define PNX8550_RST_REL_SYS_RST_OUT    0x2
#define PNX8550_RST_ASSERT_SYS_RST_OUT 0x1
#endif
