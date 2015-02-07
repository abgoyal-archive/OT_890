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

/* sun_uflash.c - Driver for user-programmable flash on
 *                Sun Microsystems SME boardsets.
 *
 * This driver does NOT provide access to the OBP-flash for
 * safety reasons-- use <linux>/drivers/sbus/char/flash.c instead.
 *
 * Copyright (c) 2001 Eric Brower (ebrower@usa.net)
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <asm/prom.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>

#define UFLASH_OBPNAME	"flashprom"
#define DRIVER_NAME	"sun_uflash"
#define PFX		DRIVER_NAME ": "

#define UFLASH_WINDOW_SIZE	0x200000
#define UFLASH_BUSWIDTH		1			/* EBus is 8-bit */

MODULE_AUTHOR("Eric Brower <ebrower@usa.net>");
MODULE_DESCRIPTION("User-programmable flash device on Sun Microsystems boardsets");
MODULE_SUPPORTED_DEVICE(DRIVER_NAME);
MODULE_LICENSE("GPL");
MODULE_VERSION("2.1");

struct uflash_dev {
	const char		*name;	/* device name */
	struct map_info 	map;	/* mtd map info */
	struct mtd_info		*mtd;	/* mtd info */
};

struct map_info uflash_map_templ = {
	.name =		"SUNW,???-????",
	.size =		UFLASH_WINDOW_SIZE,
	.bankwidth =	UFLASH_BUSWIDTH,
};

int uflash_devinit(struct of_device *op, struct device_node *dp)
{
	struct uflash_dev *up;

	if (op->resource[1].flags) {
		/* Non-CFI userflash device-- once I find one we
		 * can work on supporting it.
		 */
		printk(KERN_ERR PFX "Unsupported device at %s, 0x%llx\n",
		       dp->full_name, (unsigned long long)op->resource[0].start);

		return -ENODEV;
	}

	up = kzalloc(sizeof(struct uflash_dev), GFP_KERNEL);
	if (!up) {
		printk(KERN_ERR PFX "Cannot allocate struct uflash_dev\n");
		return -ENOMEM;
	}

	/* copy defaults and tweak parameters */
	memcpy(&up->map, &uflash_map_templ, sizeof(uflash_map_templ));

	up->map.size = resource_size(&op->resource[0]);

	up->name = of_get_property(dp, "model", NULL);
	if (up->name && 0 < strlen(up->name))
		up->map.name = (char *)up->name;

	up->map.phys = op->resource[0].start;

	up->map.virt = of_ioremap(&op->resource[0], 0, up->map.size,
				  DRIVER_NAME);
	if (!up->map.virt) {
		printk(KERN_ERR PFX "Failed to map device.\n");
		kfree(up);

		return -EINVAL;
	}

	simple_map_init(&up->map);

	/* MTD registration */
	up->mtd = do_map_probe("cfi_probe", &up->map);
	if (!up->mtd) {
		of_iounmap(&op->resource[0], up->map.virt, up->map.size);
		kfree(up);

		return -ENXIO;
	}

	up->mtd->owner = THIS_MODULE;

	add_mtd_device(up->mtd);

	dev_set_drvdata(&op->dev, up);

	return 0;
}

static int __devinit uflash_probe(struct of_device *op, const struct of_device_id *match)
{
	struct device_node *dp = op->node;

	/* Flashprom must have the "user" property in order to
	 * be used by this driver.
	 */
	if (!of_find_property(dp, "user", NULL))
		return -ENODEV;

	return uflash_devinit(op, dp);
}

static int __devexit uflash_remove(struct of_device *op)
{
	struct uflash_dev *up = dev_get_drvdata(&op->dev);

	if (up->mtd) {
		del_mtd_device(up->mtd);
		map_destroy(up->mtd);
	}
	if (up->map.virt) {
		of_iounmap(&op->resource[0], up->map.virt, up->map.size);
		up->map.virt = NULL;
	}

	kfree(up);

	return 0;
}

static const struct of_device_id uflash_match[] = {
	{
		.name = UFLASH_OBPNAME,
	},
	{},
};

MODULE_DEVICE_TABLE(of, uflash_match);

static struct of_platform_driver uflash_driver = {
	.name		= DRIVER_NAME,
	.match_table	= uflash_match,
	.probe		= uflash_probe,
	.remove		= __devexit_p(uflash_remove),
};

static int __init uflash_init(void)
{
	return of_register_driver(&uflash_driver, &of_bus_type);
}

static void __exit uflash_exit(void)
{
	of_unregister_driver(&uflash_driver);
}

module_init(uflash_init);
module_exit(uflash_exit);
