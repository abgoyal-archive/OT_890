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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/******************************************************************************
 * mt6516_timer.c - This file is for profiling !
 * 
 * Copyright 2008-2009 MediaTek Co.,Ltd. 
 *
 * modification history
 * ----------------------------------------
 * 08/20/2008, Shu-Hsin Chang written
 * ----------------------------------------
 ******************************************************************************/
   
#include <mach/irqs.h>
#include <asm/io.h>
#include <mach/dma.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/irq.h>

#include <mach/mt6516_gpt_sw.h>
#include <mach/mt6516.h>
#include <linux/sched.h>
#include <mach/mt6516_timer.h>

/******************************************************************************
 Timer Macro
******************************************************************************/
#define GPT_GET_US(x)   ( x / 13 )

/* macro to read the 32 bit timer */
/* the value in this register is updated regularly by HW */
/* every 13000000 equals to 1s */
#define READ_TIMER_US     GPT_GET_US(__raw_readl(MT6516_XGPT2_COUNT))
#define TIMER_LOAD_VAL    0xffffffff
#define MAX_XGPT_REG_US   GPT_GET_US(TIMER_LOAD_VAL)
#define MAX_TIMESTAMP_US  0xffffffff


/******************************************************************************
 hw_timer_us : return hardware counter value (us)
******************************************************************************/
ulong hw_timer_us (void)
{	return READ_TIMER_US;
}

/******************************************************************************
 Global Variable
******************************************************************************/
static ulong timestamp = 0;
/* lastinc is used to record the counter value (us) in xgpt */
static ulong lastinc;

static spinlock_t timer_lock = SPIN_LOCK_UNLOCKED;

/******************************************************************************
 reset_timer_masked : init global variables
******************************************************************************/
void reset_timestamp(void)
{
	spin_lock(&timer_lock);	

	lastinc   = READ_TIMER_US;
	timestamp = 0;

	spin_unlock(&timer_lock);	
}

/******************************************************************************
 get_timestamp : update timestamp and lastinc, then return current timestamp
******************************************************************************/
ulong get_timestamp (void)
{
	ulong current_counter = 0;

	spin_lock(&timer_lock);		
	
	current_counter = READ_TIMER_US;

	if (current_counter >= lastinc) {
		/* lastinc (xgpt) normal */
		timestamp += current_counter - lastinc; 	
	}
	else 
	{ 
		/* lastinc (xgpt) overflow */
		timestamp += MAX_XGPT_REG_US - lastinc + current_counter; 
	}
	lastinc = current_counter;

	spin_unlock(&timer_lock);	
	
	return timestamp;
}

/******************************************************************************
 get_timer_us: return the difference between current timestamp (us) and base (us)
******************************************************************************/
ulong get_timer_us (ulong base)
{
	ulong current_timestamp = get_timestamp ();

	if(current_timestamp >= base)
	{	/* timestamp normal */
		return current_timestamp - base;
	}
	else
	{	/* timestamp overflow */
		return MAX_TIMESTAMP_US - ( base - current_timestamp );
	}
}

/******************************************************************************
 init us timer (xgpt2) for profiling usage
******************************************************************************/
static int __init init_us_timer(void)
{

	/*  init lastinc and timestamp */
    reset_timestamp();

    return 0;
}

arch_initcall(init_us_timer);

