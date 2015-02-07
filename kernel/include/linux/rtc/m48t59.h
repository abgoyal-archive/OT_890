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
 * include/linux/rtc/m48t59.h
 *
 * Definitions for the platform data of m48t59 RTC chip driver.
 *
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * Mark Zhan <rongkai.zhan@windriver.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _LINUX_RTC_M48T59_H_
#define _LINUX_RTC_M48T59_H_

/*
 * M48T59 Register Offset
 */
#define M48T59_YEAR		0xf
#define M48T59_MONTH		0xe
#define M48T59_MDAY		0xd	/* Day of Month */
#define M48T59_WDAY		0xc	/* Day of Week */
#define M48T59_WDAY_CB			0x20	/* Century Bit */
#define M48T59_WDAY_CEB			0x10	/* Century Enable Bit */
#define M48T59_HOUR		0xb
#define M48T59_MIN		0xa
#define M48T59_SEC		0x9
#define M48T59_CNTL		0x8
#define M48T59_CNTL_READ		0x40
#define M48T59_CNTL_WRITE		0x80
#define M48T59_WATCHDOG		0x7
#define M48T59_INTR		0x6
#define M48T59_INTR_AFE			0x80	/* Alarm Interrupt Enable */
#define M48T59_INTR_ABE			0x20
#define M48T59_ALARM_DATE	0x5
#define M48T59_ALARM_HOUR	0x4
#define M48T59_ALARM_MIN	0x3
#define M48T59_ALARM_SEC	0x2
#define M48T59_UNUSED		0x1
#define M48T59_FLAGS		0x0
#define M48T59_FLAGS_WDT		0x80	/* watchdog timer expired */
#define M48T59_FLAGS_AF			0x40	/* alarm */
#define M48T59_FLAGS_BF			0x10	/* low battery */

#define M48T59RTC_TYPE_M48T59	0 /* to keep compatibility */
#define M48T59RTC_TYPE_M48T02	1
#define M48T59RTC_TYPE_M48T08	2

struct m48t59_plat_data {
	/* The method to access M48T59 registers */
	void (*write_byte)(struct device *dev, u32 ofs, u8 val);
	unsigned char (*read_byte)(struct device *dev, u32 ofs);

	int type; /* RTC model */

	/* ioaddr mapped externally */
	void __iomem *ioaddr;
	/* offset to RTC registers, automatically set according to the type */
	unsigned int offset;
};

#endif /* _LINUX_RTC_M48T59_H_ */
