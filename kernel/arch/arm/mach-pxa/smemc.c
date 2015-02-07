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
 * Static Memory Controller
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/sysdev.h>

#include <mach/hardware.h>

#define SMEMC_PHYS_BASE	(0x4A000000)
#define SMEMC_PHYS_SIZE	(0x90)

#define MSC0		(0x08)	/* Static Memory Controller Register 0 */
#define MSC1		(0x0C)	/* Static Memory Controller Register 1 */
#define SXCNFG		(0x1C)	/* Synchronous Static Memory Control Register */
#define MEMCLKCFG	(0x68)	/* Clock Configuration */
#define CSADRCFG0	(0x80)	/* Address Configuration Register for CS0 */
#define CSADRCFG1	(0x84)	/* Address Configuration Register for CS1 */
#define CSADRCFG2	(0x88)	/* Address Configuration Register for CS2 */
#define CSADRCFG3	(0x8C)	/* Address Configuration Register for CS3 */

#ifdef CONFIG_PM
static void __iomem *smemc_mmio_base;

static unsigned long msc[2];
static unsigned long sxcnfg, memclkcfg;
static unsigned long csadrcfg[4];

static int pxa3xx_smemc_suspend(struct sys_device *dev, pm_message_t state)
{
	msc[0] = __raw_readl(smemc_mmio_base + MSC0);
	msc[1] = __raw_readl(smemc_mmio_base + MSC1);
	sxcnfg = __raw_readl(smemc_mmio_base + SXCNFG);
	memclkcfg = __raw_readl(smemc_mmio_base + MEMCLKCFG);
	csadrcfg[0] = __raw_readl(smemc_mmio_base + CSADRCFG0);
	csadrcfg[1] = __raw_readl(smemc_mmio_base + CSADRCFG1);
	csadrcfg[2] = __raw_readl(smemc_mmio_base + CSADRCFG2);
	csadrcfg[3] = __raw_readl(smemc_mmio_base + CSADRCFG3);

	return 0;
}

static int pxa3xx_smemc_resume(struct sys_device *dev)
{
	__raw_writel(msc[0], smemc_mmio_base + MSC0);
	__raw_writel(msc[1], smemc_mmio_base + MSC1);
	__raw_writel(sxcnfg, smemc_mmio_base + SXCNFG);
	__raw_writel(memclkcfg, smemc_mmio_base + MEMCLKCFG);
	__raw_writel(csadrcfg[0], smemc_mmio_base + CSADRCFG0);
	__raw_writel(csadrcfg[1], smemc_mmio_base + CSADRCFG1);
	__raw_writel(csadrcfg[2], smemc_mmio_base + CSADRCFG2);
	__raw_writel(csadrcfg[3], smemc_mmio_base + CSADRCFG3);

	return 0;
}

static struct sysdev_class smemc_sysclass = {
	.name		= "smemc",
	.suspend	= pxa3xx_smemc_suspend,
	.resume		= pxa3xx_smemc_resume,
};

static struct sys_device smemc_sysdev = {
	.id		= 0,
	.cls		= &smemc_sysclass,
};

static int __init smemc_init(void)
{
	int ret = 0;

	if (cpu_is_pxa3xx()) {
		smemc_mmio_base = ioremap(SMEMC_PHYS_BASE, SMEMC_PHYS_SIZE);
		if (smemc_mmio_base == NULL)
			return -ENODEV;

		ret = sysdev_class_register(&smemc_sysclass);
		if (ret)
			return ret;

		ret = sysdev_register(&smemc_sysdev);
	}

	return ret;
}
subsys_initcall(smemc_init);
#endif
