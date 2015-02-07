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

#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/slab.h>
#include <linux/of_device.h>

#include <asm/errno.h>
#include <asm/dcr.h>

static void of_device_make_bus_id(struct of_device *dev)
{
	static atomic_t bus_no_reg_magic;
	struct device_node *node = dev->node;
	const u32 *reg;
	u64 addr;
	int magic;

	/*
	 * If it's a DCR based device, use 'd' for native DCRs
	 * and 'D' for MMIO DCRs.
	 */
#ifdef CONFIG_PPC_DCR
	reg = of_get_property(node, "dcr-reg", NULL);
	if (reg) {
#ifdef CONFIG_PPC_DCR_NATIVE
		dev_set_name(&dev->dev, "d%x.%s", *reg, node->name);
#else /* CONFIG_PPC_DCR_NATIVE */
		addr = of_translate_dcr_address(node, *reg, NULL);
		if (addr != OF_BAD_ADDR) {
			dev_set_name(&dev->dev, "D%llx.%s",
				     (unsigned long long)addr, node->name);
			return;
		}
#endif /* !CONFIG_PPC_DCR_NATIVE */
	}
#endif /* CONFIG_PPC_DCR */

	/*
	 * For MMIO, get the physical address
	 */
	reg = of_get_property(node, "reg", NULL);
	if (reg) {
		addr = of_translate_address(node, reg);
		if (addr != OF_BAD_ADDR) {
			dev_set_name(&dev->dev, "%llx.%s",
				     (unsigned long long)addr, node->name);
			return;
		}
	}

	/*
	 * No BusID, use the node name and add a globally incremented
	 * counter (and pray...)
	 */
	magic = atomic_add_return(1, &bus_no_reg_magic);
	dev_set_name(&dev->dev, "%s.%d", node->name, magic - 1);
}

struct of_device *of_device_alloc(struct device_node *np,
				  const char *bus_id,
				  struct device *parent)
{
	struct of_device *dev;

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return NULL;

	dev->node = of_node_get(np);
	dev->dev.dma_mask = &dev->dma_mask;
	dev->dev.parent = parent;
	dev->dev.release = of_release_dev;
	dev->dev.archdata.of_node = np;

	if (bus_id)
		dev_set_name(&dev->dev, bus_id);
	else
		of_device_make_bus_id(dev);

	return dev;
}
EXPORT_SYMBOL(of_device_alloc);

int of_device_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	struct of_device *ofdev;
	const char *compat;
	int seen = 0, cplen, sl;

	if (!dev)
		return -ENODEV;

	ofdev = to_of_device(dev);

	if (add_uevent_var(env, "OF_NAME=%s", ofdev->node->name))
		return -ENOMEM;

	if (add_uevent_var(env, "OF_TYPE=%s", ofdev->node->type))
		return -ENOMEM;

        /* Since the compatible field can contain pretty much anything
         * it's not really legal to split it out with commas. We split it
         * up using a number of environment variables instead. */

	compat = of_get_property(ofdev->node, "compatible", &cplen);
	while (compat && *compat && cplen > 0) {
		if (add_uevent_var(env, "OF_COMPATIBLE_%d=%s", seen, compat))
			return -ENOMEM;

		sl = strlen (compat) + 1;
		compat += sl;
		cplen -= sl;
		seen++;
	}

	if (add_uevent_var(env, "OF_COMPATIBLE_N=%d", seen))
		return -ENOMEM;

	/* modalias is trickier, we add it in 2 steps */
	if (add_uevent_var(env, "MODALIAS="))
		return -ENOMEM;
	sl = of_device_get_modalias(ofdev, &env->buf[env->buflen-1],
				    sizeof(env->buf) - env->buflen);
	if (sl >= (sizeof(env->buf) - env->buflen))
		return -ENOMEM;
	env->buflen += sl;

	return 0;
}
EXPORT_SYMBOL(of_device_uevent);
EXPORT_SYMBOL(of_device_get_modalias);
