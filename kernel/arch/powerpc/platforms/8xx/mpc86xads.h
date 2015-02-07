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
 * A collection of structures, addresses, and values associated with
 * the Freescale MPC86xADS board.
 * Copied from the FADS stuff.
 *
 * Author: MontaVista Software, Inc.
 *         source@mvista.com
 *
 * 2005 (c) MontaVista Software, Inc.  This file is licensed under the
 * terms of the GNU General Public License version 2.  This program is licensed
 * "as is" without any warranty of any kind, whether express or implied.
 */

#ifdef __KERNEL__
#ifndef __ASM_MPC86XADS_H__
#define __ASM_MPC86XADS_H__

/* Bits of interest in the BCSRs.
 */
#define BCSR1_ETHEN		((uint)0x20000000)
#define BCSR1_IRDAEN		((uint)0x10000000)
#define BCSR1_RS232EN_1		((uint)0x01000000)
#define BCSR1_PCCEN		((uint)0x00800000)
#define BCSR1_PCCVCC0		((uint)0x00400000)
#define BCSR1_PCCVPP0		((uint)0x00200000)
#define BCSR1_PCCVPP1		((uint)0x00100000)
#define BCSR1_PCCVPP_MASK	(BCSR1_PCCVPP0 | BCSR1_PCCVPP1)
#define BCSR1_RS232EN_2		((uint)0x00040000)
#define BCSR1_PCCVCC1		((uint)0x00010000)
#define BCSR1_PCCVCC_MASK	(BCSR1_PCCVCC0 | BCSR1_PCCVCC1)

#define BCSR4_ETH10_RST		((uint)0x80000000)	/* 10Base-T PHY reset*/
#define BCSR4_USB_LO_SPD	((uint)0x04000000)
#define BCSR4_USB_VCC		((uint)0x02000000)
#define BCSR4_USB_FULL_SPD	((uint)0x00040000)
#define BCSR4_USB_EN		((uint)0x00020000)

#define BCSR5_MII2_EN		0x40
#define BCSR5_MII2_RST		0x20
#define BCSR5_T1_RST		0x10
#define BCSR5_ATM155_RST	0x08
#define BCSR5_ATM25_RST		0x04
#define BCSR5_MII1_EN		0x02
#define BCSR5_MII1_RST		0x01

#endif /* __ASM_MPC86XADS_H__ */
#endif /* __KERNEL__ */
