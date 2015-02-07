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

#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/rtc.h>
#include <linux/delay.h>
#include <asm/prom.h>
#include <asm/rtas.h>
#include <asm/time.h>


#define MAX_RTC_WAIT 5000	/* 5 sec */
#define RTAS_CLOCK_BUSY (-2)
unsigned long __init rtas_get_boot_time(void)
{
	int ret[8];
	int error;
	unsigned int wait_time;
	u64 max_wait_tb;

	max_wait_tb = get_tb() + tb_ticks_per_usec * 1000 * MAX_RTC_WAIT;
	do {
		error = rtas_call(rtas_token("get-time-of-day"), 0, 8, ret);

		wait_time = rtas_busy_delay_time(error);
		if (wait_time) {
			/* This is boot time so we spin. */
			udelay(wait_time*1000);
		}
	} while (wait_time && (get_tb() < max_wait_tb));

	if (error != 0 && printk_ratelimit()) {
		printk(KERN_WARNING "error: reading the clock failed (%d)\n",
			error);
		return 0;
	}

	return mktime(ret[0], ret[1], ret[2], ret[3], ret[4], ret[5]);
}

/* NOTE: get_rtc_time will get an error if executed in interrupt context
 * and if a delay is needed to read the clock.  In this case we just
 * silently return without updating rtc_tm.
 */
void rtas_get_rtc_time(struct rtc_time *rtc_tm)
{
        int ret[8];
	int error;
	unsigned int wait_time;
	u64 max_wait_tb;

	max_wait_tb = get_tb() + tb_ticks_per_usec * 1000 * MAX_RTC_WAIT;
	do {
		error = rtas_call(rtas_token("get-time-of-day"), 0, 8, ret);

		wait_time = rtas_busy_delay_time(error);
		if (wait_time) {
			if (in_interrupt() && printk_ratelimit()) {
				memset(rtc_tm, 0, sizeof(struct rtc_time));
				printk(KERN_WARNING "error: reading clock"
				       " would delay interrupt\n");
				return;	/* delay not allowed */
			}
			msleep(wait_time);
		}
	} while (wait_time && (get_tb() < max_wait_tb));

        if (error != 0 && printk_ratelimit()) {
                printk(KERN_WARNING "error: reading the clock failed (%d)\n",
		       error);
		return;
        }

	rtc_tm->tm_sec = ret[5];
	rtc_tm->tm_min = ret[4];
	rtc_tm->tm_hour = ret[3];
	rtc_tm->tm_mday = ret[2];
	rtc_tm->tm_mon = ret[1] - 1;
	rtc_tm->tm_year = ret[0] - 1900;
}

int rtas_set_rtc_time(struct rtc_time *tm)
{
	int error, wait_time;
	u64 max_wait_tb;

	max_wait_tb = get_tb() + tb_ticks_per_usec * 1000 * MAX_RTC_WAIT;
	do {
	        error = rtas_call(rtas_token("set-time-of-day"), 7, 1, NULL,
				  tm->tm_year + 1900, tm->tm_mon + 1,
				  tm->tm_mday, tm->tm_hour, tm->tm_min,
				  tm->tm_sec, 0);

		wait_time = rtas_busy_delay_time(error);
		if (wait_time) {
			if (in_interrupt())
				return 1;	/* probably decrementer */
			msleep(wait_time);
		}
	} while (wait_time && (get_tb() < max_wait_tb));

        if (error != 0 && printk_ratelimit())
                printk(KERN_WARNING "error: setting the clock failed (%d)\n",
		       error);

        return 0;
}
