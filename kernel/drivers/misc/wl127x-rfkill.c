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
 * Bluetooth TI wl127x rfkill power control via GPIO
 *
 * Copyright (C) 2009 Motorola, Inc.
 * Copyright (C) 2008 Texas Instruments
 * Initial code: Pavan Savoy <pavan.savoy@gmail.com> (wl127x_power.c)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/rfkill.h>
#include <linux/platform_device.h>
#include <linux/wl127x-rfkill.h>

static int wl127x_rfkill_set_power(void *data, enum rfkill_state state)
{
	int nshutdown_gpio = (int) data;

	switch (state) {
	case RFKILL_STATE_UNBLOCKED:
		gpio_set_value(nshutdown_gpio, 1);
		break;
	case RFKILL_STATE_SOFT_BLOCKED:
		gpio_set_value(nshutdown_gpio, 0);
		break;
	default:
		printk(KERN_ERR "invalid bluetooth rfkill state %d\n", state);
	}
	return 0;
}

static int wl127x_rfkill_probe(struct platform_device *pdev)
{
	int rc = 0;
	struct wl127x_rfkill_platform_data *pdata = pdev->dev.platform_data;
	enum rfkill_state default_state = RFKILL_STATE_SOFT_BLOCKED;  /* off */

	rc = gpio_request(pdata->nshutdown_gpio, "wl127x_nshutdown_gpio");
	if (unlikely(rc))
		return rc;

	rc = gpio_direction_output(pdata->nshutdown_gpio, 0);
	if (unlikely(rc))
		return rc;

	rfkill_set_default(RFKILL_TYPE_BLUETOOTH, default_state);
	wl127x_rfkill_set_power(NULL, default_state);

	pdata->rfkill = rfkill_allocate(&pdev->dev, RFKILL_TYPE_BLUETOOTH);
	if (unlikely(!pdata->rfkill))
		return -ENOMEM;

	pdata->rfkill->name = "wl127x";
	pdata->rfkill->state = default_state;
	/* userspace cannot take exclusive control */
	pdata->rfkill->user_claim_unsupported = 1;
	pdata->rfkill->user_claim = 0;
	pdata->rfkill->data = (void *) pdata->nshutdown_gpio;
	pdata->rfkill->toggle_radio = wl127x_rfkill_set_power;

	rc = rfkill_register(pdata->rfkill);

	if (unlikely(rc))
		rfkill_free(pdata->rfkill);

	return 0;
}

static int wl127x_rfkill_remove(struct platform_device *pdev)
{
	struct wl127x_rfkill_platform_data *pdata = pdev->dev.platform_data;

	rfkill_unregister(pdata->rfkill);
	rfkill_free(pdata->rfkill);
	gpio_free(pdata->nshutdown_gpio);

	return 0;
}

static struct platform_driver wl127x_rfkill_platform_driver = {
	.probe = wl127x_rfkill_probe,
	.remove = wl127x_rfkill_remove,
	.driver = {
		   .name = "wl127x-rfkill",
		   .owner = THIS_MODULE,
		   },
};

static int __init wl127x_rfkill_init(void)
{
	return platform_driver_register(&wl127x_rfkill_platform_driver);
}

static void __exit wl127x_rfkill_exit(void)
{
	platform_driver_unregister(&wl127x_rfkill_platform_driver);
}

module_init(wl127x_rfkill_init);
module_exit(wl127x_rfkill_exit);

MODULE_ALIAS("platform:wl127x");
MODULE_DESCRIPTION("wl127x-rfkill");
MODULE_AUTHOR("Motorola");
MODULE_LICENSE("GPL");
