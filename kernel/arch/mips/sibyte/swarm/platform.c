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

#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/ata_platform.h>

#include <asm/sibyte/board.h>
#include <asm/sibyte/sb1250_genbus.h>
#include <asm/sibyte/sb1250_regs.h>

#if defined(CONFIG_SIBYTE_SWARM) || defined(CONFIG_SIBYTE_LITTLESUR)

#define DRV_NAME	"pata-swarm"

#define SWARM_IDE_SHIFT	5
#define SWARM_IDE_BASE	0x1f0
#define SWARM_IDE_CTRL	0x3f6

static struct resource swarm_pata_resource[] = {
	{
		.name	= "Swarm GenBus IDE",
		.flags	= IORESOURCE_MEM,
	}, {
		.name	= "Swarm GenBus IDE",
		.flags	= IORESOURCE_MEM,
	}, {
		.name	= "Swarm GenBus IDE",
		.flags	= IORESOURCE_IRQ,
		.start	= K_INT_GB_IDE,
		.end	= K_INT_GB_IDE,
	},
};

static struct pata_platform_info pata_platform_data = {
	.ioport_shift	= SWARM_IDE_SHIFT,
};

static struct platform_device swarm_pata_device = {
	.name		= "pata_platform",
	.id		= -1,
	.resource	= swarm_pata_resource,
	.num_resources	= ARRAY_SIZE(swarm_pata_resource),
	.dev  = {
		.platform_data		= &pata_platform_data,
		.coherent_dma_mask	= ~0,	/* grumble */
	},
};

static int __init swarm_pata_init(void)
{
	u8 __iomem *base;
	phys_t offset, size;
	struct resource *r;

	if (!SIBYTE_HAVE_IDE)
		return -ENODEV;

	base = ioremap(A_IO_EXT_BASE, 0x800);
	offset = __raw_readq(base + R_IO_EXT_REG(R_IO_EXT_START_ADDR, IDE_CS));
	size = __raw_readq(base + R_IO_EXT_REG(R_IO_EXT_MULT_SIZE, IDE_CS));
	iounmap(base);

	offset = G_IO_START_ADDR(offset) << S_IO_ADDRBASE;
	size = (G_IO_MULT_SIZE(size) + 1) << S_IO_REGSIZE;
	if (offset < A_PHYS_GENBUS || offset >= A_PHYS_GENBUS_END) {
		pr_info(DRV_NAME ": PATA interface at GenBus disabled\n");

		return -EBUSY;
	}

	pr_info(DRV_NAME ": PATA interface at GenBus slot %i\n", IDE_CS);

	r = swarm_pata_resource;
	r[0].start = offset + (SWARM_IDE_BASE << SWARM_IDE_SHIFT);
	r[0].end   = offset + ((SWARM_IDE_BASE + 8) << SWARM_IDE_SHIFT) - 1;
	r[1].start = offset + (SWARM_IDE_CTRL << SWARM_IDE_SHIFT);
	r[1].end   = offset + ((SWARM_IDE_CTRL + 1) << SWARM_IDE_SHIFT) - 1;

	return platform_device_register(&swarm_pata_device);
}

device_initcall(swarm_pata_init);

#endif /* defined(CONFIG_SIBYTE_SWARM) || defined(CONFIG_SIBYTE_LITTLESUR) */
