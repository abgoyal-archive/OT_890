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
 * MTD primitives for XIP support. Architecture specific functions
 *
 * Do not include this file directly. It's included from linux/mtd/xip.h
 *
 * Copyright (C) 2008 Darius Augulis <augulis.darius@gmail.com>, Teltonika, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <mach/mxc_timer.h>

#ifndef __ARCH_IMX_MTD_XIP_H__
#define __ARCH_IMX_MTD_XIP_H__

#ifdef CONFIG_ARCH_MX1
/* AITC registers */
#define AITC_BASE	IO_ADDRESS(AVIC_BASE_ADDR)
#define NIPNDH		(AITC_BASE + 0x58)
#define NIPNDL		(AITC_BASE + 0x5C)
#define INTENABLEH	(AITC_BASE + 0x10)
#define INTENABLEL	(AITC_BASE + 0x14)
/* MTD macros */
#define xip_irqpending() ((__raw_readl(INTENABLEH) &  __raw_readl(NIPNDH)) \
			|| (__raw_readl(INTENABLEL) &  __raw_readl(NIPNDL)))
#define xip_currtime()		(__raw_readl(TIMER_BASE + MXC_TCN))
#define xip_elapsed_since(x)	(signed)((__raw_readl(TIMER_BASE + MXC_TCN) - (x)) / 96)
#define xip_cpu_idle()		asm volatile ("mcr p15, 0, %0, c7, c0, 4" :: "r" (0))
#endif /* CONFIG_ARCH_MX1 */

#endif /* __ARCH_IMX_MTD_XIP_H__ */
