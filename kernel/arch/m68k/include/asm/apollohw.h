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

/* apollohw.h : some structures to access apollo HW */

#ifndef _ASMm68k_APOLLOHW_H_
#define _ASMm68k_APOLLOHW_H_

#include <linux/types.h>

/*
   apollo models
*/

extern u_long apollo_model;

#define APOLLO_UNKNOWN (0)
#define APOLLO_DN3000 (1)
#define APOLLO_DN3010 (2)
#define APOLLO_DN3500 (3)
#define APOLLO_DN4000 (4)
#define APOLLO_DN4500 (5)

/*
   see scn2681 data sheet for more info.
   member names are read_write.
*/

#define DECLARE_2681_FIELD(x) unsigned char x; unsigned char dummy##x

struct SCN2681 {

	DECLARE_2681_FIELD(mra);
	DECLARE_2681_FIELD(sra_csra);
	DECLARE_2681_FIELD(BRGtest_cra);
	DECLARE_2681_FIELD(rhra_thra);
	DECLARE_2681_FIELD(ipcr_acr);
	DECLARE_2681_FIELD(isr_imr);
	DECLARE_2681_FIELD(ctu_ctur);
	DECLARE_2681_FIELD(ctl_ctlr);
	DECLARE_2681_FIELD(mrb);
	DECLARE_2681_FIELD(srb_csrb);
	DECLARE_2681_FIELD(tst_crb);
	DECLARE_2681_FIELD(rhrb_thrb);
	DECLARE_2681_FIELD(reserved);
	DECLARE_2681_FIELD(ip_opcr);
	DECLARE_2681_FIELD(startCnt_setOutBit);
	DECLARE_2681_FIELD(stopCnt_resetOutBit);

};

#if 0
struct mc146818 {

	unsigned int second1:4, second2:4, alarm_second1:4, alarm_second2:4,
		     minute1:4, minute2:4, alarm_minute1:4, alarm_minute2:4;
	unsigned int hours1:4, hours2:4, alarm_hours1:4, alarm_hours2:4,
		     day_of_week1:4, day_of_week2:4, day_of_month1:4, day_of_month2:4;
	unsigned int month1:4, month2:4, year1:4, year2:4, :16;

};
#endif

struct mc146818 {
        unsigned char second, alarm_second;
        unsigned char minute, alarm_minute;
        unsigned char hours, alarm_hours;
        unsigned char day_of_week, day_of_month;
        unsigned char month, year;
};


#define IO_BASE 0x80000000

extern u_long sio01_physaddr;
extern u_long sio23_physaddr;
extern u_long rtc_physaddr;
extern u_long pica_physaddr;
extern u_long picb_physaddr;
extern u_long cpuctrl_physaddr;
extern u_long timer_physaddr;

#define SAU7_SIO01_PHYSADDR 0x10400
#define SAU7_SIO23_PHYSADDR 0x10500
#define SAU7_RTC_PHYSADDR 0x10900
#define SAU7_PICA 0x11000
#define SAU7_PICB 0x11100
#define SAU7_CPUCTRL 0x10100
#define SAU7_TIMER 0x010800

#define SAU8_SIO01_PHYSADDR 0x8400
#define SAU8_RTC_PHYSADDR 0x8900
#define SAU8_PICA 0x9400
#define SAU8_PICB 0x9500
#define SAU8_CPUCTRL 0x8100
#define SAU8_TIMER 0x8800

#define sio01 ((*(volatile struct SCN2681 *)(IO_BASE + sio01_physaddr)))
#define sio23 ((*(volatile struct SCN2681 *)(IO_BASE + sio23_physaddr)))
#define rtc (((volatile struct mc146818 *)(IO_BASE + rtc_physaddr)))
#define cpuctrl (*(volatile unsigned int *)(IO_BASE + cpuctrl_physaddr))
#define pica (IO_BASE + pica_physaddr)
#define picb (IO_BASE + picb_physaddr)
#define timer (IO_BASE + timer_physaddr)
#define addr_xlat_map ((unsigned short *)(IO_BASE + 0x17000))

#define isaIO2mem(x) (((((x) & 0x3f8)  << 7) | (((x) & 0xfc00) >> 6) | ((x) & 0x7)) + 0x40000 + IO_BASE)

#define IRQ_APOLLO	IRQ_USER

#endif
