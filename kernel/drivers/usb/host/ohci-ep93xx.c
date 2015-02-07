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
 * OHCI HCD (Host Controller Driver) for USB.
 *
 * (C) Copyright 1999 Roman Weissgaerber <weissg@vienna.at>
 * (C) Copyright 2000-2002 David Brownell <dbrownell@users.sourceforge.net>
 * (C) Copyright 2002 Hewlett-Packard Company
 *
 * Bus Glue for ep93xx.
 *
 * Written by Christopher Hoover <ch@hpl.hp.com>
 * Based on fragments of previous driver by Russell King et al.
 *
 * Modified for LH7A404 from ohci-sa1111.c
 *  by Durgesh Pattamatta <pattamattad@sharpsec.com>
 *
 * Modified for pxa27x from ohci-lh7a404.c
 *  by Nick Bane <nick@cecomputing.co.uk> 26-8-2004
 *
 * Modified for ep93xx from ohci-pxa27x.c
 *  by Lennert Buytenhek <buytenh@wantstofly.org> 28-2-2006
 *  Based on an earlier driver by Ray Lehtiniemi
 *
 * This file is licenced under the GPL.
 */

#include <linux/clk.h>
#include <linux/device.h>
#include <linux/signal.h>
#include <linux/platform_device.h>

#include <mach/hardware.h>

static struct clk *usb_host_clock;

static void ep93xx_start_hc(struct device *dev)
{
	clk_enable(usb_host_clock);
}

static void ep93xx_stop_hc(struct device *dev)
{
	clk_disable(usb_host_clock);
}

static int usb_hcd_ep93xx_probe(const struct hc_driver *driver,
			 struct platform_device *pdev)
{
	int retval;
	struct usb_hcd *hcd;

	if (pdev->resource[1].flags != IORESOURCE_IRQ) {
		pr_debug("resource[1] is not IORESOURCE_IRQ");
		return -ENOMEM;
	}

	hcd = usb_create_hcd(driver, &pdev->dev, "ep93xx");
	if (hcd == NULL)
		return -ENOMEM;

	hcd->rsrc_start = pdev->resource[0].start;
	hcd->rsrc_len = pdev->resource[0].end - pdev->resource[0].start + 1;
	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
		usb_put_hcd(hcd);
		retval = -EBUSY;
		goto err1;
	}

	hcd->regs = ioremap(hcd->rsrc_start, hcd->rsrc_len);
	if (hcd->regs == NULL) {
		pr_debug("ioremap failed");
		retval = -ENOMEM;
		goto err2;
	}

	usb_host_clock = clk_get(&pdev->dev, "usb_host");
	ep93xx_start_hc(&pdev->dev);

	ohci_hcd_init(hcd_to_ohci(hcd));

	retval = usb_add_hcd(hcd, pdev->resource[1].start, IRQF_DISABLED);
	if (retval == 0)
		return retval;

	ep93xx_stop_hc(&pdev->dev);
	iounmap(hcd->regs);
err2:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
err1:
	usb_put_hcd(hcd);

	return retval;
}

static void usb_hcd_ep93xx_remove(struct usb_hcd *hcd,
			struct platform_device *pdev)
{
	usb_remove_hcd(hcd);
	ep93xx_stop_hc(&pdev->dev);
	clk_put(usb_host_clock);
	iounmap(hcd->regs);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd(hcd);
}

static int __devinit ohci_ep93xx_start(struct usb_hcd *hcd)
{
	struct ohci_hcd *ohci = hcd_to_ohci(hcd);
	int ret;

	if ((ret = ohci_init(ohci)) < 0)
		return ret;

	if ((ret = ohci_run(ohci)) < 0) {
		err("can't start %s", hcd->self.bus_name);
		ohci_stop(hcd);
		return ret;
	}

	return 0;
}

static struct hc_driver ohci_ep93xx_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "EP93xx OHCI",
	.hcd_priv_size		= sizeof(struct ohci_hcd),
	.irq			= ohci_irq,
	.flags			= HCD_USB11 | HCD_MEMORY,
	.start			= ohci_ep93xx_start,
	.stop			= ohci_stop,
	.shutdown		= ohci_shutdown,
	.urb_enqueue		= ohci_urb_enqueue,
	.urb_dequeue		= ohci_urb_dequeue,
	.endpoint_disable	= ohci_endpoint_disable,
	.get_frame_number	= ohci_get_frame,
	.hub_status_data	= ohci_hub_status_data,
	.hub_control		= ohci_hub_control,
#ifdef CONFIG_PM
	.bus_suspend		= ohci_bus_suspend,
	.bus_resume		= ohci_bus_resume,
#endif
	.start_port_reset	= ohci_start_port_reset,
};

extern int usb_disabled(void);

static int ohci_hcd_ep93xx_drv_probe(struct platform_device *pdev)
{
	int ret;

	ret = -ENODEV;
	if (!usb_disabled())
		ret = usb_hcd_ep93xx_probe(&ohci_ep93xx_hc_driver, pdev);

	return ret;
}

static int ohci_hcd_ep93xx_drv_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);

	usb_hcd_ep93xx_remove(hcd, pdev);

	return 0;
}

#ifdef CONFIG_PM
static int ohci_hcd_ep93xx_drv_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct ohci_hcd *ohci = hcd_to_ohci(hcd);

	if (time_before(jiffies, ohci->next_statechange))
		msleep(5);
	ohci->next_statechange = jiffies;

	ep93xx_stop_hc(&pdev->dev);
	hcd->state = HC_STATE_SUSPENDED;

	return 0;
}

static int ohci_hcd_ep93xx_drv_resume(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct ohci_hcd *ohci = hcd_to_ohci(hcd);
	int status;

	if (time_before(jiffies, ohci->next_statechange))
		msleep(5);
	ohci->next_statechange = jiffies;

	ep93xx_start_hc(&pdev->dev);

	ohci_finish_controller_resume(hcd);
	return 0;
}
#endif


static struct platform_driver ohci_hcd_ep93xx_driver = {
	.probe		= ohci_hcd_ep93xx_drv_probe,
	.remove		= ohci_hcd_ep93xx_drv_remove,
	.shutdown	= usb_hcd_platform_shutdown,
#ifdef CONFIG_PM
	.suspend	= ohci_hcd_ep93xx_drv_suspend,
	.resume		= ohci_hcd_ep93xx_drv_resume,
#endif
	.driver		= {
		.name	= "ep93xx-ohci",
		.owner	= THIS_MODULE,
	},
};

MODULE_ALIAS("platform:ep93xx-ohci");
