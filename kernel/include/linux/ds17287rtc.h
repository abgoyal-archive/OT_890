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
 * ds17287rtc.h - register definitions for the ds1728[57] RTC / CMOS RAM
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * (C) 2003 Guido Guenther <agx@sigxcpu.org>
 */
#ifndef __LINUX_DS17287RTC_H
#define __LINUX_DS17287RTC_H

#include <linux/rtc.h>			/* get the user-level API */
#include <linux/mc146818rtc.h>

/* Register A */
#define DS_REGA_DV2	0x40		/* countdown chain */
#define DS_REGA_DV1	0x20		/* oscillator enable */
#define DS_REGA_DV0	0x10		/* bank select */

/* bank 1 registers */
#define DS_B1_MODEL	0x40		/* model number byte */
#define DS_B1_SN1 	0x41		/* serial number byte 1 */
#define DS_B1_SN2 	0x42		/* serial number byte 2 */
#define DS_B1_SN3 	0x43		/* serial number byte 3 */
#define DS_B1_SN4 	0x44		/* serial number byte 4 */
#define DS_B1_SN5 	0x45		/* serial number byte 5 */
#define DS_B1_SN6 	0x46		/* serial number byte 6 */
#define DS_B1_CRC 	0x47		/* CRC byte */
#define DS_B1_CENTURY 	0x48		/* Century byte */
#define DS_B1_DALARM 	0x49		/* date alarm */
#define DS_B1_XCTRL4A	0x4a		/* extendec control register 4a */
#define DS_B1_XCTRL4B	0x4b		/* extendec control register 4b */
#define DS_B1_RTCADDR2 	0x4e		/* rtc address 2 */
#define DS_B1_RTCADDR3 	0x4f		/* rtc address 3 */
#define DS_B1_RAMLSB	0x50		/* extended ram LSB */
#define DS_B1_RAMMSB	0x51		/* extended ram MSB */
#define DS_B1_RAMDPORT	0x53		/* extended ram data port */

/* register details */
/* extended control register 4a */
#define DS_XCTRL4A_VRT2	0x80 		/* valid ram and time */
#define DS_XCTRL4A_INCR	0x40		/* increment progress status */
#define DS_XCTRL4A_BME	0x20		/* burst mode enable */
#define DS_XCTRL4A_PAB	0x08		/* power active bar ctrl */
#define DS_XCTRL4A_RF	0x04		/* ram clear flag */
#define DS_XCTRL4A_WF	0x02		/* wake up alarm flag */
#define DS_XCTRL4A_KF	0x01		/* kickstart flag */

/* interrupt causes */
#define DS_XCTRL4A_IFS	(DS_XCTRL4A_RF|DS_XCTRL4A_WF|DS_XCTRL4A_KF)

/* extended control register 4b */
#define DS_XCTRL4B_ABE	0x80 		/* auxiliary battery enable */
#define DS_XCTRL4B_E32K	0x40		/* enable 32.768 kHz Output */
#define DS_XCTRL4B_CS	0x20		/* crystal select */
#define DS_XCTRL4B_RCE	0x10		/* ram clear enable */
#define DS_XCTRL4B_PRS	0x08		/* PAB resec select */
#define DS_XCTRL4B_RIE	0x04		/* ram clear interrupt enable */
#define DS_XCTRL4B_WFE	0x02		/* wake up alarm interrupt enable */
#define DS_XCTRL4B_KFE	0x01		/* kickstart interrupt enable */

/* interrupt enable bits */
#define DS_XCTRL4B_IFES	(DS_XCTRL4B_RIE|DS_XCTRL4B_WFE|DS_XCTRL4B_KFE)

#endif /* __LINUX_DS17287RTC_H */
