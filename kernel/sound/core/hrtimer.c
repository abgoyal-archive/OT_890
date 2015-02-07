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
 * ALSA timer back-end using hrtimer
 * Copyright (C) 2008 Takashi Iwai
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/hrtimer.h>
#include <sound/core.h>
#include <sound/timer.h>

MODULE_AUTHOR("Takashi Iwai <tiwai@suse.de>");
MODULE_DESCRIPTION("ALSA hrtimer backend");
MODULE_LICENSE("GPL");

MODULE_ALIAS("snd-timer-" __stringify(SNDRV_TIMER_GLOBAL_HRTIMER));

#define NANO_SEC	1000000000UL	/* 10^9 in sec */
static unsigned int resolution;

struct snd_hrtimer {
	struct snd_timer *timer;
	struct hrtimer hrt;
};

static enum hrtimer_restart snd_hrtimer_callback(struct hrtimer *hrt)
{
	struct snd_hrtimer *stime = container_of(hrt, struct snd_hrtimer, hrt);
	struct snd_timer *t = stime->timer;
	hrtimer_forward_now(hrt, ns_to_ktime(t->sticks * resolution));
	snd_timer_interrupt(stime->timer, t->sticks);
	return HRTIMER_RESTART;
}

static int snd_hrtimer_open(struct snd_timer *t)
{
	struct snd_hrtimer *stime;

	stime = kmalloc(sizeof(*stime), GFP_KERNEL);
	if (!stime)
		return -ENOMEM;
	hrtimer_init(&stime->hrt, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	stime->timer = t;
	stime->hrt.function = snd_hrtimer_callback;
	t->private_data = stime;
	return 0;
}

static int snd_hrtimer_close(struct snd_timer *t)
{
	struct snd_hrtimer *stime = t->private_data;

	if (stime) {
		hrtimer_cancel(&stime->hrt);
		kfree(stime);
		t->private_data = NULL;
	}
	return 0;
}

static int snd_hrtimer_start(struct snd_timer *t)
{
	struct snd_hrtimer *stime = t->private_data;

	hrtimer_start(&stime->hrt, ns_to_ktime(t->sticks * resolution),
		      HRTIMER_MODE_REL);
	return 0;
}

static int snd_hrtimer_stop(struct snd_timer *t)
{
	struct snd_hrtimer *stime = t->private_data;

	hrtimer_cancel(&stime->hrt);
	return 0;
}

static struct snd_timer_hardware hrtimer_hw = {
	.flags =	SNDRV_TIMER_HW_AUTO,
	.open =		snd_hrtimer_open,
	.close =	snd_hrtimer_close,
	.start =	snd_hrtimer_start,
	.stop =		snd_hrtimer_stop,
};

/*
 * entry functions
 */

static struct snd_timer *mytimer;

static int __init snd_hrtimer_init(void)
{
	struct snd_timer *timer;
	struct timespec tp;
	int err;

	hrtimer_get_res(CLOCK_MONOTONIC, &tp);
	if (tp.tv_sec > 0 || !tp.tv_nsec) {
		snd_printk(KERN_ERR
			   "snd-hrtimer: Invalid resolution %u.%09u",
			   (unsigned)tp.tv_sec, (unsigned)tp.tv_nsec);
		return -EINVAL;
	}
	resolution = tp.tv_nsec;

	/* Create a new timer and set up the fields */
	err = snd_timer_global_new("hrtimer", SNDRV_TIMER_GLOBAL_HRTIMER,
				   &timer);
	if (err < 0)
		return err;

	timer->module = THIS_MODULE;
	strcpy(timer->name, "HR timer");
	timer->hw = hrtimer_hw;
	timer->hw.resolution = resolution;
	timer->hw.ticks = NANO_SEC / resolution;

	err = snd_timer_global_register(timer);
	if (err < 0) {
		snd_timer_global_free(timer);
		return err;
	}
	mytimer = timer; /* remember this */

	return 0;
}

static void __exit snd_hrtimer_exit(void)
{
	if (mytimer) {
		snd_timer_global_free(mytimer);
		mytimer = NULL;
	}
}

module_init(snd_hrtimer_init);
module_exit(snd_hrtimer_exit);
