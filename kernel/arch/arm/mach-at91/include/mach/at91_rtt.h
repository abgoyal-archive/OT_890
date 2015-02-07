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
 * arch/arm/mach-at91/include/mach/at91_rtt.h
 *
 * Copyright (C) 2007 Andrew Victor
 * Copyright (C) 2007 Atmel Corporation.
 *
 * Real-time Timer (RTT) - System peripherals regsters.
 * Based on AT91SAM9261 datasheet revision D.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef AT91_RTT_H
#define AT91_RTT_H

#define AT91_RTT_MR		0x00			/* Real-time Mode Register */
#define		AT91_RTT_RTPRES		(0xffff << 0)		/* Real-time Timer Prescaler Value */
#define		AT91_RTT_ALMIEN		(1 << 16)		/* Alarm Interrupt Enable */
#define		AT91_RTT_RTTINCIEN	(1 << 17)		/* Real Time Timer Increment Interrupt Enable */
#define		AT91_RTT_RTTRST		(1 << 18)		/* Real Time Timer Restart */

#define AT91_RTT_AR		0x04			/* Real-time Alarm Register */
#define		AT91_RTT_ALMV		(0xffffffff)		/* Alarm Value */

#define AT91_RTT_VR		0x08			/* Real-time Value Register */
#define		AT91_RTT_CRTV		(0xffffffff)		/* Current Real-time Value */

#define AT91_RTT_SR		0x0c			/* Real-time Status Register */
#define		AT91_RTT_ALMS		(1 << 0)		/* Real-time Alarm Status */
#define		AT91_RTT_RTTINC		(1 << 1)		/* Real-time Timer Increment */

#endif
