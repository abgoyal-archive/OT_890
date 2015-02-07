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
 * SA1111 Bus Glue
 *
 * Written by Christopher Hoover <ch@hpl.hp.com>
 * Based on fragments of previous driver by Russell King et al.
 *
 * This file is licenced under the GPL.
 */

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <mach/assabet.h>
#include <mach/badge4.h>
#include <asm/hardware/sa1111.h>

#ifndef CONFIG_SA1111
#error "This file is SA-1111 bus glue.  CONFIG_SA1111 must be defined."
#endif

extern int usb_disabled(void);

/*-------------------------------------------------------------------------*/

static void sa1111_start_hc(struct sa1111_dev *dev)
{
	unsigned int usb_rst = 0;

	printk(KERN_DEBUG __FILE__
	       ": starting SA-1111 OHCI USB Controller\n");

#ifdef CONFIG_SA1100_BADGE4
	if (machine_is_badge4()) {
		badge4_set_5V(BADGE4_5V_USB, 1);
	}
#endif

	if (machine_is_xp860() ||
	    machine_has_neponset() ||
	    machine_is_pfs168() ||
	    machine_is_badge4())
		usb_rst = USB_RESET_PWRSENSELOW | USB_RESET_PWRCTRLLOW;

	/*
	 * Configure the power sense and control lines.  Place the USB
	 * host controller in reset.
	 */
	sa1111_writel(usb_rst | USB_RESET_FORCEIFRESET | USB_RESET_FORCEHCRESET,
		      dev->mapbase + SA1111_USB_RESET);

	/*
	 * Now, carefully enable the USB clock, and take
	 * the USB host controller out of reset.
	 */
	sa1111_enable_device(dev);
	udelay(11);
	sa1111_writel(usb_rst, dev->mapbase + SA1111_USB_RESET);
}

static void sa1111_stop_hc(struct sa1111_dev *dev)
{
	unsigned int usb_rst;
	printk(KERN_DEBUG __FILE__
	       ": stopping SA-1111 OHCI USB Controller\n");

	/*
	 * Put the USB host controller into reset.
	 */
	usb_rst = sa1111_readl(dev->mapbase + SA1111_USB_RESET);
	sa1111_writel(usb_rst | USB_RESET_FORCEIFRESET | USB_RESET_FORCEHCRESET,
		      dev->mapbase + SA1111_USB_RESET);

	/*
	 * Stop the USB clock.
	 */
	sa1111_disable_device(dev);

#ifdef CONFIG_SA1100_BADGE4
	if (machine_is_badge4()) {
		/* Disable power to the USB bus */
		badge4_set_5V(BADGE4_5V_USB, 0);
	}
#endif
}


/*-------------------------------------------------------------------------*/

#if 0
static void dump_hci_status(struct usb_hcd *hcd, const char *label)
{
	unsigned long status = sa1111_readl(hcd->regs + SA1111_USB_STATUS);

	dbg ("%s USB_STATUS = { %s%s%s%s%s}", label,
	     ((status & USB_STATUS_IRQHCIRMTWKUP) ? "IRQHCIRMTWKUP " : ""),
	     ((status & USB_STATUS_IRQHCIBUFFACC) ? "IRQHCIBUFFACC " : ""),
	     ((status & USB_STATUS_NIRQHCIM) ? "" : "IRQHCIM "),
	     ((status & USB_STATUS_NHCIMFCLR) ? "" : "HCIMFCLR "),
	     ((status & USB_STATUS_USBPWRSENSE) ? "USBPWRSENSE " : ""));
}
#endif

/*-------------------------------------------------------------------------*/

/* configure so an HC device and id are always provided */
/* always called with process context; sleeping is OK */


/**
 * usb_hcd_sa1111_probe - initialize SA-1111-based HCDs
 * Context: !in_interrupt()
 *
 * Allocates basic resources for this USB host controller, and
 * then invokes the start() method for the HCD associated with it
 * through the hotplug entry's driver_data.
 *
 * Store this function in the HCD's struct pci_driver as probe().
 */
