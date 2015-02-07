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
 * Sonics Silicon Backplane
 * PCI Hostdevice wrapper
 *
 * Copyright (c) 2005 Martin Langer <martin-langer@gmx.de>
 * Copyright (c) 2005 Stefano Brivio <st3@riseup.net>
 * Copyright (c) 2005 Danny van Dyk <kugelfang@gentoo.org>
 * Copyright (c) 2005 Andreas Jaggi <andreas.jaggi@waterwave.ch>
 * Copyright (c) 2005-2007 Michael Buesch <mbuesch@freenet.de>
 *
 * Licensed under the GNU/GPL. See COPYING for details.
 */

#include <linux/pci.h>
#include <linux/ssb/ssb.h>


#ifdef CONFIG_PM
static int ssb_pcihost_suspend(struct pci_dev *dev, pm_message_t state)
{
	struct ssb_bus *ssb = pci_get_drvdata(dev);
	int err;

	err = ssb_bus_suspend(ssb);
	if (err)
		return err;
	pci_save_state(dev);
	pci_disable_device(dev);
	pci_set_power_state(dev, pci_choose_state(dev, state));

	return 0;
}

static int ssb_pcihost_resume(struct pci_dev *dev)
{
	struct ssb_bus *ssb = pci_get_drvdata(dev);
	int err;

	pci_set_power_state(dev, 0);
	err = pci_enable_device(dev);
	if (err)
		return err;
	pci_restore_state(dev);
	err = ssb_bus_resume(ssb);
	if (err)
		return err;

	return 0;
}
#else /* CONFIG_PM */
# define ssb_pcihost_suspend	NULL
# define ssb_pcihost_resume	NULL
#endif /* CONFIG_PM */

static int ssb_pcihost_probe(struct pci_dev *dev,
			     const struct pci_device_id *id)
{
	struct ssb_bus *ssb;
	int err = -ENOMEM;
	const char *name;

	ssb = kzalloc(sizeof(*ssb), GFP_KERNEL);
	if (!ssb)
		goto out;
	err = pci_enable_device(dev);
	if (err)
		goto err_kfree_ssb;
	name = dev_name(&dev->dev);
	if (dev->driver && dev->driver->name)
		name = dev->driver->name;
	err = pci_request_regions(dev, name);
	if (err)
		goto err_pci_disable;
	pci_set_master(dev);

	err = ssb_bus_pcibus_register(ssb, dev);
	if (err)
		goto err_pci_release_regions;

	pci_set_drvdata(dev, ssb);

out:
	return err;

err_pci_release_regions:
	pci_release_regions(dev);
err_pci_disable:
	pci_disable_device(dev);
err_kfree_ssb:
	kfree(ssb);
	return err;
}

static void ssb_pcihost_remove(struct pci_dev *dev)
{
	struct ssb_bus *ssb = pci_get_drvdata(dev);

	ssb_bus_unregister(ssb);
	pci_release_regions(dev);
	pci_disable_device(dev);
	kfree(ssb);
	pci_set_drvdata(dev, NULL);
}

int ssb_pcihost_register(struct pci_driver *driver)
{
	driver->probe = ssb_pcihost_probe;
	driver->remove = ssb_pcihost_remove;
	driver->suspend = ssb_pcihost_suspend;
	driver->resume = ssb_pcihost_resume;

	return pci_register_driver(driver);
}
EXPORT_SYMBOL(ssb_pcihost_register);
