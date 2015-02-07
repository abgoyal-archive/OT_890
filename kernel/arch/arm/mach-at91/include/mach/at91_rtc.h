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
 * arch/arm/mach-at91/include/mach/at91_rtc.h
 *
 * Copyright (C) 2005 Ivan Kokshaysky
 * Copyright (C) SAN People
 *
 * Real Time Clock (RTC) - System peripheral registers.
 * Based on AT91RM9200 datasheet revision E.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef AT91_RTC_H
#define AT91_RTC_H

#define	AT91_RTC_CR		(AT91_RTC + 0x00)	/* Control Register */
#define		AT91_RTC_UPDTIM		(1 <<  0)		/* Update Request Time Register */
#define		AT91_RTC_UPDCAL		(1 <<  1)		/* Update Request Calendar Register */
#define		AT91_RTC_TIMEVSEL	(3 <<  8)		/* Time Event Selection */
#define			AT91_RTC_TIMEVSEL_MINUTE	(0 << 8)
#define			AT91_RTC_TIMEVSEL_HOUR		(1 << 8)
#define			AT91_RTC_TIMEVSEL_DAY24		(2 << 8)
#define			AT91_RTC_TIMEVSEL_DAY12		(3 << 8)
#define		AT91_RTC_CALEVSEL	(3 << 16)		/* Calendar Event Selection */
#define			AT91_RTC_CALEVSEL_WEEK		(0 << 16)
#define			AT91_RTC_CALEVSEL_MONTH		(1 << 16)
#define			AT91_RTC_CALEVSEL_YEAR		(2 << 16)

#define	AT91_RTC_MR		(AT91_RTC + 0x04)	/* Mode Register */
#define			AT91_RTC_HRMOD		(1 <<  0)		/* 12/24 Hour Mode */

#define	AT91_RTC_TIMR		(AT91_RTC + 0x08)	/* Time Register */
#define		AT91_RTC_SEC		(0x7f <<  0)		/* Current Second */
#define		AT91_RTC_MIN		(0x7f <<  8)		/* Current Minute */
#define		AT91_RTC_HOUR		(0x3f << 16)		/* Current Hour */
#define		AT91_RTC_AMPM		(1    << 22)		/* Ante Meridiem Post Meridiem Indicator */

#define	AT91_RTC_CALR		(AT91_RTC + 0x0c)	/* Calendar Register */
#define		AT91_RTC_CENT		(0x7f <<  0)		/* Current Century */
#define		AT91_RTC_YEAR		(0xff <<  8)		/* Current Year */
#define		AT91_RTC_MONTH		(0x1f << 16)		/* Current Month */
#define		AT91_RTC_DAY		(7    << 21)		/* Current Day */
#define		AT91_RTC_DATE		(0x3f << 24)		/* Current Date */

#define	AT91_RTC_TIMALR		(AT91_RTC + 0x10)	/* Time Alarm Register */
#define		AT91_RTC_SECEN		(1 <<  7)		/* Second Alarm Enable */
#define		AT91_RTC_MINEN		(1 << 15)		/* Minute Alarm Enable */
#define		AT91_RTC_HOUREN		(1 << 23)		/* Hour Alarm Enable */

#define	AT91_RTC_CALALR		(AT91_RTC + 0x14)	/* Calendar Alarm Register */
#define		AT91_RTC_MTHEN		(1 << 23)		/* Month Alarm Enable */
#define		AT91_RTC_DATEEN		(1 << 31)		/* Date Alarm Enable */

#define	AT91_RTC_SR		(AT91_RTC + 0x18)	/* Status Register */
#define		AT91_RTC_ACKUPD		(1 <<  0)		/* Acknowledge for Update */
#define		AT91_RTC_ALARM		(1 <<  1)		/* Alarm Flag */
#define		AT91_RTC_SECEV		(1 <<  2)		/* Second Event */
#define		AT91_RTC_TIMEV		(1 <<  3)		/* Time Event */
#define		AT91_RTC_CALEV		(1 <<  4)		/* Calendar Event */

#define	AT91_RTC_SCCR		(AT91_RTC + 0x1c)	/* Status Clear Command Register */
#define	AT91_RTC_IER		(AT91_RTC + 0x20)	/* Interrupt Enable Register */
#define	AT91_RTC_IDR		(AT91_RTC + 0x24)	/* Interrupt Disable Register */
#define	AT91_RTC_IMR		(AT91_RTC + 0x28)	/* Interrupt Mask Register */

#define	AT91_RTC_VER		(AT91_RTC + 0x2c)	/* Valid Entry Register */
#define		AT91_RTC_NVTIM		(1 <<  0)		/* Non valid Time */
#define		AT91_RTC_NVCAL		(1 <<  1)		/* Non valid Calendar */
#define		AT91_RTC_NVTIMALR	(1 <<  2)		/* Non valid Time Alarm */
#define		AT91_RTC_NVCALALR	(1 <<  3)		/* Non valid Calendar Alarm */

#endif
