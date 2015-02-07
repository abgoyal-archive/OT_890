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
 *  Copyright (C) 2004 by Basler Vision Technologies AG
 *  Author: Thomas Koeller <thomas.koeller@baslerweb.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#if !defined(_ASM_RM9K_OCD_H)
#define _ASM_RM9K_OCD_H

#include <linux/types.h>
#include <linux/spinlock.h>
#include <asm/io.h>

extern volatile void __iomem * const ocd_base;
extern volatile void __iomem * const titan_base;

#define ocd_addr(__x__)		(ocd_base + (__x__))
#define titan_addr(__x__)	(titan_base + (__x__))
#define scram_addr(__x__)	(scram_base + (__x__))

/* OCD register access */
#define ocd_readl(__offs__) __raw_readl(ocd_addr(__offs__))
#define ocd_readw(__offs__) __raw_readw(ocd_addr(__offs__))
#define ocd_readb(__offs__) __raw_readb(ocd_addr(__offs__))
#define ocd_writel(__val__, __offs__) \
	__raw_writel((__val__), ocd_addr(__offs__))
#define ocd_writew(__val__, __offs__) \
	__raw_writew((__val__), ocd_addr(__offs__))
#define ocd_writeb(__val__, __offs__) \
	__raw_writeb((__val__), ocd_addr(__offs__))

/* TITAN register access - 32 bit-wide only */
#define titan_readl(__offs__) __raw_readl(titan_addr(__offs__))
#define titan_writel(__val__, __offs__) \
	__raw_writel((__val__), titan_addr(__offs__))

/* Protect access to shared TITAN registers */
extern spinlock_t titan_lock;
extern int titan_irqflags;
#define lock_titan_regs() spin_lock_irqsave(&titan_lock, titan_irqflags)
#define unlock_titan_regs() spin_unlock_irqrestore(&titan_lock, titan_irqflags)

#endif	/* !defined(_ASM_RM9K_OCD_H) */
