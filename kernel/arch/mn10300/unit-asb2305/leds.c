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

/* ASB2305 Peripheral 7-segment LEDs x4 support
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
#include <linux/param.h>
#include <linux/init.h>
#include <asm/io.h>
#include <asm/processor.h>
#include <asm/cpu/intctl-regs.h>
#include <asm/cpu/rtc-regs.h>
#include <asm/unit/leds.h>

static const u8 asb2305_led_hex_tbl[16] = {
	0x80, 0xf2, 0x48, 0x60, 0x32, 0x24, 0x04, 0xf0,
	0x00, 0x20, 0x10, 0x06, 0x8c, 0x42, 0x0c, 0x1c
};

static const u32 asb2305_led_chase_tbl[6] = {
	~0x02020202,	/* top		- segA */
	~0x04040404,	/* right top	- segB */
	~0x08080808,	/* right bottom	- segC */
	~0x10101010,	/* bottom	- segD */
	~0x20202020,	/* left bottom	- segE */
	~0x40404040,	/* left top	- segF */
};

static unsigned asb2305_led_chase;

void peripheral_leds7x4_display_dec(unsigned int val, unsigned int points)
{
	u32 leds;

	leds = asb2305_led_hex_tbl[(val/1000) % 10];
	leds <<= 8;
	leds |= asb2305_led_hex_tbl[(val/100) % 10];
	leds <<= 8;
	leds |= asb2305_led_hex_tbl[(val/10) % 10];
	leds <<= 8;
	leds |= asb2305_led_hex_tbl[val % 10];
	leds |= points^0x01010101;

	ASB2305_7SEGLEDS = leds;
}

void peripheral_leds7x4_display_hex(unsigned int val, unsigned int points)
{
	u32 leds;

	leds = asb2305_led_hex_tbl[(val/1000) % 10];
	leds <<= 8;
	leds |= asb2305_led_hex_tbl[(val/100) % 10];
	leds <<= 8;
	leds |= asb2305_led_hex_tbl[(val/10) % 10];
	leds <<= 8;
	leds |= asb2305_led_hex_tbl[val % 10];
	leds |= points^0x01010101;

	ASB2305_7SEGLEDS = leds;
}

void peripheral_leds_display_exception(enum exception_code code)
{
	u32 leds;

	leds = asb2305_led_hex_tbl[(code/0x100) % 0x10];
	leds <<= 8;
	leds |= asb2305_led_hex_tbl[(code/0x10) % 0x10];
	leds <<= 8;
	leds |= asb2305_led_hex_tbl[code % 0x10];
	leds |= 0x6d010101;

	ASB2305_7SEGLEDS = leds;
}

void peripheral_leds7x4_display_minssecs(unsigned int time, unsigned int points)
{
	u32 leds;

	leds = asb2305_led_hex_tbl[(time/600) % 6];
	leds <<= 8;
	leds |= asb2305_led_hex_tbl[(time/60) % 10];
	leds <<= 8;
	leds |= asb2305_led_hex_tbl[(time/10) % 6];
	leds <<= 8;
	leds |= asb2305_led_hex_tbl[time % 10];
	leds |= points^0x01010101;

	ASB2305_7SEGLEDS = leds;
}

void peripheral_leds7x4_display_rtc(void)
{
	unsigned int clock;
	u8 mins, secs;

	mins = RTMCR;
	secs = RTSCR;

	clock = ((mins & 0xf0) >> 4);
	clock *= 10;
	clock += (mins & 0x0f);
	clock *= 6;

	clock += ((secs & 0xf0) >> 4);
	clock *= 10;
	clock += (secs & 0x0f);

	peripheral_leds7x4_display_minssecs(clock, 0);
}

void peripheral_leds_led_chase(void)
{
	ASB2305_7SEGLEDS = asb2305_led_chase_tbl[asb2305_led_chase];
	asb2305_led_chase++;
	if (asb2305_led_chase >= 6)
		asb2305_led_chase = 0;
}
