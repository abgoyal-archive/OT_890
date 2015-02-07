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

/* rtc-starfire.c: Starfire platform RTC driver.
 *
 * Copyright (C) 2008 David S. Miller <davem@davemloft.net>
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/rtc.h>
#include <linux/platform_device.h>

#include <asm/oplib.h>

MODULE_AUTHOR("David S. Miller <davem@davemloft.net>");
MODULE_DESCRIPTION("Starfire RTC driver");
MODULE_LICENSE("GPL");

static u32 starfire_get_time(void)
{
	static char obp_gettod[32];
	static u32 unix_tod;

	sprintf(obp_gettod, "h# %08x unix-gettod",
		(unsigned int) (long) &unix_tod);
	prom_feval(obp_gettod);

	return unix_tod;
}

static int starfire_read_time(struct device *dev, struct rtc_time *tm)
{
	rtc_time_to_tm(starfire_get_time(), tm);
	return rtc_valid_tm(tm);
}

static const struct rtc_class_ops starfire_rtc_ops = {
	.read_time	= starfire_read_time,
};

static int __init starfire_rtc_probe(struct platform_device *pdev)
{
	struct rtc_device *rtc = rtc_device_register("starfire", &pdev->dev,
				     &starfire_rtc_ops, THIS_MODULE);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	platform_set_drvdata(pdev, rtc);

	return 0;
}

static int __exit starfire_rtc_remove(struct platform_device *pdev)
{
	struct rtc_device *rtc = platform_get_drvdata(pdev);

	rtc_device_unregister(rtc);

	return 0;
}

static struct platform_driver starfire_rtc_driver = {
	.driver		= {
		.name	= "rtc-starfire",
		.owner	= THIS_MODULE,
	},
	.remove		= __exit_p(starfire_rtc_remove),
};

static int __init starfire_rtc_init(void)
{
	return platform_driver_probe(&starfire_rtc_driver, starfire_rtc_probe);
}

static void __exit starfire_rtc_exit(void)
{
	platform_driver_unregister(&starfire_rtc_driver);
}

module_init(starfire_rtc_init);
module_exit(starfire_rtc_exit);
