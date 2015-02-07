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
 * sni_82596.c -- driver for intel 82596 ethernet controller, as
 *  		  used in older SNI RM machines
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/irq.h>

#define SNI_82596_DRIVER_VERSION "SNI RM 82596 driver - Revision: 0.01"

static const char sni_82596_string[] = "snirm_82596";

#define DMA_ALLOC                      dma_alloc_coherent
#define DMA_FREE                       dma_free_coherent
#define DMA_WBACK(priv, addr, len)     do { } while (0)
#define DMA_INV(priv, addr, len)       do { } while (0)
#define DMA_WBACK_INV(priv, addr, len) do { } while (0)

#define SYSBUS      0x00004400

/* big endian CPU, 82596 little endian */
#define SWAP32(x)   cpu_to_le32((u32)(x))
#define SWAP16(x)   cpu_to_le16((u16)(x))

#define OPT_MPU_16BIT    0x01

#include "lib82596.c"

MODULE_AUTHOR("Thomas Bogendoerfer");
MODULE_DESCRIPTION("i82596 driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:snirm_82596");
module_param(i596_debug, int, 0);
MODULE_PARM_DESC(i596_debug, "82596 debug mask");

static inline void ca(struct net_device *dev)
{
	struct i596_private *lp = netdev_priv(dev);

	writel(0, lp->ca);
}


static void mpu_port(struct net_device *dev, int c, dma_addr_t x)
{
	struct i596_private *lp = netdev_priv(dev);

	u32 v = (u32) (c) | (u32) (x);

	if (lp->options & OPT_MPU_16BIT) {
		writew(v & 0xffff, lp->mpu_port);
		wmb();  /* order writes to MPU port */
		udelay(1);
		writew(v >> 16, lp->mpu_port);
	} else {
		writel(v, lp->mpu_port);
		wmb();  /* order writes to MPU port */
		udelay(1);
		writel(v, lp->mpu_port);
	}
}


static int __devinit sni_82596_probe(struct platform_device *dev)
{
	struct	net_device *netdevice;
	struct i596_private *lp;
	struct  resource *res, *ca, *idprom, *options;
	int	retval = -ENOMEM;
	void __iomem *mpu_addr;
	void __iomem *ca_addr;
	u8 __iomem *eth_addr;

	res = platform_get_resource(dev, IORESOURCE_MEM, 0);
	ca = platform_get_resource(dev, IORESOURCE_MEM, 1);
	options = platform_get_resource(dev, 0, 0);
	idprom = platform_get_resource(dev, IORESOURCE_MEM, 2);
	if (!res || !ca || !options || !idprom)
		return -ENODEV;
	mpu_addr = ioremap_nocache(res->start, 4);
	if (!mpu_addr)
		return -ENOMEM;
	ca_addr = ioremap_nocache(ca->start, 4);
	if (!ca_addr)
		goto probe_failed_free_mpu;

	printk(KERN_INFO "Found i82596 at 0x%x\n", res->start);

	netdevice = alloc_etherdev(sizeof(struct i596_private));
	if (!netdevice)
		goto probe_failed_free_ca;

	SET_NETDEV_DEV(netdevice, &dev->dev);
	platform_set_drvdata (dev, netdevice);

	netdevice->base_addr = res->start;
	netdevice->irq = platform_get_irq(dev, 0);

	eth_addr = ioremap_nocache(idprom->start, 0x10);
	if (!eth_addr)
		goto probe_failed;

	/* someone seems to like messed up stuff */
	netdevice->dev_addr[0] = readb(eth_addr + 0x0b);
	netdevice->dev_addr[1] = readb(eth_addr + 0x0a);
	netdevice->dev_addr[2] = readb(eth_addr + 0x09);
	netdevice->dev_addr[3] = readb(eth_addr + 0x08);
	netdevice->dev_addr[4] = readb(eth_addr + 0x07);
	netdevice->dev_addr[5] = readb(eth_addr + 0x06);
	iounmap(eth_addr);

	if (!netdevice->irq) {
		printk(KERN_ERR "%s: IRQ not found for i82596 at 0x%lx\n",
			__FILE__, netdevice->base_addr);
		goto probe_failed;
	}

	lp = netdev_priv(netdevice);
	lp->options = options->flags & IORESOURCE_BITS;
	lp->ca = ca_addr;
	lp->mpu_port = mpu_addr;

	retval = i82596_probe(netdevice);
	if (retval == 0)
		return 0;

probe_failed:
	free_netdev(netdevice);
probe_failed_free_ca:
	iounmap(ca_addr);
probe_failed_free_mpu:
	iounmap(mpu_addr);
	return retval;
}

static int __devexit sni_82596_driver_remove(struct platform_device *pdev)
{
	struct net_device *dev = platform_get_drvdata(pdev);
	struct i596_private *lp = netdev_priv(dev);

	unregister_netdev(dev);
	DMA_FREE(dev->dev.parent, sizeof(struct i596_private),
		 lp->dma, lp->dma_addr);
	iounmap(lp->ca);
	iounmap(lp->mpu_port);
	free_netdev (dev);
	return 0;
}

static struct platform_driver sni_82596_driver = {
	.probe	= sni_82596_probe,
	.remove	= __devexit_p(sni_82596_driver_remove),
	.driver	= {
		.name	= sni_82596_string,
		.owner	= THIS_MODULE,
	},
};

static int __devinit sni_82596_init(void)
{
	printk(KERN_INFO SNI_82596_DRIVER_VERSION "\n");
	return platform_driver_register(&sni_82596_driver);
}


static void __exit sni_82596_exit(void)
{
	platform_driver_unregister(&sni_82596_driver);
}

module_init(sni_82596_init);
module_exit(sni_82596_exit);
