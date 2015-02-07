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
 *  PS3 time and rtc routines.
 *
 *  Copyright (C) 2006 Sony Computer Entertainment Inc.
 *  Copyright 2006 Sony Corp.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/kernel.h>

#include <asm/rtc.h>
#include <asm/lv1call.h>
#include <asm/ps3.h>

#include "platform.h"

#define dump_tm(_a) _dump_tm(_a, __func__, __LINE__)
static void _dump_tm(const struct rtc_time *tm, const char* func, int line)
{
	pr_debug("%s:%d tm_sec  %d\n", func, line, tm->tm_sec);
	pr_debug("%s:%d tm_min  %d\n", func, line, tm->tm_min);
	pr_debug("%s:%d tm_hour %d\n", func, line, tm->tm_hour);
	pr_debug("%s:%d tm_mday %d\n", func, line, tm->tm_mday);
	pr_debug("%s:%d tm_mon  %d\n", func, line, tm->tm_mon);
	pr_debug("%s:%d tm_year %d\n", func, line, tm->tm_year);
	pr_debug("%s:%d tm_wday %d\n", func, line, tm->tm_wday);
}

#define dump_time(_a) _dump_time(_a, __func__, __LINE__)
static void __maybe_unused _dump_time(int time, const char *func,
	int line)
{
	struct rtc_time tm;

	to_tm(time, &tm);

	pr_debug("%s:%d time    %d\n", func, line, time);
	_dump_tm(&tm, func, line);
}

void __init ps3_calibrate_decr(void)
{
	int result;
	u64 tmp;

	result = ps3_repository_read_be_tb_freq(0, &tmp);
	BUG_ON(result);

	ppc_tb_freq = tmp;
	ppc_proc_freq = ppc_tb_freq * 40;
}

static u64 read_rtc(void)
{
	int result;
	u64 rtc_val;
	u64 tb_val;

	result = lv1_get_rtc(&rtc_val, &tb_val);
	BUG_ON(result);

	return rtc_val;
}

int ps3_set_rtc_time(struct rtc_time *tm)
{
	u64 now = mktime(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
		tm->tm_hour, tm->tm_min, tm->tm_sec);

	ps3_os_area_set_rtc_diff(now - read_rtc());
	return 0;
}

void ps3_get_rtc_time(struct rtc_time *tm)
{
	to_tm(read_rtc() + ps3_os_area_get_rtc_diff(), tm);
	tm->tm_year -= 1900;
	tm->tm_mon -= 1;
}

unsigned long __init ps3_get_boot_time(void)
{
	return read_rtc() + ps3_os_area_get_rtc_diff();
}
