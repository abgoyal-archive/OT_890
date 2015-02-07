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
 * Alchemy Development Board example suspend userspace interface.
 *
 * (c) 2008 Manuel Lauss <mano@roarinelk.homelinux.net>
 */

#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/suspend.h>
#include <linux/sysfs.h>
#include <asm/mach-au1x00/au1000.h>

/*
 * Generic suspend userspace interface for Alchemy development boards.
 * This code exports a few sysfs nodes under /sys/power/db1x/ which
 * can be used by userspace to en/disable all au1x-provided wakeup
 * sources and configure the timeout after which the the TOYMATCH2 irq
 * is to trigger a wakeup.
 */


static unsigned long db1x_pm_sleep_secs;
static unsigned long db1x_pm_wakemsk;
static unsigned long db1x_pm_last_wakesrc;

static int db1x_pm_enter(suspend_state_t state)
{
	/* enable GPIO based wakeup */
	au_writel(1, SYS_PININPUTEN);

	/* clear and setup wake cause and source */
	au_writel(0, SYS_WAKEMSK);
	au_sync();
	au_writel(0, SYS_WAKESRC);
	au_sync();

	au_writel(db1x_pm_wakemsk, SYS_WAKEMSK);
	au_sync();

	/* setup 1Hz-timer-based wakeup: wait for reg access */
	while (au_readl(SYS_COUNTER_CNTRL) & SYS_CNTRL_M20)
		asm volatile ("nop");

	au_writel(au_readl(SYS_TOYREAD) + db1x_pm_sleep_secs, SYS_TOYMATCH2);
	au_sync();

	/* wait for value to really hit the register */
	while (au_readl(SYS_COUNTER_CNTRL) & SYS_CNTRL_M20)
		asm volatile ("nop");

	/* ...and now the sandman can come! */
	au_sleep();

	return 0;
}

static int db1x_pm_begin(suspend_state_t state)
{
	if (!db1x_pm_wakemsk) {
		printk(KERN_ERR "db1x: no wakeup source activated!\n");
		return -EINVAL;
	}

	return 0;
}

static void db1x_pm_end(void)
{
	/* read and store wakeup source, the clear the register. To
	 * be able to clear it, WAKEMSK must be cleared first.
	 */
	db1x_pm_last_wakesrc = au_readl(SYS_WAKESRC);

	au_writel(0, SYS_WAKEMSK);
	au_writel(0, SYS_WAKESRC);
	au_sync();

}

static struct platform_suspend_ops db1x_pm_ops = {
	.valid		= suspend_valid_only_mem,
	.begin		= db1x_pm_begin,
	.enter		= db1x_pm_enter,
	.end		= db1x_pm_end,
};

#define ATTRCMP(x) (0 == strcmp(attr->attr.name, #x))

static ssize_t db1x_pmattr_show(struct kobject *kobj,
				struct kobj_attribute *attr,
				char *buf)
{
	int idx;

	if (ATTRCMP(timer_timeout))
		return sprintf(buf, "%lu\n", db1x_pm_sleep_secs);

	else if (ATTRCMP(timer))
		return sprintf(buf, "%u\n",
				!!(db1x_pm_wakemsk & SYS_WAKEMSK_M2));

	else if (ATTRCMP(wakesrc))
		return sprintf(buf, "%lu\n", db1x_pm_last_wakesrc);

	else if (ATTRCMP(gpio0) || ATTRCMP(gpio1) || ATTRCMP(gpio2) ||
		 ATTRCMP(gpio3) || ATTRCMP(gpio4) || ATTRCMP(gpio5) ||
		 ATTRCMP(gpio6) || ATTRCMP(gpio7)) {
		idx = (attr->attr.name)[4] - '0';
		return sprintf(buf, "%d\n",
			!!(db1x_pm_wakemsk & SYS_WAKEMSK_GPIO(idx)));

	} else if (ATTRCMP(wakemsk)) {
		return sprintf(buf, "%08lx\n", db1x_pm_wakemsk);
	}

	return -ENOENT;
}

