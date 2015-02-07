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
 * Smp timebase synchronization for ppc.
 *
 * Copyright (C) 2003 Samuel Rydh (samuel@ibrium.se)
 *
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/unistd.h>
#include <linux/init.h>
#include <asm/atomic.h>
#include <asm/smp.h>
#include <asm/time.h>

#define NUM_ITER		300

enum {
	kExit=0, kSetAndTest, kTest
};

static struct {
	volatile u64		tb;
	volatile u64		mark;
	volatile int		cmd;
	volatile int		handshake;
	int			filler[2];

	volatile int		ack;
	int			filler2[7];

	volatile int		race_result;
} *tbsync;

static volatile int		running;

static void __devinit enter_contest(u64 mark, long add)
{
	while (get_tb() < mark)
		tbsync->race_result = add;
}

void __devinit smp_generic_take_timebase(void)
{
	int cmd;
	u64 tb;
	unsigned long flags;

	local_irq_save(flags);
	while (!running)
		barrier();
	rmb();

	for (;;) {
		tbsync->ack = 1;
		while (!tbsync->handshake)
			barrier();
		rmb();

		cmd = tbsync->cmd;
		tb = tbsync->tb;
		mb();
		tbsync->ack = 0;
		if (cmd == kExit)
			break;

		while (tbsync->handshake)
			barrier();
		if (cmd == kSetAndTest)
			set_tb(tb >> 32, tb & 0xfffffffful);
		enter_contest(tbsync->mark, -1);
	}
	local_irq_restore(flags);
}

static int __devinit start_contest(int cmd, long offset, int num)
{
	int i, score=0;
	u64 tb;
	u64 mark;

	tbsync->cmd = cmd;

	local_irq_disable();
	for (i = -3; i < num; ) {
		tb = get_tb() + 400;
		tbsync->tb = tb + offset;
		tbsync->mark = mark = tb + 400;

		wmb();

		tbsync->handshake = 1;
		while (tbsync->ack)
			barrier();

		while (get_tb() <= tb)
			barrier();
		tbsync->handshake = 0;
		enter_contest(mark, 1);

		while (!tbsync->ack)
			barrier();

		if (i++ > 0)
			score += tbsync->race_result;
	}
	local_irq_enable();
	return score;
}

void __devinit smp_generic_give_timebase(void)
{
	int i, score, score2, old, min=0, max=5000, offset=1000;

	pr_debug("Software timebase sync\n");

	/* if this fails then this kernel won't work anyway... */
	tbsync = kzalloc( sizeof(*tbsync), GFP_KERNEL );
	mb();
	running = 1;

	while (!tbsync->ack)
		barrier();

	pr_debug("Got ack\n");

	/* binary search */
	for (old = -1; old != offset ; offset = (min+max) / 2) {
		score = start_contest(kSetAndTest, offset, NUM_ITER);

		pr_debug("score %d, offset %d\n", score, offset );

		if( score > 0 )
			max = offset;
		else
			min = offset;
		old = offset;
	}
	score = start_contest(kSetAndTest, min, NUM_ITER);
	score2 = start_contest(kSetAndTest, max, NUM_ITER);

	pr_debug("Min %d (score %d), Max %d (score %d)\n",
		 min, score, max, score2);
	score = abs(score);
	score2 = abs(score2);
	offset = (score < score2) ? min : max;

	/* guard against inaccurate mttb */
	for (i = 0; i < 10; i++) {
		start_contest(kSetAndTest, offset, NUM_ITER/10);

		if ((score2 = start_contest(kTest, offset, NUM_ITER)) < 0)
			score2 = -score2;
		if (score2 <= score || score2 < 20)
			break;
	}
	pr_debug("Final offset: %d (%d/%d)\n", offset, score2, NUM_ITER );

	/* exiting */
	tbsync->cmd = kExit;
	wmb();
	tbsync->handshake = 1;
	while (tbsync->ack)
		barrier();
	tbsync->handshake = 0;
	kfree(tbsync);
	tbsync = NULL;
	running = 0;
}
