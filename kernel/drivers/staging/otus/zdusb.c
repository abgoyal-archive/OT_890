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
 * Copyright (c) 2007-2008 Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/*                                                                      */
/*  Module Name : zdusb.c                                               */
/*                                                                      */
/*  Abstract                                                            */
/*     This module contains plug and play handling for USB device driver*/
/*                                                                      */
/*  NOTES                                                               */
/*     Platform dependent.                                              */
/*                                                                      */
/************************************************************************/

#ifdef MODVERSIONS
#include <linux/modversions.h>
#endif

#include <linux/module.h>
#include <linux/usb.h>

#include "usbdrv.h"
#include "zdusb.h"

int zfLnxAllocAllUrbs(struct usbdrv_private *macp);
void zfLnxFreeAllUrbs(struct usbdrv_private *macp);
void zfLnxUnlinkAllUrbs(struct usbdrv_private *macp);

MODULE_AUTHOR("Atheros Communications");
MODULE_DESCRIPTION("Atheros 802.11n Wireless LAN adapter");
MODULE_LICENSE("Dual BSD/GPL");

static const char driver_name[] = "Otus";

/* table of devices that work with this driver */
static struct usb_device_id zd1221_ids [] = {
	{ USB_DEVICE(VENDOR_ATHR, PRODUCT_AR9170) },
        { USB_DEVICE(VENDOR_DLINK, PRODUCT_DWA160A) },
	{ USB_DEVICE(0x0846, 0x9010) },
	{ }					/* Terminating entry */
};

MODULE_DEVICE_TABLE(usb, zd1221_ids);

extern u8_t zfLnxInitSetup(struct net_device *dev, struct usbdrv_private *macp);
extern int usbdrv_close(struct net_device *dev);
extern u8_t zfLnxClearStructs(struct net_device *dev);
extern int zfWdsClose(struct net_device *dev);
extern int zfUnregisterWdsDev(struct net_device* parentDev, u16_t wdsId);
extern int zfLnxVapClose(struct net_device *dev);
extern int zfLnxUnregisterVapDev(struct net_device* parentDev, u16_t vapId);

/* WDS */
extern struct zsWdsStruct wds[ZM_WDS_PORT_NUMBER];

/* VAP */
extern struct zsVapStruct vap[ZM_VAP_PORT_NUMBER];

static int zfLnxProbe(struct usb_interface *interface,
	const struct usb_device_id *id)
{
    struct usb_device *dev = interface_to_usbdev(interface);

    struct net_device *net = NULL;
    struct usbdrv_private *macp = NULL;
    int vendor_id, product_id;
    int result = 0;

    usb_get_dev(dev);

    vendor_id = dev->descriptor.idVendor;
    product_id = dev->descriptor.idProduct;

#ifdef HMAC_DEBUG
    printk(KERN_NOTICE "vendor_id = %04x\n", vendor_id);
    printk(KERN_NOTICE "product_id = %04x\n", product_id);

    if (dev->speed == USB_SPEED_HIGH)
        printk(KERN_NOTICE "USB 2.0 Host\n");
    else
        printk(KERN_NOTICE "USB 1.1 Host\n");
#endif

    if (!(macp = kmalloc(sizeof(struct usbdrv_private), GFP_KERNEL)))
    {
        printk(KERN_ERR "out of memory allocating device structure\n");
        result = -ENOMEM;
        goto fail;
    }

    /* Zero the memory */
    memset(macp, 0, sizeof(struct usbdrv_private));

    net = alloc_etherdev(0);

    if (net == NULL)
    {
        printk(KERN_ERR "zfLnxProbe: Not able to alloc etherdev struct\n");
        result = -ENOMEM;
        goto fail1;
    }

    strcpy(net->name, "ath%d");

    net->ml_priv = macp;   //kernel 2.6
    macp->udev = dev;
    macp->device = net;

    /* set up the endpoint information */
    /* check out the endpoints */
    macp->interface = interface;

    //init_waitqueue_head(&macp->regSet_wait);
    //init_waitqueue_head(&macp->iorwRsp_wait);
    //init_waitqueue_head(&macp->term_wait);

    if (!zfLnxAllocAllUrbs(macp))
    {
        result = -ENOMEM;
        goto fail2;
    }

    if (!zfLnxInitSetup(net, macp))
    {
        result = -EIO;
        goto fail3;
    }
    else
    {
        usb_set_intfdata(interface, macp);
        SET_NETDEV_DEV(net, &interface->dev);

        if (register_netdev(net) != 0)
        {
            usb_set_intfdata(interface, NULL);
            goto fail3;
        }
    }

    netif_carrier_off(net);
    goto done;

fail3:
    zfLnxFreeAllUrbs(macp);
fail2:
    free_netdev(net);  //kernel 2.6
fail1:
    kfree(macp);

fail:
    usb_put_dev(dev);
    macp = NULL;

done:
    return result;
}

static void zfLnxDisconnect(struct usb_interface *interface)
{
    struct usbdrv_private *macp = (struct usbdrv_private *) usb_get_intfdata(interface);

    printk(KERN_DEBUG "zfLnxDisconnect\n");

    if (!macp)
    {
        printk(KERN_ERR "unregistering non-existant device\n");
        return;
    }

    if (macp->driver_isolated)
    {
        if (macp->device->flags & IFF_UP)
            usbdrv_close(macp->device);
    }

#if 0
    /* Close WDS */
    //zfWdsClose(wds[0].dev);
    /* Unregister WDS */
    //zfUnregisterWdsDev(macp->device, 0);

    /* Close VAP */
    zfLnxVapClose(vap[0].dev);
    /* Unregister VAP */
    zfLnxUnregisterVapDev(macp->device, 0);
#endif

    zfLnxClearStructs(macp->device);

    unregister_netdev(macp->device);

    usb_put_dev(interface_to_usbdev(interface));

    //printk(KERN_ERR "3. zfLnxUnlinkAllUrbs\n");
    //zfLnxUnlinkAllUrbs(macp);

    /* Free network interface */
    free_netdev(macp->device);

    zfLnxFreeAllUrbs(macp);
    //zfLnxClearStructs(macp->device);
    kfree(macp);
    macp = NULL;

    usb_set_intfdata(interface, NULL);
}

static struct usb_driver zd1221_driver = {
	.name         = driver_name,
	.probe        = zfLnxProbe,
	.disconnect   = zfLnxDisconnect,
	.id_table     = zd1221_ids,
};

int __init zfLnxIinit(void)
{
    printk(KERN_NOTICE "%s - version %s\n",  DRIVER_NAME, VERSIONID);
    return usb_register(&zd1221_driver);
}

void __exit zfLnxExit(void)
{
    usb_deregister(&zd1221_driver);
}

module_init(zfLnxIinit);
module_exit(zfLnxExit);