static ssize_t db1x_pmattr_store(struct kobject *kobj,
				 struct kobj_attribute *attr,
				 const char *instr,
				 size_t bytes)
{
	unsigned long l;
	int tmp;

	if (ATTRCMP(timer_timeout)) {
		tmp = strict_strtoul(instr, 0, &l);
		if (tmp)
			return tmp;

		db1x_pm_sleep_secs = l;

	} else if (ATTRCMP(timer)) {
		if (instr[0] != '0')
			db1x_pm_wakemsk |= SYS_WAKEMSK_M2;
		else
			db1x_pm_wakemsk &= ~SYS_WAKEMSK_M2;

	} else if (ATTRCMP(gpio0) || ATTRCMP(gpio1) || ATTRCMP(gpio2) ||
		   ATTRCMP(gpio3) || ATTRCMP(gpio4) || ATTRCMP(gpio5) ||
		   ATTRCMP(gpio6) || ATTRCMP(gpio7)) {
		tmp = (attr->attr.name)[4] - '0';
		if (instr[0] != '0') {
			db1x_pm_wakemsk |= SYS_WAKEMSK_GPIO(tmp);
		} else {
			db1x_pm_wakemsk &= ~SYS_WAKEMSK_GPIO(tmp);
		}

	} else if (ATTRCMP(wakemsk)) {
		tmp = strict_strtoul(instr, 0, &l);
		if (tmp)
			return tmp;

		db1x_pm_wakemsk = l & 0x0000003f;

	} else
		bytes = -ENOENT;

	return bytes;
}

#define ATTR(x)							\
	static struct kobj_attribute x##_attribute = 		\
		__ATTR(x, 0664, db1x_pmattr_show,		\
				db1x_pmattr_store);

ATTR(gpio0)		/* GPIO-based wakeup enable */
ATTR(gpio1)
ATTR(gpio2)
ATTR(gpio3)
ATTR(gpio4)
ATTR(gpio5)
ATTR(gpio6)
ATTR(gpio7)
ATTR(timer)		/* TOYMATCH2-based wakeup enable */
ATTR(timer_timeout)	/* timer-based wakeup timeout value, in seconds */
ATTR(wakesrc)		/* contents of SYS_WAKESRC after last wakeup */
ATTR(wakemsk)		/* direct access to SYS_WAKEMSK */

#define ATTR_LIST(x)	& x ## _attribute.attr
static struct attribute *db1x_pmattrs[] = {
	ATTR_LIST(gpio0),
	ATTR_LIST(gpio1),
	ATTR_LIST(gpio2),
	ATTR_LIST(gpio3),
	ATTR_LIST(gpio4),
	ATTR_LIST(gpio5),
	ATTR_LIST(gpio6),
	ATTR_LIST(gpio7),
	ATTR_LIST(timer),
	ATTR_LIST(timer_timeout),
	ATTR_LIST(wakesrc),
	ATTR_LIST(wakemsk),
	NULL,		/* terminator */
};

static struct attribute_group db1x_pmattr_group = {
	.name	= "db1x",
	.attrs	= db1x_pmattrs,
};

/*
 * Initialize suspend interface
 */
static int __init pm_init(void)
{
	/* init TOY to tick at 1Hz if not already done. No need to wait
	 * for confirmation since there's plenty of time from here to
	 * the next suspend cycle.
	 */
	if (au_readl(SYS_TOYTRIM) != 32767) {
		au_writel(32767, SYS_TOYTRIM);
		au_sync();
	}

	db1x_pm_last_wakesrc = au_readl(SYS_WAKESRC);

	au_writel(0, SYS_WAKESRC);
	au_sync();
	au_writel(0, SYS_WAKEMSK);
	au_sync();

	suspend_set_ops(&db1x_pm_ops);

	return sysfs_create_group(power_kobj, &db1x_pmattr_group);
}

late_initcall(pm_init);
