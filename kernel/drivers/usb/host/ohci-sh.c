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
 * Copyright (C) 2008 Renesas Solutions Corp.
 *
 * Author : Yoshihiro Shimoda <shimoda.yoshihiro@renesas.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <linux/platform_device.h>

static int ohci_sh_start(struct usb_hcd *hcd)
{
	struct ohci_hcd	*ohci = hcd_to_ohci(hcd);

	ohci_hcd_init(ohci);
	ohci_init(ohci);
	ohci_run(ohci);
	hcd->state = HC_STATE_RUNNING;
	return 0;
}

static const struct hc_driver ohci_sh_hc_driver = {
	.description =		hcd_name,
	.product_desc =		"SuperH OHCI",
	.hcd_priv_size =	sizeof(struct ohci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq =			ohci_irq,
	.flags =		HCD_USB11 | HCD_MEMORY,

	/*
	 * basic lifecycle operations
	 */
	.start =		ohci_sh_start,
	.stop =			ohci_stop,
	.shutdown =		ohci_shutdown,

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

#define resource_len(r) (((r)->end - (r)->start) + 1)
static int ohci_hcd_sh_probe(struct platform_device *pdev)
{
	struct resource *res = NULL;
	struct usb_hcd *hcd = NULL;
	int irq = -1;
	int ret;

	if (usb_disabled())
		return -ENODEV;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		err("platform_get_resource error.");
		return -ENODEV;
	}

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		err("platform_get_irq error.");
		return -ENODEV;
	}

	/* initialize hcd */
	hcd = usb_create_hcd(&ohci_sh_hc_driver, &pdev->dev, (char *)hcd_name);
	if (!hcd) {
		err("Failed to create hcd");
		return -ENOMEM;
	}

	hcd->regs = (void __iomem *)res->start;
	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_len(res);
	ret = usb_add_hcd(hcd, irq, IRQF_DISABLED);
	if (ret != 0) {
		err("Failed to add hcd");
		usb_put_hcd(hcd);
		return ret;
	}

	return ret;
}

static int ohci_hcd_sh_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);

	usb_remove_hcd(hcd);
	usb_put_hcd(hcd);

	return 0;
}

static struct platform_driver ohci_hcd_sh_driver = {
	.probe		= ohci_hcd_sh_probe,
	.remove		= ohci_hcd_sh_remove,
	.shutdown	= usb_hcd_platform_shutdown,
	.driver		= {
		.name	= "sh_ohci",
		.owner	= THIS_MODULE,
	},
};

MODULE_ALIAS("platform:sh_ohci");
