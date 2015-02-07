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

/* ASB2305 timer specifcations
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#ifndef _ASM_UNIT_TIMEX_H
#define _ASM_UNIT_TIMEX_H

#ifndef __ASSEMBLY__
#include <linux/irq.h>
#endif /* __ASSEMBLY__ */

#include <asm/cpu/timer-regs.h>
#include <asm/unit/clock.h>

/*
 * jiffies counter specifications
 */

#define	TMJCBR_MAX		0xffff
#define	TMJCBC			TM01BC

#define	TMJCMD			TM01MD
#define	TMJCBR			TM01BR
#define	TMJCIRQ			TM1IRQ
#define	TMJCICR			TM1ICR
#define	TMJCICR_LEVEL		GxICR_LEVEL_5

#ifndef __ASSEMBLY__

static inline void startup_jiffies_counter(void)
{
	unsigned rate;
	u16 md, t16;

	/* use as little prescaling as possible to avoid losing accuracy */
	md = TM0MD_SRC_IOCLK;
	rate = MN10300_JCCLK / HZ;

	if (rate > TMJCBR_MAX) {
		md = TM0MD_SRC_IOCLK_8;
		rate = MN10300_JCCLK / 8 / HZ;

		if (rate > TMJCBR_MAX) {
			md = TM0MD_SRC_IOCLK_32;
			rate = MN10300_JCCLK / 32 / HZ;

			if (rate > TMJCBR_MAX)
				BUG();
		}
	}

	TMJCBR = rate - 1;
	t16 = TMJCBR;

	TMJCMD =
		md |
		TM1MD_SRC_TM0CASCADE << 8 |
		TM0MD_INIT_COUNTER |
		TM1MD_INIT_COUNTER << 8;

	TMJCMD =
		md |
		TM1MD_SRC_TM0CASCADE << 8 |
		TM0MD_COUNT_ENABLE |
		TM1MD_COUNT_ENABLE << 8;

	t16 = TMJCMD;

	TMJCICR |= GxICR_ENABLE | GxICR_DETECT | GxICR_REQUEST;
	t16 = TMJCICR;
}

static inline void shutdown_jiffies_counter(void)
{
}

#endif /* !__ASSEMBLY__ */


/*
 * timestamp counter specifications
 */

#define	TMTSCBR_MAX		0xffffffff
#define	TMTSCBC			TM45BC

#ifndef __ASSEMBLY__

static inline void startup_timestamp_counter(void)
{
	/* set up timer 4 & 5 cascaded as a 32-bit counter to count real time
	 * - count down from 4Gig-1 to 0 and wrap at IOCLK rate
	 */
	TM45BR = TMTSCBR_MAX;

	TM4MD = TM4MD_SRC_IOCLK;
	TM4MD |= TM4MD_INIT_COUNTER;
	TM4MD &= ~TM4MD_INIT_COUNTER;
	TM4ICR = 0;

	TM5MD = TM5MD_SRC_TM4CASCADE;
	TM5MD |= TM5MD_INIT_COUNTER;
	TM5MD &= ~TM5MD_INIT_COUNTER;
	TM5ICR = 0;

	TM5MD |= TM5MD_COUNT_ENABLE;
	TM4MD |= TM4MD_COUNT_ENABLE;
}

static inline void shutdown_timestamp_counter(void)
{
	TM4MD = 0;
	TM5MD = 0;
}

/*
 * we use a cascaded pair of 16-bit down-counting timers to count I/O
 * clock cycles for the purposes of time keeping
 */
typedef unsigned long cycles_t;

static inline cycles_t read_timestamp_counter(void)
{
	return (cycles_t) TMTSCBC;
}

#endif /* !__ASSEMBLY__ */

#endif /* _ASM_UNIT_TIMEX_H */
