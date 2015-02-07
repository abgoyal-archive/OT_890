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
 * arch/arm/mach-at91/include/mach/timex.h
 *
 *  Copyright (C) 2003 SAN People
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __ASM_ARCH_TIMEX_H
#define __ASM_ARCH_TIMEX_H

#include <mach/hardware.h>

#if defined(CONFIG_ARCH_AT91RM9200)

#define CLOCK_TICK_RATE		(AT91_SLOW_CLOCK)

#elif defined(CONFIG_ARCH_AT91SAM9260)

#if defined(CONFIG_MACH_USB_A9260) || defined(CONFIG_MACH_QIL_A9260)
#define AT91SAM9_MASTER_CLOCK	90000000
#else
#define AT91SAM9_MASTER_CLOCK	99300000
#endif

#define CLOCK_TICK_RATE		(AT91SAM9_MASTER_CLOCK/16)

#elif defined(CONFIG_ARCH_AT91SAM9261)

#define AT91SAM9_MASTER_CLOCK	99300000
#define CLOCK_TICK_RATE		(AT91SAM9_MASTER_CLOCK/16)

#elif defined(CONFIG_ARCH_AT91SAM9263)

#if defined(CONFIG_MACH_USB_A9263)
#define AT91SAM9_MASTER_CLOCK	90000000
#else
#define AT91SAM9_MASTER_CLOCK	99959500
#endif

#define CLOCK_TICK_RATE		(AT91SAM9_MASTER_CLOCK/16)

#elif defined(CONFIG_ARCH_AT91SAM9RL)

#define AT91SAM9_MASTER_CLOCK	100000000
#define CLOCK_TICK_RATE		(AT91SAM9_MASTER_CLOCK/16)

#elif defined(CONFIG_ARCH_AT91SAM9G20)

#define AT91SAM9_MASTER_CLOCK	132096000
#define CLOCK_TICK_RATE		(AT91SAM9_MASTER_CLOCK/16)

#elif defined(CONFIG_ARCH_AT91CAP9)

#define AT91CAP9_MASTER_CLOCK	100000000
#define CLOCK_TICK_RATE		(AT91CAP9_MASTER_CLOCK/16)

#elif defined(CONFIG_ARCH_AT91X40)

#define AT91X40_MASTER_CLOCK	40000000
#define CLOCK_TICK_RATE		(AT91X40_MASTER_CLOCK)

#endif

#endif
