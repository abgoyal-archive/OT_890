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
 * cisreg.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * The initial developer of the original code is David A. Hinds
 * <dahinds@users.sourceforge.net>.  Portions created by David A. Hinds
 * are Copyright (C) 1999 David A. Hinds.  All Rights Reserved.
 *
 * (C) 1999             David A. Hinds
 */

#ifndef _LINUX_CISREG_H
#define _LINUX_CISREG_H

/*
 * Offsets from ConfigBase for CIS registers
 */
#define CISREG_COR		0x00
#define CISREG_CCSR		0x02
#define CISREG_PRR		0x04
#define CISREG_SCR		0x06
#define CISREG_ESR		0x08
#define CISREG_IOBASE_0		0x0a
#define CISREG_IOBASE_1		0x0c
#define CISREG_IOBASE_2		0x0e
#define CISREG_IOBASE_3		0x10
#define CISREG_IOSIZE		0x12

/*
 * Configuration Option Register
 */
#define COR_CONFIG_MASK		0x3f
#define COR_MFC_CONFIG_MASK	0x38
#define COR_FUNC_ENA		0x01
#define COR_ADDR_DECODE		0x02
#define COR_IREQ_ENA		0x04
#define COR_LEVEL_REQ		0x40
#define COR_SOFT_RESET		0x80

/*
 * Card Configuration and Status Register
 */
#define CCSR_INTR_ACK		0x01
#define CCSR_INTR_PENDING	0x02
#define CCSR_POWER_DOWN		0x04
#define CCSR_AUDIO_ENA		0x08
#define CCSR_IOIS8		0x20
#define CCSR_SIGCHG_ENA		0x40
#define CCSR_CHANGED		0x80

/*
 * Pin Replacement Register
 */
#define PRR_WP_STATUS		0x01
#define PRR_READY_STATUS	0x02
#define PRR_BVD2_STATUS		0x04
#define PRR_BVD1_STATUS		0x08
#define PRR_WP_EVENT		0x10
#define PRR_READY_EVENT		0x20
#define PRR_BVD2_EVENT		0x40
#define PRR_BVD1_EVENT		0x80

/*
 * Socket and Copy Register
 */
#define SCR_SOCKET_NUM		0x0f
#define SCR_COPY_NUM		0x70

/*
 * Extended Status Register
 */
#define ESR_REQ_ATTN_ENA	0x01
#define ESR_REQ_ATTN		0x10

/*
 * CardBus Function Status Registers
 */
#define CBFN_EVENT		0x00
#define CBFN_MASK		0x04
#define CBFN_STATE		0x08
#define CBFN_FORCE		0x0c

/*
 * These apply to all the CardBus function registers
 */
#define CBFN_WP			0x0001
#define CBFN_READY		0x0002
#define CBFN_BVD2		0x0004
#define CBFN_BVD1		0x0008
#define CBFN_GWAKE		0x0010
#define CBFN_INTR		0x8000

/*
 * Extra bits in the Function Event Mask Register
 */
#define FEMR_BAM_ENA		0x0020
#define FEMR_PWM_ENA		0x0040
#define FEMR_WKUP_MASK		0x4000

/*
 * Indirect Addressing Registers for Zoomed Video: these are addresses
 * in common memory space
 */
#define CISREG_ICTRL0		0x02	/* control registers */
#define CISREG_ICTRL1		0x03
#define CISREG_IADDR0		0x04	/* address registers */
#define CISREG_IADDR1		0x05
#define CISREG_IADDR2		0x06
#define CISREG_IADDR3		0x07
#define CISREG_IDATA0		0x08	/* data registers */
#define CISREG_IDATA1		0x09

#define ICTRL0_COMMON		0x01
#define ICTRL0_AUTOINC		0x02
#define ICTRL0_BYTEGRAN		0x04

#endif /* _LINUX_CISREG_H */
