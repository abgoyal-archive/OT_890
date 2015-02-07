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

/* -*- mode: c; c-basic-offset: 8 -*- */

/* SNI RM driver
 *
 * Copyright (C) 2001 by James.Bottomley@HansenPartnership.com
**-----------------------------------------------------------------------------
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
**-----------------------------------------------------------------------------
 */

/*
 * Based on lasi700.c
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/mm.h>
#include <linux/blkdev.h>
#include <linux/sched.h>
#include <linux/ioport.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>

#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/irq.h>
#include <asm/delay.h>

#include <scsi/scsi_host.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_transport.h>
#include <scsi/scsi_transport_spi.h>

#include "53c700.h"

MODULE_AUTHOR("Thomas Bogendörfer");
MODULE_DESCRIPTION("SNI RM 53c710 SCSI Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:snirm_53c710");

#define SNIRM710_CLOCK	32

static struct scsi_host_template snirm710_template = {
	.name		= "SNI RM SCSI 53c710",
	.proc_name	= "snirm_53c710",
	.this_id	= 7,
	.module		= THIS_MODULE,
};

static int __init snirm710_probe(struct platform_device *dev)
{
	unsigned long base;
	struct NCR_700_Host_Parameters *hostdata;
	struct Scsi_Host *host;
	struct  resource *res;

	res = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENODEV;

	base = res->start;
	hostdata = kzalloc(sizeof(*hostdata), GFP_KERNEL);
	if (!hostdata) {
		dev_printk(KERN_ERR, dev, "Failed to allocate host data\n");
		return -ENOMEM;
	}

	hostdata->dev = &dev->dev;
	dma_set_mask(&dev->dev, DMA_32BIT_MASK);
	hostdata->base = ioremap_nocache(base, 0x100);
	hostdata->differential = 0;

	hostdata->clock = SNIRM710_CLOCK;
	hostdata->force_le_on_be = 1;
	hostdata->chip710 = 1;
	hostdata->burst_length = 4;

	host = NCR_700_detect(&snirm710_template, hostdata, &dev->dev);
	if (!host)
		goto out_kfree;
	host->this_id = 7;
	host->base = base;
	host->irq = platform_get_irq(dev, 0);
	if(request_irq(host->irq, NCR_700_intr, IRQF_SHARED, "snirm710", host)) {
		printk(KERN_ERR "snirm710: request_irq failed!\n");
		goto out_put_host;
	}

	dev_set_drvdata(&dev->dev, host);
	scsi_scan_host(host);

	return 0;

 out_put_host:
	scsi_host_put(host);
 out_kfree:
	iounmap(hostdata->base);
	kfree(hostdata);
	return -ENODEV;
}

static int __exit snirm710_driver_remove(struct platform_device *dev)
{
	struct Scsi_Host *host = dev_get_drvdata(&dev->dev);
	struct NCR_700_Host_Parameters *hostdata =
		(struct NCR_700_Host_Parameters *)host->hostdata[0];

	scsi_remove_host(host);
	NCR_700_release(host);
	free_irq(host->irq, host);
	iounmap(hostdata->base);
	kfree(hostdata);

	return 0;
}

static struct platform_driver snirm710_driver = {
	.probe	= snirm710_probe,
	.remove	= __devexit_p(snirm710_driver_remove),
	.driver	= {
		.name	= "snirm_53c710",
		.owner	= THIS_MODULE,
	},
};

static int __init snirm710_init(void)
{
	return platform_driver_register(&snirm710_driver);
}

static void __exit snirm710_exit(void)
{
	platform_driver_unregister(&snirm710_driver);
}

module_init(snirm710_init);
module_exit(snirm710_exit);
