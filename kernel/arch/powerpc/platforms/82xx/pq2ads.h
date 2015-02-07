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
 * PQ2/mpc8260 board-specific stuff
 *
 * A collection of structures, addresses, and values associated with
 * the Freescale MPC8260ADS/MPC8266ADS-PCI boards.
 * Copied from the RPX-Classic and SBS8260 stuff.
 *
 * Author: Vitaly Bordug <vbordug@ru.mvista.com>
 *
 * Originally written by Dan Malek for Motorola MPC8260 family
 *
 * Copyright (c) 2001 Dan Malek <dan@embeddedalley.com>
 * Copyright (c) 2006 MontaVista Software, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#ifdef __KERNEL__
#ifndef __MACH_ADS8260_DEFS
#define __MACH_ADS8260_DEFS

#include <linux/seq_file.h>

/* Backword-compatibility stuff for the drivers */
#define CPM_MAP_ADDR		((uint)0xf0000000)
#define CPM_IRQ_OFFSET 0

/* The ADS8260 has 16, 32-bit wide control/status registers, accessed
 * only on word boundaries.
 * Not all are used (yet), or are interesting to us (yet).
 */

/* Things of interest in the CSR.
 */
#define BCSR0_LED0		((uint)0x02000000)      /* 0 == on */
#define BCSR0_LED1		((uint)0x01000000)      /* 0 == on */
#define BCSR1_FETHIEN		((uint)0x08000000)      /* 0 == enable*/
#define BCSR1_FETH_RST		((uint)0x04000000)      /* 0 == reset */
#define BCSR1_RS232_EN1		((uint)0x02000000)      /* 0 ==enable */
#define BCSR1_RS232_EN2		((uint)0x01000000)      /* 0 ==enable */
#define BCSR3_FETHIEN2		((uint)0x10000000)      /* 0 == enable*/
#define BCSR3_FETH2_RST		((uint)0x80000000)      /* 0 == reset */

/* cpm serial driver works with constants below */

#define SIU_INT_SMC1		((uint)0x04+CPM_IRQ_OFFSET)
#define SIU_INT_SMC2		((uint)0x05+CPM_IRQ_OFFSET)
#define SIU_INT_SCC1		((uint)0x28+CPM_IRQ_OFFSET)
#define SIU_INT_SCC2		((uint)0x29+CPM_IRQ_OFFSET)
#define SIU_INT_SCC3		((uint)0x2a+CPM_IRQ_OFFSET)
#define SIU_INT_SCC4		((uint)0x2b+CPM_IRQ_OFFSET)

#endif /* __MACH_ADS8260_DEFS */
#endif /* __KERNEL__ */
