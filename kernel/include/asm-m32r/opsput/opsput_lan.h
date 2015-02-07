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

#ifndef _OPSPUT_OPSPUT_LAN_H
#define _OPSPUT_OPSPUT_LAN_H

/*
 * include/asm-m32r/opsput/opsput_lan.h
 *
 * OPSPUT-LAN board
 *
 * Copyright (c) 2002-2004	Takeo Takahashi, Mamoru Sakugawa
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file "COPYING" in the main directory of
 * this archive for more details.
 */

#ifndef __ASSEMBLY__
/*
 * C functions use non-cache address.
 */
#define OPSPUT_LAN_BASE	(0x10000000 /* + NONCACHE_OFFSET */)
#else
#define OPSPUT_LAN_BASE	(0x10000000 + NONCACHE_OFFSET)
#endif /* __ASSEMBLY__ */

/* ICU
 *  ICUISTS:	status register
 *  ICUIREQ0: 	request register
 *  ICUIREQ1: 	request register
 *  ICUCR3:	control register for CFIREQ# interrupt
 *  ICUCR4:	control register for CFC Card insert interrupt
 *  ICUCR5:	control register for CFC Card eject interrupt
 *  ICUCR6:	control register for external interrupt
 *  ICUCR11:	control register for MMC Card insert/eject interrupt
 *  ICUCR13:	control register for SC error interrupt
 *  ICUCR14:	control register for SC receive interrupt
 *  ICUCR15:	control register for SC send interrupt
 *  ICUCR16:	control register for SIO0 receive interrupt
 *  ICUCR17:	control register for SIO0 send interrupt
 */
#define OPSPUT_LAN_IRQ_LAN	(OPSPUT_LAN_PLD_IRQ_BASE + 1)	/* LAN */
#define OPSPUT_LAN_IRQ_I2C	(OPSPUT_LAN_PLD_IRQ_BASE + 3)	/* I2C */

#define OPSPUT_LAN_ICUISTS	__reg16(OPSPUT_LAN_BASE + 0xc0002)
#define OPSPUT_LAN_ICUISTS_VECB_MASK	(0xf000)
#define OPSPUT_LAN_VECB(x)	((x) & OPSPUT_LAN_ICUISTS_VECB_MASK)
#define OPSPUT_LAN_ICUISTS_ISN_MASK	(0x07c0)
#define OPSPUT_LAN_ICUISTS_ISN(x)	((x) & OPSPUT_LAN_ICUISTS_ISN_MASK)
#define OPSPUT_LAN_ICUIREQ0	__reg16(OPSPUT_LAN_BASE + 0xc0004)
#define OPSPUT_LAN_ICUCR1	__reg16(OPSPUT_LAN_BASE + 0xc0010)
#define OPSPUT_LAN_ICUCR3	__reg16(OPSPUT_LAN_BASE + 0xc0014)

#endif /* _OPSPUT_OPSPUT_LAN_H */
