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
 * arch/arm/mach-at91/include/mach/at91_st.h
 *
 * Copyright (C) 2005 Ivan Kokshaysky
 * Copyright (C) SAN People
 *
 * System Timer (ST) - System peripherals registers.
 * Based on AT91RM9200 datasheet revision E.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef AT91_ST_H
#define AT91_ST_H

#define	AT91_ST_CR		(AT91_ST + 0x00)	/* Control Register */
#define 	AT91_ST_WDRST		(1 << 0)		/* Watchdog Timer Restart */

#define	AT91_ST_PIMR		(AT91_ST + 0x04)	/* Period Interval Mode Register */
#define		AT91_ST_PIV		(0xffff <<  0)		/* Period Interval Value */

#define	AT91_ST_WDMR		(AT91_ST + 0x08)	/* Watchdog Mode Register */
#define		AT91_ST_WDV		(0xffff <<  0)		/* Watchdog Counter Value */
#define		AT91_ST_RSTEN		(1	<< 16)		/* Reset Enable */
#define		AT91_ST_EXTEN		(1	<< 17)		/* External Signal Assertion Enable */

#define	AT91_ST_RTMR		(AT91_ST + 0x0c)	/* Real-time Mode Register */
#define		AT91_ST_RTPRES		(0xffff <<  0)		/* Real-time Prescalar Value */

#define	AT91_ST_SR		(AT91_ST + 0x10)	/* Status Register */
#define		AT91_ST_PITS		(1 << 0)		/* Period Interval Timer Status */
#define		AT91_ST_WDOVF		(1 << 1) 		/* Watchdog Overflow */
#define		AT91_ST_RTTINC		(1 << 2) 		/* Real-time Timer Increment */
#define		AT91_ST_ALMS		(1 << 3) 		/* Alarm Status */

#define	AT91_ST_IER		(AT91_ST + 0x14)	/* Interrupt Enable Register */
#define	AT91_ST_IDR		(AT91_ST + 0x18)	/* Interrupt Disable Register */
#define	AT91_ST_IMR		(AT91_ST + 0x1c)	/* Interrupt Mask Register */

#define	AT91_ST_RTAR		(AT91_ST + 0x20)	/* Real-time Alarm Register */
#define		AT91_ST_ALMV		(0xfffff << 0)		/* Alarm Value */

#define	AT91_ST_CRTR		(AT91_ST + 0x24)	/* Current Real-time Register */
#define		AT91_ST_CRTV		(0xfffff << 0)		/* Current Real-Time Value */

#endif
