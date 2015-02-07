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
 *  drivers/switch/switch_gpio.c
 *
 * Copyright (C) 2008 Google, Inc.
 * Author: Mike Lockwood <lockwood@android.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/switch.h>
#include <linux/workqueue.h>
#include <linux/gpio.h>

struct gpio_switch_data {
	struct switch_dev sdev;
	unsigned gpio;
	const char *name_on;
	const char *name_off;
	const char *state_on;
	const char *state_off;
	int irq;
	struct work_struct work;
};

static void gpio_switch_work(struct work_struct *work)
{
	int state;
	struct gpio_switch_data	*data =
		container_of(work, struct gpio_switch_data, work);

	state = gpio_get_value(data->gpio);
	switch_set_state(&data->sdev, state);
}

static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
	struct gpio_switch_data *switch_data =
	    (struct gpio_switch_data *)dev_id;

	schedule_work(&switch_data->work);
	return IRQ_HANDLED;
}

static ssize_t switch_gpio_print_state(struct switch_dev *sdev, char *buf)
{
	struct gpio_switch_data	*switch_data =
		container_of(sdev, struct gpio_switch_data, sdev);
	const char *state;
	if (switch_get_state(sdev))
		state = switch_data->state_on;
	else
		state = switch_data->state_off;

	if (state)
		return sprintf(buf, "%s\n", state);
	return -1;
}

static int gpio_switch_probe(struct platform_device *pdev)
{
	struct gpio_switch_platform_data *pdata = pdev->dev.platform_data;
	struct gpio_switch_data *switch_data;
	int ret = 0;

	if (!pdata)
		return -EBUSY;

	switch_data = kzalloc(sizeof(struct gpio_switch_data), GFP_KERNEL);
	if (!switch_data)
		return -ENOMEM;

	switch_data->sdev.name = pdata->name;
	switch_data->gpio = pdata->gpio;
	switch_data->name_on = pdata->name_on;
	switch_data->name_off = pdata->name_off;
	switch_data->state_on = pdata->state_on;
	switch_data->state_off = pdata->state_off;
	switch_data->sdev.print_state = switch_gpio_print_state;

    ret = switch_dev_register(&switch_data->sdev);
	if (ret < 0)
		goto err_switch_dev_register;

	ret = gpio_request(switch_data->gpio, pdev->name);
	if (ret < 0)
		goto err_request_gpio;

	ret = gpio_direction_input(switch_data->gpio);
	if (ret < 0)
		goto err_set_gpio_input;

	INIT_WORK(&switch_data->work, gpio_switch_work);

	switch_data->irq = gpio_to_irq(switch_data->gpio);
	if (switch_data->irq < 0) {
		ret = switch_data->irq;
		goto err_detect_irq_num_failed;
	}

	ret = request_irq(switch_data->irq, gpio_irq_handler,
			  IRQF_TRIGGER_LOW, pdev->name, switch_data);
	if (ret < 0)
		goto err_request_irq;

	/* Perform initial detection */
	gpio_switch_work(&switch_data->work);

	return 0;

err_request_irq:
err_detect_irq_num_failed:
err_set_gpio_input:
	gpio_free(switch_data->gpio);
err_request_gpio:
    switch_dev_unregister(&switch_data->sdev);
err_switch_dev_register:
	kfree(switch_data);

	return ret;
}

static int __devexit gpio_switch_remove(struct platform_device *pdev)
{
	struct gpio_switch_data *switch_data = platform_get_drvdata(pdev);

	cancel_work_sync(&switch_data->work);
	gpio_free(switch_data->gpio);
    switch_dev_unregister(&switch_data->sdev);
	kfree(switch_data);

	return 0;
}

static struct platform_driver gpio_switch_driver = {
	.probe		= gpio_switch_probe,
	.remove		= __devexit_p(gpio_switch_remove),
	.driver		= {
		.name	= "switch-gpio",
		.owner	= THIS_MODULE,
	},
};

static int __init gpio_switch_init(void)
{
	return platform_driver_register(&gpio_switch_driver);
}

static void __exit gpio_switch_exit(void)
{
	platform_driver_unregister(&gpio_switch_driver);
}

module_init(gpio_switch_init);
module_exit(gpio_switch_exit);

MODULE_AUTHOR("Mike Lockwood <lockwood@android.com>");
MODULE_DESCRIPTION("GPIO Switch driver");
MODULE_LICENSE("GPL");
