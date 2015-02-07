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
 * UIO SMX Cryptengine driver.
 *
 * (C) 2008 Nias Digital P/L <bn@niasdigital.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/device.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/io.h>

#define DRV_NAME "smx-ce"
#define DRV_VERSION "0.03"

#define SMX_CSR  0x00000000
#define SMX_EnD  0x00000001
#define SMX_RUN  0x00000002
#define SMX_DRDY 0x00000004
#define SMX_ERR  0x00000008

static irqreturn_t smx_handler(int irq, struct uio_info *dev_info)
{
	void __iomem *csr = dev_info->mem[0].internal_addr + SMX_CSR;

	u32 status = ioread32(csr);

	if (!(status & SMX_DRDY))
		return IRQ_NONE;

	/* Disable interrupt */
	iowrite32(status & ~SMX_DRDY, csr);
	return IRQ_HANDLED;
}

static int __devinit smx_ce_probe(struct platform_device *dev)
{

	int ret = -ENODEV;
	struct uio_info *info;
	struct resource *regs;

	info = kzalloc(sizeof(struct uio_info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	regs = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (!regs) {
		dev_err(&dev->dev, "No memory resource specified\n");
		goto out_free;
	}

	info->mem[0].addr = regs->start;
	if (!info->mem[0].addr) {
		dev_err(&dev->dev, "Invalid memory resource\n");
		goto out_free;
	}

	info->mem[0].size = regs->end - regs->start + 1;
	info->mem[0].internal_addr = ioremap(regs->start, info->mem[0].size);

	if (!info->mem[0].internal_addr) {
		dev_err(&dev->dev, "Can't remap memory address range\n");
		goto out_free;
	}

	info->mem[0].memtype = UIO_MEM_PHYS;

	info->name = "smx-ce";
	info->version = "0.03";

	info->irq = platform_get_irq(dev, 0);
	if (info->irq < 0) {
		ret = info->irq;
		dev_err(&dev->dev, "No (or invalid) IRQ resource specified\n");
		goto out_unmap;
	}

	info->irq_flags = IRQF_SHARED;
	info->handler = smx_handler;

	platform_set_drvdata(dev, info);

	ret = uio_register_device(&dev->dev, info);

	if (ret)
		goto out_unmap;

	return 0;

out_unmap:
	iounmap(info->mem[0].internal_addr);
out_free:
	kfree(info);

	return ret;
}

static int __devexit smx_ce_remove(struct platform_device *dev)
{
	struct uio_info *info = platform_get_drvdata(dev);

	uio_unregister_device(info);
	platform_set_drvdata(dev, NULL);
	iounmap(info->mem[0].internal_addr);

	kfree(info);

	return 0;
}

static struct platform_driver smx_ce_driver = {
	.probe		= smx_ce_probe,
	.remove		= __devexit_p(smx_ce_remove),
	.driver		= {
		.name	= DRV_NAME,
		.owner	= THIS_MODULE,
	},
};

static int __init smx_ce_init_module(void)
{
	return platform_driver_register(&smx_ce_driver);
}
module_init(smx_ce_init_module);

static void __exit smx_ce_exit_module(void)
{
	platform_driver_unregister(&smx_ce_driver);
}
module_exit(smx_ce_exit_module);

MODULE_LICENSE("GPL v2");
MODULE_VERSION(DRV_VERSION);
MODULE_AUTHOR("Ben Nizette <bn@niasdigital.com>");
