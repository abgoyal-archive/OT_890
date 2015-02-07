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
 * Virtual EISA root driver.
 * Acts as a placeholder if we don't have a proper EISA bridge.
 *
 * (C) 2003 Marc Zyngier <maz@wild-wind.fr.eu.org>
 *
 * This code is released under the GPL version 2.
 */

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/eisa.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#if defined(CONFIG_ALPHA_JENSEN) || defined(CONFIG_EISA_VLB_PRIMING)
#define EISA_FORCE_PROBE_DEFAULT 1
#else
#define EISA_FORCE_PROBE_DEFAULT 0
#endif

static int force_probe = EISA_FORCE_PROBE_DEFAULT;
static void virtual_eisa_release (struct device *);

/* The default EISA device parent (virtual root device).
 * Now use a platform device, since that's the obvious choice. */

static struct platform_device eisa_root_dev = {
	.name = "eisa",
	.id   = 0,
	.dev  = {
		.release = virtual_eisa_release,
	},
};

static struct eisa_root_device eisa_bus_root = {
	.dev           = &eisa_root_dev.dev,
	.bus_base_addr = 0,
	.res	       = &ioport_resource,
	.slots	       = EISA_MAX_SLOTS,
	.dma_mask      = 0xffffffff,
};

static void virtual_eisa_release (struct device *dev)
{
	/* nothing really to do here */
}

static int __init virtual_eisa_root_init (void)
{
	int r;
	
        if ((r = platform_device_register (&eisa_root_dev))) {
                return r;
        }

	eisa_bus_root.force_probe = force_probe;
	
	eisa_root_dev.dev.driver_data = &eisa_bus_root;

	if (eisa_root_register (&eisa_bus_root)) {
		/* A real bridge may have been registered before
		 * us. So quietly unregister. */
		platform_device_unregister (&eisa_root_dev);
		return -1;
	}

	return 0;
}

module_param (force_probe, int, 0444);

device_initcall (virtual_eisa_root_init);
