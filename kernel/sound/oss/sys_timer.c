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
 * sound/oss/sys_timer.c
 *
 * The default timer for the Level 2 sequencer interface
 * Uses the (1/HZ sec) timer of kernel.
 */
/*
 * Copyright (C) by Hannu Savolainen 1993-1997
 *
 * OSS/Free for Linux is distributed under the GNU GENERAL PUBLIC LICENSE (GPL)
 * Version 2 (June 1991). See the "COPYING" file distributed with this software
 * for more info.
 */
/*
 * Thomas Sailer   : ioctl code reworked (vmalloc/vfree removed)
 * Andrew Veliath  : adapted tmr2ticks from level 1 sequencer (avoid overflow)
 */
#include <linux/spinlock.h>
#include "sound_config.h"

static volatile int opened, tmr_running;
static volatile time_t tmr_offs, tmr_ctr;
static volatile unsigned long ticks_offs;
static volatile int curr_tempo, curr_timebase;
static volatile unsigned long curr_ticks;
static volatile unsigned long next_event_time;
static unsigned long prev_event_time;

static void     poll_def_tmr(unsigned long dummy);
static DEFINE_SPINLOCK(lock);
static DEFINE_TIMER(def_tmr, poll_def_tmr, 0, 0);

static unsigned long
tmr2ticks(int tmr_value)
{
	/*
	 *    Convert timer ticks to MIDI ticks
	 */

	unsigned long tmp;
	unsigned long scale;

	/* tmr_value (ticks per sec) *
	   1000000 (usecs per sec) / HZ (ticks per sec) -=> usecs */
	tmp = tmr_value * (1000000 / HZ);
	scale = (60 * 1000000) / (curr_tempo * curr_timebase);	/* usecs per MIDI tick */
	return (tmp + scale / 2) / scale;
}

static void
poll_def_tmr(unsigned long dummy)
{

	if (opened)
	  {

		  {
			  def_tmr.expires = (1) + jiffies;
			  add_timer(&def_tmr);
		  };

		  if (tmr_running)
		    {
				spin_lock(&lock);
			    tmr_ctr++;
			    curr_ticks = ticks_offs + tmr2ticks(tmr_ctr);

			    if (curr_ticks >= next_event_time)
			      {
				      next_event_time = (unsigned long) -1;
				      sequencer_timer(0);
			      }
				spin_unlock(&lock);
		    }
	  }
}

static void
tmr_reset(void)
{
	unsigned long   flags;

	spin_lock_irqsave(&lock,flags);
	tmr_offs = 0;
	ticks_offs = 0;
	tmr_ctr = 0;
	next_event_time = (unsigned long) -1;
	prev_event_time = 0;
	curr_ticks = 0;
	spin_unlock_irqrestore(&lock,flags);
}

static int
def_tmr_open(int dev, int mode)
{
	if (opened)
		return -EBUSY;

	tmr_reset();
	curr_tempo = 60;
	curr_timebase = 100;
	opened = 1;

	;

	{
		def_tmr.expires = (1) + jiffies;
		add_timer(&def_tmr);
	};

	return 0;
}

static void
def_tmr_close(int dev)
{
	opened = tmr_running = 0;
	del_timer(&def_tmr);
}

static int
def_tmr_event(int dev, unsigned char *event)
{
	unsigned char   cmd = event[1];
	unsigned long   parm = *(int *) &event[4];

	switch (cmd)
	  {
	  case TMR_WAIT_REL:
		  parm += prev_event_time;
	  case TMR_WAIT_ABS:
		  if (parm > 0)
		    {
			    long            time;

			    if (parm <= curr_ticks)	/* It's the time */
				    return TIMER_NOT_ARMED;

			    time = parm;
			    next_event_time = prev_event_time = time;

			    return TIMER_ARMED;
		    }
		  break;

	  case TMR_START:
		  tmr_reset();
		  tmr_running = 1;
		  break;

	  case TMR_STOP:
		  tmr_running = 0;
		  break;

	  case TMR_CONTINUE:
		  tmr_running = 1;
		  break;

	  case TMR_TEMPO:
		  if (parm)
		    {
			    if (parm < 8)
				    parm = 8;
			    if (parm > 360)
				    parm = 360;
			    tmr_offs = tmr_ctr;
			    ticks_offs += tmr2ticks(tmr_ctr);
			    tmr_ctr = 0;
			    curr_tempo = parm;
		    }
		  break;

	  case TMR_ECHO:
		  seq_copy_to_input(event, 8);
		  break;

	  default:;
	  }

	return TIMER_NOT_ARMED;
}

static unsigned long
def_tmr_get_time(int dev)
{
	if (!opened)
		return 0;

	return curr_ticks;
}

/* same as sound_timer.c:timer_ioctl!? */
static int def_tmr_ioctl(int dev, unsigned int cmd, void __user *arg)
{
	int __user *p = arg;
	int val;

	switch (cmd) {
	case SNDCTL_TMR_SOURCE:
		return __put_user(TMR_INTERNAL, p);

	case SNDCTL_TMR_START:
		tmr_reset();
		tmr_running = 1;
		return 0;

	case SNDCTL_TMR_STOP:
		tmr_running = 0;
		return 0;

	case SNDCTL_TMR_CONTINUE:
		tmr_running = 1;
		return 0;

	case SNDCTL_TMR_TIMEBASE:
		if (__get_user(val, p))
			return -EFAULT;
		if (val) {
			if (val < 1)
				val = 1;
			if (val > 1000)
				val = 1000;
			curr_timebase = val;
		}
		return __put_user(curr_timebase, p);

	case SNDCTL_TMR_TEMPO:
		if (__get_user(val, p))
			return -EFAULT;
		if (val) {
			if (val < 8)
				val = 8;
			if (val > 250)
				val = 250;
			tmr_offs = tmr_ctr;
			ticks_offs += tmr2ticks(tmr_ctr);
			tmr_ctr = 0;
			curr_tempo = val;
			reprogram_timer();
		}
		return __put_user(curr_tempo, p);

	case SNDCTL_SEQ_CTRLRATE:
		if (__get_user(val, p))
			return -EFAULT;
		if (val != 0)	/* Can't change */
			return -EINVAL;
		val = ((curr_tempo * curr_timebase) + 30) / 60;
		return __put_user(val, p);
		
	case SNDCTL_SEQ_GETTIME:
		return __put_user(curr_ticks, p);
		
	case SNDCTL_TMR_METRONOME:
		/* NOP */
		break;
		
	default:;
	}
	return -EINVAL;
}

static void
def_tmr_arm(int dev, long time)
{
	if (time < 0)
		time = curr_ticks + 1;
	else if (time <= curr_ticks)	/* It's the time */
		return;

	next_event_time = prev_event_time = time;

	return;
}

struct sound_timer_operations default_sound_timer =
{
	.owner		= THIS_MODULE,
	.info		= {"System clock", 0},
	.priority	= 0,	/* Priority */
	.devlink	= 0,	/* Local device link */
	.open		= def_tmr_open,
	.close		= def_tmr_close,
	.event		= def_tmr_event,
	.get_time	= def_tmr_get_time,
	.ioctl		= def_tmr_ioctl,
	.arm_timer	= def_tmr_arm
};