int usb_hcd_sa1111_probe (const struct hc_driver *driver,
			  struct sa1111_dev *dev)
{
	struct usb_hcd *hcd;
	int retval;

	hcd = usb_create_hcd (driver, &dev->dev, "sa1111");
	if (!hcd)
		return -ENOMEM;
	hcd->rsrc_start = dev->res.start;
	hcd->rsrc_len = dev->res.end - dev->res.start + 1;

	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
		dbg("request_mem_region failed");
		retval = -EBUSY;
		goto err1;
	}
	hcd->regs = dev->mapbase;

	sa1111_start_hc(dev);
	ohci_hcd_init(hcd_to_ohci(hcd));

	retval = usb_add_hcd(hcd, dev->irq[1], IRQF_DISABLED);
	if (retval == 0)
		return retval;

	sa1111_stop_hc(dev);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
 err1:
	usb_put_hcd(hcd);
	return retval;
}


/* may be called without controller electrically present */
/* may be called with controller, bus, and devices active */

/**
 * usb_hcd_sa1111_remove - shutdown processing for SA-1111-based HCDs
 * @dev: USB Host Controller being removed
 * Context: !in_interrupt()
 *
 * Reverses the effect of usb_hcd_sa1111_probe(), first invoking
 * the HCD's stop() method.  It is always called from a thread
 * context, normally "rmmod", "apmd", or something similar.
 *
 */
void usb_hcd_sa1111_remove (struct usb_hcd *hcd, struct sa1111_dev *dev)
{
	usb_remove_hcd(hcd);
	sa1111_stop_hc(dev);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd(hcd);
}

/*-------------------------------------------------------------------------*/

static int __devinit
ohci_sa1111_start (struct usb_hcd *hcd)
{
	struct ohci_hcd	*ohci = hcd_to_ohci (hcd);
	int		ret;

	if ((ret = ohci_init(ohci)) < 0)
		return ret;

	if ((ret = ohci_run (ohci)) < 0) {
		err ("can't start %s", hcd->self.bus_name);
		ohci_stop (hcd);
		return ret;
	}
	return 0;
}

/*-------------------------------------------------------------------------*/

static const struct hc_driver ohci_sa1111_hc_driver = {
	.description =		hcd_name,
	.product_desc =		"SA-1111 OHCI",
	.hcd_priv_size =	sizeof(struct ohci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq =			ohci_irq,
	.flags =		HCD_USB11 | HCD_MEMORY,

	/*
	 * basic lifecycle operations
	 */
	.start =		ohci_sa1111_start,
	.stop =			ohci_stop,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue =		ohci_urb_enqueue,
	.urb_dequeue =		ohci_urb_dequeue,
	.endpoint_disable =	ohci_endpoint_disable,

	/*
	 * scheduling support
	 */
	.get_frame_number =	ohci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data =	ohci_hub_status_data,
	.hub_control =		ohci_hub_control,
#ifdef	CONFIG_PM
	.bus_suspend =		ohci_bus_suspend,
	.bus_resume =		ohci_bus_resume,
#endif
	.start_port_reset =	ohci_start_port_reset,
};

/*-------------------------------------------------------------------------*/

static int ohci_hcd_sa1111_drv_probe(struct sa1111_dev *dev)
{
	int ret;

	if (usb_disabled())
		return -ENODEV;

	ret = usb_hcd_sa1111_probe(&ohci_sa1111_hc_driver, dev);
	return ret;
}

static int ohci_hcd_sa1111_drv_remove(struct sa1111_dev *dev)
{
	struct usb_hcd *hcd = sa1111_get_drvdata(dev);

	usb_hcd_sa1111_remove(hcd, dev);
	return 0;
}

static struct sa1111_driver ohci_hcd_sa1111_driver = {
	.drv = {
		.name	= "sa1111-ohci",
	},
	.devid		= SA1111_DEVID_USB,
	.probe		= ohci_hcd_sa1111_drv_probe,
	.remove		= ohci_hcd_sa1111_drv_remove,
};

