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
 * modified for SNI usage by Thomas Bogendoerfer
 *
 * This code is released under the GPL version 2.
 */

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/eisa.h>
#include <linux/init.h>

/* The default EISA device parent (virtual root device).
 * Now use a platform device, since that's the obvious choice. */

static struct platform_device eisa_root_dev = {
	.name = "eisa",
	.id   = 0,
};

static struct eisa_root_device eisa_bus_root = {
	.dev           = &eisa_root_dev.dev,
	.bus_base_addr = 0,
	.res	       = &ioport_resource,
	.slots	       = EISA_MAX_SLOTS,
	.dma_mask      = 0xffffffff,
	.force_probe   = 1,
};

int __init sni_eisa_root_init(void)
{
	int r;

	r = platform_device_register(&eisa_root_dev);
	if (!r)
		return r;

	eisa_root_dev.dev.driver_data = &eisa_bus_root;

	if (eisa_root_register(&eisa_bus_root)) {
		/* A real bridge may have been registered before
		 * us. So quietly unregister. */
		platform_device_unregister(&eisa_root_dev);
		return -1;
	}
	return 0;
}
