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

/* MN10300 RTC management
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mc146818rtc.h>
#include <linux/bcd.h>
#include <linux/timex.h>
#include <asm/rtc-regs.h>
#include <asm/rtc.h>

DEFINE_SPINLOCK(rtc_lock);
EXPORT_SYMBOL(rtc_lock);

/* last time the RTC got updated */
static long last_rtc_update;

/* time for RTC to update itself in ioclks */
static unsigned long mn10300_rtc_update_period;

/*
 * read the current RTC time
 */
unsigned long __init get_initial_rtc_time(void)
{
	struct rtc_time tm;

	get_rtc_time(&tm);

	return mktime(tm.tm_year, tm.tm_mon, tm.tm_mday,
		      tm.tm_hour, tm.tm_min, tm.tm_sec);
}

/*
 * In order to set the CMOS clock precisely, set_rtc_mmss has to be called 500
 * ms after the second nowtime has started, because when nowtime is written
 * into the registers of the CMOS clock, it will jump to the next second
 * precisely 500 ms later.  Check the Motorola MC146818A or Dallas DS12887 data
 * sheet for details.
 *
 * BUG: This routine does not handle hour overflow properly; it just
 *      sets the minutes. Usually you'll only notice that after reboot!
 */
static int set_rtc_mmss(unsigned long nowtime)
{
	unsigned char save_control, save_freq_select;
	int retval = 0;
	int real_seconds, real_minutes, cmos_minutes;

	/* gets recalled with irq locally disabled */
	spin_lock(&rtc_lock);
	save_control = CMOS_READ(RTC_CONTROL); /* tell the clock it's being
						* set */
	CMOS_WRITE(save_control | RTC_SET, RTC_CONTROL);

	save_freq_select = CMOS_READ(RTC_FREQ_SELECT); /* stop and reset
							* prescaler */
	CMOS_WRITE(save_freq_select | RTC_DIV_RESET2, RTC_FREQ_SELECT);

	cmos_minutes = CMOS_READ(RTC_MINUTES);
	if (!(save_control & RTC_DM_BINARY) || RTC_ALWAYS_BCD)
		cmos_minutes = bcd2bin(cmos_minutes);

	/*
	 * since we're only adjusting minutes and seconds,
	 * don't interfere with hour overflow. This avoids
	 * messing with unknown time zones but requires your
	 * RTC not to be off by more than 15 minutes
	 */
	real_seconds = nowtime % 60;
	real_minutes = nowtime / 60;
	if (((abs(real_minutes - cmos_minutes) + 15) / 30) & 1)
		/* correct for half hour time zone */
		real_minutes += 30;
	real_minutes %= 60;

	if (abs(real_minutes - cmos_minutes) < 30) {
		if (!(save_control & RTC_DM_BINARY) || RTC_ALWAYS_BCD) {
			real_seconds = bin2bcd(real_seconds);
			real_minutes = bin2bcd(real_minutes);
		}
		CMOS_WRITE(real_seconds, RTC_SECONDS);
		CMOS_WRITE(real_minutes, RTC_MINUTES);
	} else {
		printk(KERN_WARNING
		       "set_rtc_mmss: can't update from %d to %d\n",
		       cmos_minutes, real_minutes);
		retval = -1;
	}

	/* The following flags have to be released exactly in this order,
	 * otherwise the DS12887 (popular MC146818A clone with integrated
	 * battery and quartz) will not reset the oscillator and will not
	 * update precisely 500 ms later. You won't find this mentioned in
	 * the Dallas Semiconductor data sheets, but who believes data
	 * sheets anyway ...                           -- Markus Kuhn
	 */
	CMOS_WRITE(save_control, RTC_CONTROL);
	CMOS_WRITE(save_freq_select, RTC_FREQ_SELECT);
	spin_unlock(&rtc_lock);

	return retval;
}

void check_rtc_time(void)
{
	/* the RTC clock just finished ticking over again this second
	 * - if we have an externally synchronized Linux clock, then update
	 *   RTC clock accordingly every ~11 minutes. set_rtc_mmss() has to be
	 *   called as close as possible to 500 ms before the new second starts.
	 */
	if ((time_status & STA_UNSYNC) == 0 &&
	    xtime.tv_sec > last_rtc_update + 660 &&
	    xtime.tv_nsec / 1000 >= 500000 - ((unsigned) TICK_SIZE) / 2 &&
	    xtime.tv_nsec / 1000 <= 500000 + ((unsigned) TICK_SIZE) / 2
	    ) {
		if (set_rtc_mmss(xtime.tv_sec) == 0)
			last_rtc_update = xtime.tv_sec;
		else
			/* do it again in 60s */
			last_rtc_update = xtime.tv_sec - 600;
	}
}

/*
 * calibrate the TSC clock against the RTC
 */
void __init calibrate_clock(void)
{
	unsigned long count0, counth, count1;
	unsigned char status;

	/* make sure the RTC is running and is set to operate in 24hr mode */
	status = RTSRC;
	RTCRB |= RTCRB_SET;
	RTCRB |= RTCRB_TM_24HR;
	RTCRA |= RTCRA_DVR;
	RTCRA &= ~RTCRA_DVR;
	RTCRB &= ~RTCRB_SET;

	/* work out the clock speed by counting clock cycles between ends of
	 * the RTC update cycle - track the RTC through one complete update
	 * cycle (1 second)
	 */
	startup_timestamp_counter();

	while (!(RTCRA & RTCRA_UIP)) {}
	while ((RTCRA & RTCRA_UIP)) {}

	count0 = TMTSCBC;

	while (!(RTCRA & RTCRA_UIP)) {}

	counth = TMTSCBC;

	while ((RTCRA & RTCRA_UIP)) {}

	count1 = TMTSCBC;

	shutdown_timestamp_counter();

	MN10300_TSCCLK = count0 - count1; /* the timers count down */
	mn10300_rtc_update_period = counth - count1;
	MN10300_TSC_PER_HZ = MN10300_TSCCLK / HZ;
}
