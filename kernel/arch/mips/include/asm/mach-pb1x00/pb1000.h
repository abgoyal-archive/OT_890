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
 * Alchemy Semi Pb1000 Referrence Board
 *
 * Copyright 2001, 2008 MontaVista Software Inc.
 * Author: MontaVista Software, Inc. <source@mvista.com>
 *
 * ########################################################################
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
 *
 * ########################################################################
 *
 *
 */
#ifndef __ASM_PB1000_H
#define __ASM_PB1000_H

/* PCMCIA PB1000 specific defines */
#define PCMCIA_MAX_SOCK  1
#define PCMCIA_NUM_SOCKS (PCMCIA_MAX_SOCK + 1)

#define PB1000_PCR		0xBE000000
#  define PCR_SLOT_0_VPP0	(1 << 0)
#  define PCR_SLOT_0_VPP1	(1 << 1)
#  define PCR_SLOT_0_VCC0	(1 << 2)
#  define PCR_SLOT_0_VCC1	(1 << 3)
#  define PCR_SLOT_0_RST	(1 << 4)
#  define PCR_SLOT_1_VPP0	(1 << 8)
#  define PCR_SLOT_1_VPP1	(1 << 9)
#  define PCR_SLOT_1_VCC0	(1 << 10)
#  define PCR_SLOT_1_VCC1	(1 << 11)
#  define PCR_SLOT_1_RST	(1 << 12)

#define PB1000_MDR		0xBE000004
#  define MDR_PI		(1 << 5)	/* PCMCIA int latch  */
#  define MDR_EPI		(1 << 14)	/* enable PCMCIA int */
#  define MDR_CPI		(1 << 15)	/* clear  PCMCIA int  */

#define PB1000_ACR1		0xBE000008
#  define ACR1_SLOT_0_CD1	(1 << 0)	/* card detect 1	*/
#  define ACR1_SLOT_0_CD2	(1 << 1)	/* card detect 2	*/
#  define ACR1_SLOT_0_READY	(1 << 2)	/* ready		*/
#  define ACR1_SLOT_0_STATUS	(1 << 3)	/* status change	*/
#  define ACR1_SLOT_0_VS1	(1 << 4)	/* voltage sense 1	*/
#  define ACR1_SLOT_0_VS2	(1 << 5)	/* voltage sense 2	*/
#  define ACR1_SLOT_0_INPACK	(1 << 6)	/* inpack pin status	*/
#  define ACR1_SLOT_1_CD1	(1 << 8)	/* card detect 1	*/
#  define ACR1_SLOT_1_CD2	(1 << 9)	/* card detect 2	*/
#  define ACR1_SLOT_1_READY	(1 << 10)	/* ready		*/
#  define ACR1_SLOT_1_STATUS	(1 << 11)	/* status change	*/
#  define ACR1_SLOT_1_VS1	(1 << 12)	/* voltage sense 1	*/
#  define ACR1_SLOT_1_VS2	(1 << 13)	/* voltage sense 2	*/
#  define ACR1_SLOT_1_INPACK	(1 << 14)	/* inpack pin status	*/

#define CPLD_AUX0		0xBE00000C
#define CPLD_AUX1		0xBE000010
#define CPLD_AUX2		0xBE000014

/* Voltage levels */

/* VPPEN1 - VPPEN0 */
#define VPP_GND ((0 << 1) | (0 << 0))
#define VPP_5V	((1 << 1) | (0 << 0))
#define VPP_3V	((0 << 1) | (1 << 0))
#define VPP_12V ((0 << 1) | (1 << 0))
#define VPP_HIZ ((1 << 1) | (1 << 0))

/* VCCEN1 - VCCEN0 */
#define VCC_3V	((0 << 1) | (1 << 0))
#define VCC_5V	((1 << 1) | (0 << 0))
#define VCC_HIZ ((0 << 1) | (0 << 0))

/* VPP/VCC */
#define SET_VCC_VPP(VCC, VPP, SLOT) \
	((((VCC) << 2) | ((VPP) << 0)) << ((SLOT) * 8))
#endif /* __ASM_PB1000_H */
