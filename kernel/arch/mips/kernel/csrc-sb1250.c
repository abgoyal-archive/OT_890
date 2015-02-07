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
 * Copyright (C) 2000, 2001 Broadcom Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include <linux/clocksource.h>

#include <asm/addrspace.h>
#include <asm/io.h>
#include <asm/time.h>

#include <asm/sibyte/sb1250.h>
#include <asm/sibyte/sb1250_regs.h>
#include <asm/sibyte/sb1250_int.h>
#include <asm/sibyte/sb1250_scd.h>

#define SB1250_HPT_NUM		3
#define SB1250_HPT_VALUE	M_SCD_TIMER_CNT /* max value */

/*
 * The HPT is free running from SB1250_HPT_VALUE down to 0 then starts over
 * again.
 */
static cycle_t sb1250_hpt_read(void)
{
	unsigned int count;

	count = G_SCD_TIMER_CNT(__raw_readq(IOADDR(A_SCD_TIMER_REGISTER(SB1250_HPT_NUM, R_SCD_TIMER_CNT))));

	return SB1250_HPT_VALUE - count;
}

struct clocksource bcm1250_clocksource = {
	.name	= "bcm1250-counter-3",
	.rating	= 200,
	.read	= sb1250_hpt_read,
	.mask	= CLOCKSOURCE_MASK(23),
	.flags	= CLOCK_SOURCE_IS_CONTINUOUS,
};

void __init sb1250_clocksource_init(void)
{
	struct clocksource *cs = &bcm1250_clocksource;

	/* Setup hpt using timer #3 but do not enable irq for it */
	__raw_writeq(0,
		     IOADDR(A_SCD_TIMER_REGISTER(SB1250_HPT_NUM,
						 R_SCD_TIMER_CFG)));
	__raw_writeq(SB1250_HPT_VALUE,
		     IOADDR(A_SCD_TIMER_REGISTER(SB1250_HPT_NUM,
						 R_SCD_TIMER_INIT)));
	__raw_writeq(M_SCD_TIMER_ENABLE | M_SCD_TIMER_MODE_CONTINUOUS,
		     IOADDR(A_SCD_TIMER_REGISTER(SB1250_HPT_NUM,
						 R_SCD_TIMER_CFG)));

	clocksource_set_clock(cs, V_SCD_TIMER_FREQ);
	clocksource_register(cs);
}
