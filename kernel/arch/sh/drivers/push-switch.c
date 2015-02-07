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
 * Generic push-switch framework
 *
 * Copyright (C) 2006  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <asm/push-switch.h>

#define DRV_NAME "push-switch"
#define DRV_VERSION "0.1.1"

static ssize_t switch_show(struct device *dev,
			   struct device_attribute *attr,
			   char *buf)
{
	struct push_switch_platform_info *psw_info = dev->platform_data;
	return sprintf(buf, "%s\n", psw_info->name);
}
static DEVICE_ATTR(switch, S_IRUGO, switch_show, NULL);

static void switch_timer(unsigned long data)
{
	struct push_switch *psw = (struct push_switch *)data;

	schedule_work(&psw->work);
}

static void switch_work_handler(struct work_struct *work)
{
	struct push_switch *psw = container_of(work, struct push_switch, work);
	struct platform_device *pdev = psw->pdev;

	psw->state = 0;

	kobject_uevent(&pdev->dev.kobj, KOBJ_CHANGE);
}

static int switch_drv_probe(struct platform_device *pdev)
{
	struct push_switch_platform_info *psw_info;
	struct push_switch *psw;
	int ret, irq;

	psw = kzalloc(sizeof(struct push_switch), GFP_KERNEL);
	if (unlikely(!psw))
		return -ENOMEM;

	irq = platform_get_irq(pdev, 0);
	if (unlikely(irq < 0)) {
		ret = -ENODEV;
		goto err;
	}

	psw_info = pdev->dev.platform_data;
	BUG_ON(!psw_info);

	ret = request_irq(irq, psw_info->irq_handler,
			  IRQF_DISABLED | psw_info->irq_flags,
			  psw_info->name ? psw_info->name : DRV_NAME, pdev);
	if (unlikely(ret < 0))
		goto err;

	if (psw_info->name) {
		ret = device_create_file(&pdev->dev, &dev_attr_switch);
		if (unlikely(ret)) {
			dev_err(&pdev->dev, "Failed creating device attrs\n");
			ret = -EINVAL;
			goto err_irq;
		}
	}

	INIT_WORK(&psw->work, switch_work_handler);
	init_timer(&psw->debounce);

	psw->debounce.function = switch_timer;
	psw->debounce.data = (unsigned long)psw;

	/* Workqueue API brain-damage */
	psw->pdev = pdev;

	platform_set_drvdata(pdev, psw);

	return 0;

err_irq:
	free_irq(irq, pdev);
err:
	kfree(psw);
	return ret;
}

static int switch_drv_remove(struct platform_device *pdev)
{
	struct push_switch *psw = platform_get_drvdata(pdev);
	struct push_switch_platform_info *psw_info = pdev->dev.platform_data;
	int irq = platform_get_irq(pdev, 0);

	if (psw_info->name)
		device_remove_file(&pdev->dev, &dev_attr_switch);

	platform_set_drvdata(pdev, NULL);
	flush_scheduled_work();
	del_timer_sync(&psw->debounce);
	free_irq(irq, pdev);

	kfree(psw);

	return 0;
}

static struct platform_driver switch_driver = {
	.probe		= switch_drv_probe,
	.remove		= switch_drv_remove,
	.driver		= {
		.name	= DRV_NAME,
	},
};

static int __init switch_init(void)
{
	printk(KERN_NOTICE DRV_NAME ": version %s loaded\n", DRV_VERSION);
	return platform_driver_register(&switch_driver);
}

static void __exit switch_exit(void)
{
	platform_driver_unregister(&switch_driver);
}
module_init(switch_init);
module_exit(switch_exit);

MODULE_VERSION(DRV_VERSION);
MODULE_AUTHOR("Paul Mundt");
MODULE_LICENSE("GPL v2");
