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
 * arch/arm/mach-ks8695/include/mach/regs-pci.h
 *
 * Copyright (C) 2006 Ben Dooks <ben@simtec.co.uk>
 * Copyright (C) 2006 Simtec Electronics
 *
 * KS8695 - PCI bridge registers and bit definitions.
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#define KS8695_PCI_OFFSET	(0xF0000 + 0x2000)
#define KS8695_PCI_VA		(KS8695_IO_VA + KS8695_PCI_OFFSET)
#define KS8695_PCI_PA		(KS8695_IO_PA + KS8695_PCI_OFFSET)


#define KS8695_CRCFID		(0x000)		/* Configuration: Identification */
#define KS8695_CRCFCS		(0x004)		/* Configuration: Command and Status */
#define KS8695_CRCFRV		(0x008)		/* Configuration: Revision */
#define KS8695_CRCFLT		(0x00C)		/* Configuration: Latency Timer */
#define KS8695_CRCBMA		(0x010)		/* Configuration: Base Memory Address */
#define KS8695_CRCSID		(0x02C)		/* Configuration: Subsystem ID */
#define KS8695_CRCFIT		(0x03C)		/* Configuration: Interrupt */
#define KS8695_PBCA		(0x100)		/* Bridge Configuration Address */
#define KS8695_PBCD		(0x104)		/* Bridge Configuration Data */
#define KS8695_PBM		(0x200)		/* Bridge Mode */
#define KS8695_PBCS		(0x204)		/* Bridge Control and Status */
#define KS8695_PMBA		(0x208)		/* Bridge Memory Base Address */
#define KS8695_PMBAC		(0x20C)		/* Bridge Memory Base Address Control */
#define KS8695_PMBAM		(0x210)		/* Bridge Memory Base Address Mask */
#define KS8695_PMBAT		(0x214)		/* Bridge Memory Base Address Translation */
#define KS8695_PIOBA		(0x218)		/* Bridge I/O Base Address */
#define KS8695_PIOBAC		(0x21C)		/* Bridge I/O Base Address Control */
#define KS8695_PIOBAM		(0x220)		/* Bridge I/O Base Address Mask */
#define KS8695_PIOBAT		(0x224)		/* Bridge I/O Base Address Translation */


/* Configuration: Identification */

/* Configuration: Command and Status */

/* Configuration: Revision */



#define CFRV_GUEST		(1 << 23)

#define PBCA_TYPE1		(1)
#define PBCA_ENABLE		(1 << 31)


