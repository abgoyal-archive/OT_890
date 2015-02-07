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

#ifndef __RTC_H__
#define __RTC_H__

   /* Dallas DS1302 clock/calendar register numbers. */
#  define RTC_SECONDS      0
#  define RTC_MINUTES      1
#  define RTC_HOURS        2
#  define RTC_DAY_OF_MONTH 3
#  define RTC_MONTH        4
#  define RTC_WEEKDAY      5
#  define RTC_YEAR         6
#  define RTC_CONTROL      7

   /* Bits in CONTROL register. */
#  define RTC_CONTROL_WRITEPROTECT 	0x80
#  define RTC_TRICKLECHARGER 		8

  /* Bits in TRICKLECHARGER register TCS TCS TCS TCS DS DS RS RS. */
#  define RTC_TCR_PATTERN 	0xA0	/* 1010xxxx */
#  define RTC_TCR_1DIOD 	0x04	/* xxxx01xx */
#  define RTC_TCR_2DIOD 	0x08	/* xxxx10xx */
#  define RTC_TCR_DISABLED 	0x00	/* xxxxxx00 Disabled */
#  define RTC_TCR_2KOHM 	0x01	/* xxxxxx01 2KOhm */
#  define RTC_TCR_4KOHM 	0x02	/* xxxxxx10 4kOhm */
#  define RTC_TCR_8KOHM 	0x03	/* xxxxxx11 8kOhm */

#ifdef CONFIG_DS1302
extern unsigned char ds1302_readreg(int reg);
extern void ds1302_writereg(int reg, unsigned char val);
extern int ds1302_init(void);
#  define CMOS_READ(x) ds1302_readreg(x)
#  define CMOS_WRITE(val,reg) ds1302_writereg(reg,val)
#  define RTC_INIT() ds1302_init()
#else
  /* No RTC configured so we shouldn't try to access any. */
#  define CMOS_READ(x) 42
#  define CMOS_WRITE(x,y)
#  define RTC_INIT() (-1)
#endif

/*
 * The struct used to pass data via the following ioctl. Similar to the
 * struct tm in <time.h>, but it needs to be here so that the kernel
 * source is self contained, allowing cross-compiles, etc. etc.
 */
struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

/* ioctl() calls that are permitted to the /dev/rtc interface. */
#define RTC_MAGIC 'p'
#define RTC_RD_TIME		_IOR(RTC_MAGIC, 0x09, struct rtc_time)	/* Read RTC time. */
#define RTC_SET_TIME		_IOW(RTC_MAGIC, 0x0a, struct rtc_time)	/* Set RTC time. */
#define RTC_SET_CHARGE  	_IOW(RTC_MAGIC, 0x0b, int)
#define RTC_MAX_IOCTL 0x0b

#endif /* __RTC_H__ */
