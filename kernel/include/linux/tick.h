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

/*  linux/include/linux/tick.h
 *
 *  This file contains the structure definitions for tick related functions
 *
 */
#ifndef _LINUX_TICK_H
#define _LINUX_TICK_H

#include <linux/clockchips.h>
#include <asm/tcm.h>

#ifdef CONFIG_GENERIC_CLOCKEVENTS

enum tick_device_mode {
	TICKDEV_MODE_PERIODIC,
	TICKDEV_MODE_ONESHOT,
};

struct tick_device {
	struct clock_event_device *evtdev;
	enum tick_device_mode mode;
};

enum tick_nohz_mode {
	NOHZ_MODE_INACTIVE,
	NOHZ_MODE_LOWRES,
	NOHZ_MODE_HIGHRES,
};

/**
 * struct tick_sched - sched tick emulation and no idle tick control/stats
 * @sched_timer:	hrtimer to schedule the periodic tick in high
 *			resolution mode
 * @idle_tick:		Store the last idle tick expiry time when the tick
 *			timer is modified for idle sleeps. This is necessary
 *			to resume the tick timer operation in the timeline
 *			when the CPU returns from idle
 * @tick_stopped:	Indicator that the idle tick has been stopped
 * @idle_jiffies:	jiffies at the entry to idle for idle time accounting
 * @idle_calls:		Total number of idle calls
 * @idle_sleeps:	Number of idle calls, where the sched tick was stopped
 * @idle_entrytime:	Time when the idle call was entered
 * @idle_waketime:	Time when the idle was interrupted
 * @idle_exittime:	Time when the idle state was left
 * @idle_sleeptime:	Sum of the time slept in idle with sched tick stopped
 * @sleep_length:	Duration of the current idle sleep
 */
struct tick_sched {
	struct hrtimer			sched_timer;
	unsigned long			check_clocks;
	enum tick_nohz_mode		nohz_mode;
	ktime_t				idle_tick;
	int				inidle;
	int				tick_stopped;
	unsigned long			idle_jiffies;
	unsigned long			idle_calls;
	unsigned long			idle_sleeps;
	int				idle_active;
	ktime_t				idle_entrytime;
	ktime_t				idle_waketime;
	ktime_t				idle_exittime;
	ktime_t				idle_sleeptime;
	ktime_t				idle_lastupdate;
	ktime_t				sleep_length;
	unsigned long			last_jiffies;
	unsigned long			next_jiffies;
	ktime_t				idle_expires;
};

extern void __init tick_init(void);
extern int tick_is_oneshot_available(void);
extern struct tick_device *tick_get_device(int cpu);

# ifdef CONFIG_HIGH_RES_TIMERS
extern int tick_init_highres(void);
extern int tick_program_event(ktime_t expires, int force);
extern void tick_setup_sched_timer(void);
# endif

# if defined CONFIG_NO_HZ || defined CONFIG_HIGH_RES_TIMERS
extern void tick_cancel_sched_timer(int cpu);
# else
static inline void tick_cancel_sched_timer(int cpu) { }
# endif

# ifdef CONFIG_GENERIC_CLOCKEVENTS_BROADCAST
extern struct tick_device *tick_get_broadcast_device(void);
extern struct cpumask *tick_get_broadcast_mask(void);

#  ifdef CONFIG_TICK_ONESHOT
extern struct cpumask *tick_get_broadcast_oneshot_mask(void);
#  endif

# endif /* BROADCAST */

# ifdef CONFIG_TICK_ONESHOT
extern void tick_clock_notify(void);
extern int tick_check_oneshot_change(int allow_nohz);
extern struct tick_sched *tick_get_tick_sched(int cpu);
extern void __tcmfunc tick_check_idle(int cpu);
# else
static inline void tick_clock_notify(void) { }
static inline int tick_check_oneshot_change(int allow_nohz) { return 0; }
static inline void tick_check_idle(int cpu) { }
# endif

#else /* CONFIG_GENERIC_CLOCKEVENTS */
static inline void tick_init(void) { }
static inline void tick_cancel_sched_timer(int cpu) { }
static inline void tick_clock_notify(void) { }
static inline int tick_check_oneshot_change(int allow_nohz) { return 0; }
static inline void tick_check_idle(int cpu) { }
#endif /* !CONFIG_GENERIC_CLOCKEVENTS */

# ifdef CONFIG_NO_HZ
extern void tick_nohz_stop_sched_tick(int inidle);
extern void tick_nohz_restart_sched_tick(void);
extern ktime_t tick_nohz_get_sleep_length(void);
extern u64 get_cpu_idle_time_us(int cpu, u64 *last_update_time);
# else
static inline void tick_nohz_stop_sched_tick(int inidle) { }
static inline void tick_nohz_restart_sched_tick(void) { }
static inline ktime_t tick_nohz_get_sleep_length(void)
{
	ktime_t len = { .tv64 = NSEC_PER_SEC/HZ };

	return len;
}
static inline u64 get_cpu_idle_time_us(int cpu, u64 *unused) { return -1; }
# endif /* !NO_HZ */

#endif
