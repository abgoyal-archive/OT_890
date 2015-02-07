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

/* ASB2305-specific LEDs
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

#ifndef _ASM_UNIT_LEDS_H
#define _ASM_UNIT_LEDS_H

#include <asm/pio-regs.h>
#include <asm/cpu-regs.h>
#include <asm/exceptions.h>

#define ASB2305_7SEGLEDS	__SYSREG(0xA6F90000, u32)

/* perform a hard reset by driving PIO06 low */
#define mn10300_unit_hard_reset()		\
do {						\
	P0OUT &= 0xbf;				\
	P0MD = (P0MD & P0MD_6) | P0MD_6_OUT;	\
} while (0)

/*
 * use the 7-segment LEDs to indicate states
 */
/* indicate double-fault by displaying "db-f" on the LEDs */
#define mn10300_set_dbfleds			\
	mov	0x43077f1d,d0		;	\
	mov	d0,(ASB2305_7SEGLEDS)

/* flip the 7-segment LEDs between "Gdb-" and "----" */
#define mn10300_set_gdbleds(ONOFF)				\
do {								\
	ASB2305_7SEGLEDS = (ONOFF) ? 0x8543077f : 0x7f7f7f7f;	\
} while (0)

#ifndef __ASSEMBLY__
extern void peripheral_leds_display_exception(enum exception_code);
extern void peripheral_leds_led_chase(void);
extern void peripheral_leds7x4_display_dec(unsigned int, unsigned int);
extern void peripheral_leds7x4_display_hex(unsigned int, unsigned int);
extern void peripheral_leds7x4_display_minssecs(unsigned int, unsigned int);
extern void peripheral_leds7x4_display_rtc(void);
#endif /* __ASSEMBLY__ */

#endif /* _ASM_UNIT_LEDS_H */
