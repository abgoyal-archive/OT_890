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
 *  Copyright (C) 1991, 1992, 1995  Linus Torvalds
 *
 * Adapted for PowerPC (PReP) by Gary Thomas
 * Modified by Cort Dougan (cort@cs.nmt.edu).
 * Copied and modified from arch/i386/kernel/time.c
 *
 */
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/param.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/timex.h>
#include <linux/kernel_stat.h>
#include <linux/mc146818rtc.h>
#include <linux/init.h>
#include <linux/bcd.h>
#include <linux/ioport.h>

#include <asm/io.h>
#include <asm/nvram.h>
#include <asm/prom.h>
#include <asm/sections.h>
#include <asm/time.h>

extern spinlock_t rtc_lock;

static int nvram_as1 = NVRAM_AS1;
static int nvram_as0 = NVRAM_AS0;
static int nvram_data = NVRAM_DATA;

long __init chrp_time_init(void)
{
	struct device_node *rtcs;
	struct resource r;
	int base;

	rtcs = of_find_compatible_node(NULL, "rtc", "pnpPNP,b00");
	if (rtcs == NULL)
		rtcs = of_find_compatible_node(NULL, "rtc", "ds1385-rtc");
	if (rtcs == NULL)
		return 0;
	if (of_address_to_resource(rtcs, 0, &r)) {
		of_node_put(rtcs);
		return 0;
	}
	of_node_put(rtcs);

	base = r.start;
	nvram_as1 = 0;
	nvram_as0 = base;
	nvram_data = base + 1;

	return 0;
}

int chrp_cmos_clock_read(int addr)
{
	if (nvram_as1 != 0)
		outb(addr>>8, nvram_as1);
	outb(addr, nvram_as0);
	return (inb(nvram_data));
}

void chrp_cmos_clock_write(unsigned long val, int addr)
{
	if (nvram_as1 != 0)
		outb(addr>>8, nvram_as1);
	outb(addr, nvram_as0);
	outb(val, nvram_data);
	return;
}

/*
 * Set the hardware clock. -- Cort
 */
int chrp_set_rtc_time(struct rtc_time *tmarg)
{
	unsigned char save_control, save_freq_select;
	struct rtc_time tm = *tmarg;

	spin_lock(&rtc_lock);

	save_control = chrp_cmos_clock_read(RTC_CONTROL); /* tell the clock it's being set */

	chrp_cmos_clock_write((save_control|RTC_SET), RTC_CONTROL);

	save_freq_select = chrp_cmos_clock_read(RTC_FREQ_SELECT); /* stop and reset prescaler */

	chrp_cmos_clock_write((save_freq_select|RTC_DIV_RESET2), RTC_FREQ_SELECT);

	if (!(save_control & RTC_DM_BINARY) || RTC_ALWAYS_BCD) {
		tm.tm_sec = bin2bcd(tm.tm_sec);
		tm.tm_min = bin2bcd(tm.tm_min);
		tm.tm_hour = bin2bcd(tm.tm_hour);
		tm.tm_mon = bin2bcd(tm.tm_mon);
		tm.tm_mday = bin2bcd(tm.tm_mday);
		tm.tm_year = bin2bcd(tm.tm_year);
	}
	chrp_cmos_clock_write(tm.tm_sec,RTC_SECONDS);
	chrp_cmos_clock_write(tm.tm_min,RTC_MINUTES);
	chrp_cmos_clock_write(tm.tm_hour,RTC_HOURS);
	chrp_cmos_clock_write(tm.tm_mon,RTC_MONTH);
	chrp_cmos_clock_write(tm.tm_mday,RTC_DAY_OF_MONTH);
	chrp_cmos_clock_write(tm.tm_year,RTC_YEAR);

	/* The following flags have to be released exactly in this order,
	 * otherwise the DS12887 (popular MC146818A clone with integrated
	 * battery and quartz) will not reset the oscillator and will not
	 * update precisely 500 ms later. You won't find this mentioned in
	 * the Dallas Semiconductor data sheets, but who believes data
	 * sheets anyway ...                           -- Markus Kuhn
	 */
	chrp_cmos_clock_write(save_control, RTC_CONTROL);
	chrp_cmos_clock_write(save_freq_select, RTC_FREQ_SELECT);

	spin_unlock(&rtc_lock);
	return 0;
}

void chrp_get_rtc_time(struct rtc_time *tm)
{
	unsigned int year, mon, day, hour, min, sec;

	do {
		sec = chrp_cmos_clock_read(RTC_SECONDS);
		min = chrp_cmos_clock_read(RTC_MINUTES);
		hour = chrp_cmos_clock_read(RTC_HOURS);
		day = chrp_cmos_clock_read(RTC_DAY_OF_MONTH);
		mon = chrp_cmos_clock_read(RTC_MONTH);
		year = chrp_cmos_clock_read(RTC_YEAR);
	} while (sec != chrp_cmos_clock_read(RTC_SECONDS));

	if (!(chrp_cmos_clock_read(RTC_CONTROL) & RTC_DM_BINARY) || RTC_ALWAYS_BCD) {
		sec = bcd2bin(sec);
		min = bcd2bin(min);
		hour = bcd2bin(hour);
		day = bcd2bin(day);
		mon = bcd2bin(mon);
		year = bcd2bin(year);
	}
	if (year < 70)
		year += 100;
	tm->tm_sec = sec;
	tm->tm_min = min;
	tm->tm_hour = hour;
	tm->tm_mday = day;
	tm->tm_mon = mon;
	tm->tm_year = year;
}
