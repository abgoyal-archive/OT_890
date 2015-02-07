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

/* rtc-parisc: RTC for HP PA-RISC firmware
 *
 * Copyright (C) 2008 Kyle McMartin <kyle@mcmartin.ca>
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/time.h>
#include <linux/platform_device.h>

#include <asm/rtc.h>

/* as simple as can be, and no simpler. */
struct parisc_rtc {
	struct rtc_device *rtc;
	spinlock_t lock;
};

static int parisc_get_time(struct device *dev, struct rtc_time *tm)
{
	struct parisc_rtc *p = dev_get_drvdata(dev);
	unsigned long flags, ret;

	spin_lock_irqsave(&p->lock, flags);
	ret = get_rtc_time(tm);
	spin_unlock_irqrestore(&p->lock, flags);

	if (ret & RTC_BATT_BAD)
		return -EOPNOTSUPP;

	return 0;
}

static int parisc_set_time(struct device *dev, struct rtc_time *tm)
{
	struct parisc_rtc *p = dev_get_drvdata(dev);
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&p->lock, flags);
	ret = set_rtc_time(tm);
	spin_unlock_irqrestore(&p->lock, flags);

	if (ret < 0)
		return -EOPNOTSUPP;

	return 0;
}

static const struct rtc_class_ops parisc_rtc_ops = {
	.read_time = parisc_get_time,
	.set_time = parisc_set_time,
};

static int __devinit parisc_rtc_probe(struct platform_device *dev)
{
	struct parisc_rtc *p;

	p = kzalloc(sizeof (*p), GFP_KERNEL);
	if (!p)
		return -ENOMEM;

	spin_lock_init(&p->lock);

	p->rtc = rtc_device_register("rtc-parisc", &dev->dev, &parisc_rtc_ops,
					THIS_MODULE);
	if (IS_ERR(p->rtc)) {
		int err = PTR_ERR(p->rtc);
		kfree(p);
		return err;
	}

	platform_set_drvdata(dev, p);

	return 0;
}

static int __devexit parisc_rtc_remove(struct platform_device *dev)
{
	struct parisc_rtc *p = platform_get_drvdata(dev);

	rtc_device_unregister(p->rtc);
	kfree(p);

	return 0;
}

static struct platform_driver parisc_rtc_driver = {
	.driver = {
		.name = "rtc-parisc",
		.owner = THIS_MODULE,
	},
	.probe = parisc_rtc_probe,
	.remove = __devexit_p(parisc_rtc_remove),
};

static int __init parisc_rtc_init(void)
{
	return platform_driver_register(&parisc_rtc_driver);
}

static void __exit parisc_rtc_fini(void)
{
	platform_driver_unregister(&parisc_rtc_driver);
}

module_init(parisc_rtc_init);
module_exit(parisc_rtc_fini);

MODULE_AUTHOR("Kyle McMartin <kyle@mcmartin.ca>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HP PA-RISC RTC driver");
