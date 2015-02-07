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
 * drivers/uio/uio_pdrv.c
 *
 * Copyright (C) 2008 by Digi International Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */
#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/stringify.h>

#define DRIVER_NAME "uio_pdrv"

struct uio_platdata {
	struct uio_info *uioinfo;
};

static int uio_pdrv_probe(struct platform_device *pdev)
{
	struct uio_info *uioinfo = pdev->dev.platform_data;
	struct uio_platdata *pdata;
	struct uio_mem *uiomem;
	int ret = -ENODEV;
	int i;

	if (!uioinfo || !uioinfo->name || !uioinfo->version) {
		dev_dbg(&pdev->dev, "%s: err_uioinfo\n", __func__);
		goto err_uioinfo;
	}

	pdata = kzalloc(sizeof(*pdata), GFP_KERNEL);
	if (!pdata) {
		ret = -ENOMEM;
		dev_dbg(&pdev->dev, "%s: err_alloc_pdata\n", __func__);
		goto err_alloc_pdata;
	}

	pdata->uioinfo = uioinfo;

	uiomem = &uioinfo->mem[0];

	for (i = 0; i < pdev->num_resources; ++i) {
		struct resource *r = &pdev->resource[i];

		if (r->flags != IORESOURCE_MEM)
			continue;

		if (uiomem >= &uioinfo->mem[MAX_UIO_MAPS]) {
			dev_warn(&pdev->dev, "device has more than "
					__stringify(MAX_UIO_MAPS)
					" I/O memory resources.\n");
			break;
		}

		uiomem->memtype = UIO_MEM_PHYS;
		uiomem->addr = r->start;
		uiomem->size = r->end - r->start + 1;
		++uiomem;
	}

	while (uiomem < &uioinfo->mem[MAX_UIO_MAPS]) {
		uiomem->size = 0;
		++uiomem;
	}

	pdata->uioinfo->priv = pdata;

	ret = uio_register_device(&pdev->dev, pdata->uioinfo);

	if (ret) {
		kfree(pdata);
err_alloc_pdata:
err_uioinfo:
		return ret;
	}

	platform_set_drvdata(pdev, pdata);

	return 0;
}

static int uio_pdrv_remove(struct platform_device *pdev)
{
	struct uio_platdata *pdata = platform_get_drvdata(pdev);

	uio_unregister_device(pdata->uioinfo);

	kfree(pdata);

	return 0;
}

static struct platform_driver uio_pdrv = {
	.probe = uio_pdrv_probe,
	.remove = uio_pdrv_remove,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	},
};

static int __init uio_pdrv_init(void)
{
	return platform_driver_register(&uio_pdrv);
}

static void __exit uio_pdrv_exit(void)
{
	platform_driver_unregister(&uio_pdrv);
}
module_init(uio_pdrv_init);
module_exit(uio_pdrv_exit);

MODULE_AUTHOR("Uwe Kleine-Koenig");
MODULE_DESCRIPTION("Userspace I/O platform driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:" DRIVER_NAME);
