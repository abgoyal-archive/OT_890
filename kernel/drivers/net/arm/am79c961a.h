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
 * linux/drivers/net/arm/am79c961a.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _LINUX_am79c961a_H
#define _LINUX_am79c961a_H

/* use 0 for production, 1 for verification, >2 for debug. debug flags: */
#define DEBUG_TX	 2
#define DEBUG_RX	 4
#define DEBUG_INT	 8
#define DEBUG_IC	16
#ifndef NET_DEBUG
#define NET_DEBUG 	0
#endif

#define NET_UID		0
#define NET_RDP		0x10
#define NET_RAP		0x12
#define NET_RESET	0x14
#define NET_IDP		0x16

/*
 * RAP registers
 */
#define CSR0		0
#define CSR0_INIT	0x0001
#define CSR0_STRT	0x0002
#define CSR0_STOP	0x0004
#define CSR0_TDMD	0x0008
#define CSR0_TXON	0x0010
#define CSR0_RXON	0x0020
#define CSR0_IENA	0x0040
#define CSR0_INTR	0x0080
#define CSR0_IDON	0x0100
#define CSR0_TINT	0x0200
#define CSR0_RINT	0x0400
#define CSR0_MERR	0x0800
#define CSR0_MISS	0x1000
#define CSR0_CERR	0x2000
#define CSR0_BABL	0x4000
#define CSR0_ERR	0x8000

#define CSR3		3
#define CSR3_EMBA	0x0008
#define CSR3_DXMT2PD	0x0010
#define CSR3_LAPPEN	0x0020
#define CSR3_DXSUFLO	0x0040
#define CSR3_IDONM	0x0100
#define CSR3_TINTM	0x0200
#define CSR3_RINTM	0x0400
#define CSR3_MERRM	0x0800
#define CSR3_MISSM	0x1000
#define CSR3_BABLM	0x4000
#define CSR3_MASKALL	0x5F00

#define CSR4		4
#define CSR4_JABM	0x0001
#define CSR4_JAB	0x0002
#define CSR4_TXSTRTM	0x0004
#define CSR4_TXSTRT	0x0008
#define CSR4_RCVCCOM	0x0010
#define CSR4_RCVCCO	0x0020
#define CSR4_MFCOM	0x0100
#define CSR4_MFCO	0x0200
#define CSR4_ASTRP_RCV	0x0400
#define CSR4_APAD_XMIT	0x0800

#define CTRL1		5
#define CTRL1_SPND	0x0001

#define LADRL		8
#define LADRM1		9
#define LADRM2		10
#define LADRH		11
#define PADRL		12
#define PADRM		13
#define PADRH		14

#define MODE		15
#define MODE_DISRX	0x0001
#define MODE_DISTX	0x0002
#define MODE_LOOP	0x0004
#define MODE_DTCRC	0x0008
#define MODE_COLL	0x0010
#define MODE_DRETRY	0x0020
#define MODE_INTLOOP	0x0040
#define MODE_PORT_AUI	0x0000
#define MODE_PORT_10BT	0x0080
#define MODE_DRXPA	0x2000
#define MODE_DRXBA	0x4000
#define MODE_PROMISC	0x8000

#define BASERXL		24
#define BASERXH		25
#define BASETXL		30
#define BASETXH		31

#define POLLINT		47

#define SIZERXR		76
#define SIZETXR		78

#define CSR_MFC		112

#define RMD_ENP		0x0100
#define RMD_STP		0x0200
#define RMD_CRC		0x0800
#define RMD_FRAM	0x2000
#define RMD_ERR		0x4000
#define RMD_OWN		0x8000

#define TMD_ENP		0x0100
#define TMD_STP		0x0200
#define TMD_MORE	0x1000
#define TMD_ERR		0x4000
#define TMD_OWN		0x8000

#define TST_RTRY	0x0400
#define TST_LCAR	0x0800
#define TST_LCOL	0x1000
#define TST_UFLO	0x4000
#define TST_BUFF	0x8000

#define ISALED0		0x0004
#define ISALED0_LNKST	0x8000

struct dev_priv {
    struct net_device_stats stats;
    unsigned long	rxbuffer[RX_BUFFERS];
    unsigned long	txbuffer[TX_BUFFERS];
    unsigned char	txhead;
    unsigned char	txtail;
    unsigned char	rxhead;
    unsigned char	rxtail;
    unsigned long	rxhdr;
    unsigned long	txhdr;
    spinlock_t		chip_lock;
    struct timer_list	timer;
};

#endif
