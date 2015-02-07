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
 * arch/m68k/sun3/intersil.c
 *
 * basic routines for accessing the intersil clock within the sun3 machines
 *
 * started 11/12/1999 Sam Creasey
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */

#include <linux/kernel.h>
#include <linux/rtc.h>

#include <asm/errno.h>
#include <asm/system.h>
#include <asm/rtc.h>
#include <asm/intersil.h>


/* bits to set for start/run of the intersil */
#define STOP_VAL (INTERSIL_STOP | INTERSIL_INT_ENABLE | INTERSIL_24H_MODE)
#define START_VAL (INTERSIL_RUN | INTERSIL_INT_ENABLE | INTERSIL_24H_MODE)

/* does this need to be implemented? */
unsigned long sun3_gettimeoffset(void)
{
  return 1;
}


/* get/set hwclock */

int sun3_hwclk(int set, struct rtc_time *t)
{
	volatile struct intersil_dt *todintersil;
	unsigned long flags;

        todintersil = (struct intersil_dt *) &intersil_clock->counter;

	local_irq_save(flags);

	intersil_clock->cmd_reg = STOP_VAL;

	/* set or read the clock */
	if(set) {
		todintersil->csec = 0;
		todintersil->hour = t->tm_hour;
		todintersil->minute = t->tm_min;
		todintersil->second = t->tm_sec;
		todintersil->month = t->tm_mon;
		todintersil->day = t->tm_mday;
		todintersil->year = t->tm_year - 68;
		todintersil->weekday = t->tm_wday;
	} else {
		/* read clock */
		t->tm_sec = todintersil->csec;
		t->tm_hour = todintersil->hour;
		t->tm_min = todintersil->minute;
		t->tm_sec = todintersil->second;
		t->tm_mon = todintersil->month;
		t->tm_mday = todintersil->day;
		t->tm_year = todintersil->year + 68;
		t->tm_wday = todintersil->weekday;
	}

	intersil_clock->cmd_reg = START_VAL;

	local_irq_restore(flags);

	return 0;

}

