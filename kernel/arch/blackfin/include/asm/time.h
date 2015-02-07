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
 * asm-blackfin/time.h:
 *
 * Copyright 2004-2008 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef _ASM_BLACKFIN_TIME_H
#define _ASM_BLACKFIN_TIME_H

/*
 * The way that the Blackfin core timer works is:
 *  - CCLK is divided by a programmable 8-bit pre-scaler (TSCALE)
 *  - Every time TSCALE ticks, a 32bit is counted down (TCOUNT)
 *
 * If you take the fastest clock (1ns, or 1GHz to make the math work easier)
 *    10ms is 10,000,000 clock ticks, which fits easy into a 32-bit counter
 *    (32 bit counter is 4,294,967,296ns or 4.2 seconds) so, we don't need
 *    to use TSCALE, and program it to zero (which is pass CCLK through).
 *    If you feel like using it, try to keep HZ * TIMESCALE to some
 *    value that divides easy (like power of 2).
 */

#ifndef CONFIG_CPU_FREQ
#define TIME_SCALE 1
#define __bfin_cycles_off (0)
#define __bfin_cycles_mod (0)
#else
/*
 * Blackfin CPU frequency scaling supports max Core Clock 1, 1/2 and 1/4 .
 * Whenever we change the Core Clock frequency changes we immediately
 * adjust the Core Timer Presale Register. This way we don't lose time.
 */
#define TIME_SCALE 4
extern unsigned long long __bfin_cycles_off;
extern unsigned int __bfin_cycles_mod;
#endif

#endif
