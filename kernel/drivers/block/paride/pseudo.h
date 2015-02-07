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
        pseudo.h    (c) 1997-8  Grant R. Guenther <grant@torque.net>
                                Under the terms of the GNU General Public License.

	This is the "pseudo-interrupt" logic for parallel port drivers.

        This module is #included into each driver.  It makes one
        function available:

		ps_set_intr( void (*continuation)(void),
			     int  (*ready)(void),
			     int timeout,
			     int nice )

	Which will arrange for ready() to be evaluated frequently and
	when either it returns true, or timeout jiffies have passed,
	continuation() will be invoked.

	If nice is 1, the test will done approximately once a
	jiffy.  If nice is 0, the test will also be done whenever
	the scheduler runs (by adding it to a task queue).  If
	nice is greater than 1, the test will be done once every
	(nice-1) jiffies. 

*/

/* Changes:

	1.01	1998.05.03	Switched from cli()/sti() to spinlocks
	1.02    1998.12.14      Added support for nice > 1
*/
	
#define PS_VERSION	"1.02"

#include <linux/sched.h>
#include <linux/workqueue.h>

static void ps_tq_int(struct work_struct *work);

static void (* ps_continuation)(void);
static int (* ps_ready)(void);
static unsigned long ps_timeout;
static int ps_tq_active = 0;
static int ps_nice = 0;

static DEFINE_SPINLOCK(ps_spinlock __attribute__((unused)));

static DECLARE_DELAYED_WORK(ps_tq, ps_tq_int);

static void ps_set_intr(void (*continuation)(void), 
			int (*ready)(void),
			int timeout, int nice)
{
	unsigned long	flags;

	spin_lock_irqsave(&ps_spinlock,flags);

	ps_continuation = continuation;
	ps_ready = ready;
	ps_timeout = jiffies + timeout;
	ps_nice = nice;

	if (!ps_tq_active) {
		ps_tq_active = 1;
		if (!ps_nice)
			schedule_delayed_work(&ps_tq, 0);
		else
			schedule_delayed_work(&ps_tq, ps_nice-1);
	}
	spin_unlock_irqrestore(&ps_spinlock,flags);
}

static void ps_tq_int(struct work_struct *work)
{
	void (*con)(void);
	unsigned long flags;

	spin_lock_irqsave(&ps_spinlock,flags);

	con = ps_continuation;
	ps_tq_active = 0;

	if (!con) {
		spin_unlock_irqrestore(&ps_spinlock,flags);
		return;
	}
	if (!ps_ready || ps_ready() || time_after_eq(jiffies, ps_timeout)) {
		ps_continuation = NULL;
		spin_unlock_irqrestore(&ps_spinlock,flags);
		con();
		return;
	}
	ps_tq_active = 1;
	if (!ps_nice)
		schedule_delayed_work(&ps_tq, 0);
	else
		schedule_delayed_work(&ps_tq, ps_nice-1);
	spin_unlock_irqrestore(&ps_spinlock,flags);
}

/* end of pseudo.h */

