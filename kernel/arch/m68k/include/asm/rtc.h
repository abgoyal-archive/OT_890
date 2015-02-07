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

/* include/asm-m68k/rtc.h
 *
 * Copyright Richard Zidlicky
 * implementation details for genrtc/q40rtc driver
 */
/* permission is hereby granted to copy, modify and redistribute this code
 * in terms of the GNU Library General Public License, Version 2 or later,
 * at your option.
 */

#ifndef _ASM_RTC_H
#define _ASM_RTC_H

#ifdef __KERNEL__

#include <linux/rtc.h>
#include <asm/errno.h>
#include <asm/machdep.h>

#define RTC_PIE 0x40		/* periodic interrupt enable */
#define RTC_AIE 0x20		/* alarm interrupt enable */
#define RTC_UIE 0x10		/* update-finished interrupt enable */

/* some dummy definitions */
#define RTC_BATT_BAD 0x100	/* battery bad */
#define RTC_SQWE 0x08		/* enable square-wave output */
#define RTC_DM_BINARY 0x04	/* all time/date values are BCD if clear */
#define RTC_24H 0x02		/* 24 hour mode - else hours bit 7 means pm */
#define RTC_DST_EN 0x01	        /* auto switch DST - works f. USA only */

static inline unsigned int get_rtc_time(struct rtc_time *time)
{
	/*
	 * Only the values that we read from the RTC are set. We leave
	 * tm_wday, tm_yday and tm_isdst untouched. Even though the
	 * RTC has RTC_DAY_OF_WEEK, we ignore it, as it is only updated
	 * by the RTC when initially set to a non-zero value.
	 */
	mach_hwclk(0, time);
	return RTC_24H;
}

static inline int set_rtc_time(struct rtc_time *time)
{
	return mach_hwclk(1, time);
}

static inline unsigned int get_rtc_ss(void)
{
	if (mach_get_ss)
		return mach_get_ss();
	else{
		struct rtc_time h;

		get_rtc_time(&h);
		return h.tm_sec;
	}
}

static inline int get_rtc_pll(struct rtc_pll_info *pll)
{
	if (mach_get_rtc_pll)
		return mach_get_rtc_pll(pll);
	else
		return -EINVAL;
}
static inline int set_rtc_pll(struct rtc_pll_info *pll)
{
	if (mach_set_rtc_pll)
		return mach_set_rtc_pll(pll);
	else
		return -EINVAL;
}
#endif /* __KERNEL__ */

#endif /* _ASM__RTC_H */
