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
 * arch/arm/mach-ixp23xx/espresso.c
 *
 * Double Espresso-specific routines
 *
 * Author: Lennert Buytenhek <buytenh@wantstofly.org>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/serial.h>
#include <linux/tty.h>
#include <linux/bitops.h>
#include <linux/ioport.h>
#include <linux/serial_8250.h>
#include <linux/serial_core.h>
#include <linux/device.h>
#include <linux/mm.h>
#include <linux/pci.h>
#include <linux/mtd/physmap.h>

#include <asm/types.h>
#include <asm/setup.h>
#include <asm/memory.h>
#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/tlbflush.h>
#include <asm/pgtable.h>

#include <asm/mach/map.h>
#include <asm/mach/irq.h>
#include <asm/mach/arch.h>
#include <asm/mach/pci.h>

static int __init espresso_pci_init(void)
{
	if (machine_is_espresso())
		ixp23xx_pci_slave_init();

	return 0;
};
subsys_initcall(espresso_pci_init);

static struct physmap_flash_data espresso_flash_data = {
	.width		= 2,
};

static struct resource espresso_flash_resource = {
	.start		= 0x90000000,
	.end		= 0x91ffffff,
	.flags		= IORESOURCE_MEM,
};

static struct platform_device espresso_flash = {
	.name		= "physmap-flash",
	.id		= 0,
	.dev		= {
		.platform_data	= &espresso_flash_data,
	},
	.num_resources	= 1,
	.resource	= &espresso_flash_resource,
};

static void __init espresso_init(void)
{
	platform_device_register(&espresso_flash);

	/*
	 * Mark flash as writeable.
	 */
	IXP23XX_EXP_CS0[0] |= IXP23XX_FLASH_WRITABLE;
	IXP23XX_EXP_CS0[1] |= IXP23XX_FLASH_WRITABLE;

	ixp23xx_sys_init();
}

MACHINE_START(ESPRESSO, "IP Fabrics Double Espresso")
	/* Maintainer: Lennert Buytenhek */
	.phys_io	= IXP23XX_PERIPHERAL_PHYS,
	.io_pg_offst	= ((IXP23XX_PERIPHERAL_VIRT >> 18)) & 0xfffc,
	.map_io		= ixp23xx_map_io,
	.init_irq	= ixp23xx_init_irq,
	.timer		= &ixp23xx_timer,
	.boot_params	= 0x00000100,
	.init_machine	= espresso_init,
MACHINE_END
