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

/* orinoco_pci.h
 * 
 * Common code for all Orinoco drivers for PCI devices, including
 * both native PCI and PCMCIA-to-PCI bridges.
 *
 * Copyright (C) 2005, Pavel Roskin.
 * See orinoco.c for license.
 */

#ifndef _ORINOCO_PCI_H
#define _ORINOCO_PCI_H

#include <linux/netdevice.h>

/* Driver specific data */
struct orinoco_pci_card {
	void __iomem *bridge_io;
	void __iomem *attr_io;
};

#ifdef CONFIG_PM
static int orinoco_pci_suspend(struct pci_dev *pdev, pm_message_t state)
{
	struct net_device *dev = pci_get_drvdata(pdev);
	struct orinoco_private *priv = netdev_priv(dev);
	unsigned long flags;
	int err;

	err = orinoco_lock(priv, &flags);
	if (err) {
		printk(KERN_ERR "%s: cannot lock hardware for suspend\n",
		       dev->name);
		return err;
	}

	err = __orinoco_down(dev);
	if (err)
		printk(KERN_WARNING "%s: error %d bringing interface down "
		       "for suspend\n", dev->name, err);
	
	netif_device_detach(dev);

	priv->hw_unavailable++;
	
	orinoco_unlock(priv, &flags);

	free_irq(pdev->irq, dev);
	pci_save_state(pdev);
	pci_disable_device(pdev);
	pci_set_power_state(pdev, PCI_D3hot);

	return 0;
}

static int orinoco_pci_resume(struct pci_dev *pdev)
{
	struct net_device *dev = pci_get_drvdata(pdev);
	struct orinoco_private *priv = netdev_priv(dev);
	unsigned long flags;
	int err;

	pci_set_power_state(pdev, 0);
	err = pci_enable_device(pdev);
	if (err) {
		printk(KERN_ERR "%s: pci_enable_device failed on resume\n",
		       dev->name);
		return err;
	}
	pci_restore_state(pdev);

	err = request_irq(pdev->irq, orinoco_interrupt, IRQF_SHARED,
			  dev->name, dev);
	if (err) {
		printk(KERN_ERR "%s: cannot re-allocate IRQ on resume\n",
		       dev->name);
		pci_disable_device(pdev);
		return -EBUSY;
	}

	err = orinoco_reinit_firmware(dev);
	if (err) {
		printk(KERN_ERR "%s: error %d re-initializing firmware "
		       "on resume\n", dev->name, err);
		return err;
	}

	spin_lock_irqsave(&priv->lock, flags);

	netif_device_attach(dev);

	priv->hw_unavailable--;

	if (priv->open && (! priv->hw_unavailable)) {
		err = __orinoco_up(dev);
		if (err)
			printk(KERN_ERR "%s: Error %d restarting card on resume\n",
			       dev->name, err);
	}
	
	spin_unlock_irqrestore(&priv->lock, flags);

	return 0;
}
#else
#define orinoco_pci_suspend NULL
#define orinoco_pci_resume NULL
#endif

#endif /* _ORINOCO_PCI_H */
