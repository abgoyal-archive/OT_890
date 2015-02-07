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

#ifndef _I8042_SPARCIO_H
#define _I8042_SPARCIO_H

#include <linux/of_device.h>

#include <asm/io.h>
#include <asm/oplib.h>
#include <asm/prom.h>

static int i8042_kbd_irq = -1;
static int i8042_aux_irq = -1;
#define I8042_KBD_IRQ i8042_kbd_irq
#define I8042_AUX_IRQ i8042_aux_irq

#define I8042_KBD_PHYS_DESC "sparcps2/serio0"
#define I8042_AUX_PHYS_DESC "sparcps2/serio1"
#define I8042_MUX_PHYS_DESC "sparcps2/serio%d"

static void __iomem *kbd_iobase;
static struct resource *kbd_res;

#define I8042_COMMAND_REG	(kbd_iobase + 0x64UL)
#define I8042_DATA_REG		(kbd_iobase + 0x60UL)

static inline int i8042_read_data(void)
{
	return readb(kbd_iobase + 0x60UL);
}

static inline int i8042_read_status(void)
{
	return readb(kbd_iobase + 0x64UL);
}

static inline void i8042_write_data(int val)
{
	writeb(val, kbd_iobase + 0x60UL);
}

static inline void i8042_write_command(int val)
{
	writeb(val, kbd_iobase + 0x64UL);
}

#ifdef CONFIG_PCI

#define OBP_PS2KBD_NAME1	"kb_ps2"
#define OBP_PS2KBD_NAME2	"keyboard"
#define OBP_PS2MS_NAME1		"kdmouse"
#define OBP_PS2MS_NAME2		"mouse"

static int __devinit sparc_i8042_probe(struct of_device *op, const struct of_device_id *match)
{
	struct device_node *dp = op->node;

	dp = dp->child;
	while (dp) {
		if (!strcmp(dp->name, OBP_PS2KBD_NAME1) ||
		    !strcmp(dp->name, OBP_PS2KBD_NAME2)) {
			struct of_device *kbd = of_find_device_by_node(dp);
			unsigned int irq = kbd->irqs[0];
			if (irq == 0xffffffff)
				irq = op->irqs[0];
			i8042_kbd_irq = irq;
			kbd_iobase = of_ioremap(&kbd->resource[0],
						0, 8, "kbd");
			kbd_res = &kbd->resource[0];
		} else if (!strcmp(dp->name, OBP_PS2MS_NAME1) ||
			   !strcmp(dp->name, OBP_PS2MS_NAME2)) {
			struct of_device *ms = of_find_device_by_node(dp);
			unsigned int irq = ms->irqs[0];
			if (irq == 0xffffffff)
				irq = op->irqs[0];
			i8042_aux_irq = irq;
		}

		dp = dp->sibling;
	}

	return 0;
}

static int __devexit sparc_i8042_remove(struct of_device *op)
{
	of_iounmap(kbd_res, kbd_iobase, 8);

	return 0;
}

static const struct of_device_id sparc_i8042_match[] = {
	{
		.name = "8042",
	},
	{},
};
MODULE_DEVICE_TABLE(of, sparc_i8042_match);

static struct of_platform_driver sparc_i8042_driver = {
	.name		= "i8042",
	.match_table	= sparc_i8042_match,
	.probe		= sparc_i8042_probe,
	.remove		= __devexit_p(sparc_i8042_remove),
};

static int __init i8042_platform_init(void)
{
	struct device_node *root = of_find_node_by_path("/");

	if (!strcmp(root->name, "SUNW,JavaStation-1")) {
		/* Hardcoded values for MrCoffee.  */
		i8042_kbd_irq = i8042_aux_irq = 13 | 0x20;
		kbd_iobase = ioremap(0x71300060, 8);
		if (!kbd_iobase)
			return -ENODEV;
	} else {
		int err = of_register_driver(&sparc_i8042_driver,
					     &of_bus_type);
		if (err)
			return err;

		if (i8042_kbd_irq == -1 ||
		    i8042_aux_irq == -1) {
			if (kbd_iobase) {
				of_iounmap(kbd_res, kbd_iobase, 8);
				kbd_iobase = (void __iomem *) NULL;
			}
			return -ENODEV;
		}
	}

	i8042_reset = 1;

	return 0;
}

static inline void i8042_platform_exit(void)
{
	struct device_node *root = of_find_node_by_path("/");

	if (strcmp(root->name, "SUNW,JavaStation-1"))
		of_unregister_driver(&sparc_i8042_driver);
}

#else /* !CONFIG_PCI */
static int __init i8042_platform_init(void)
{
	return -ENODEV;
}

static inline void i8042_platform_exit(void)
{
}
#endif /* !CONFIG_PCI */

#endif /* _I8042_SPARCIO_H */
