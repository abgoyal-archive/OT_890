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
 * SH7770 Setup
 *
 *  Copyright (C) 2006 - 2008  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/serial.h>
#include <linux/serial_sci.h>

static struct plat_sci_port sci_platform_data[] = {
	{
		.mapbase	= 0xff923000,
		.flags		= UPF_BOOT_AUTOCONF,
		.type		= PORT_SCIF,
		.irqs		= { 61, 61, 61, 61 },
	}, {
		.mapbase	= 0xff924000,
		.flags		= UPF_BOOT_AUTOCONF,
		.type		= PORT_SCIF,
		.irqs		= { 62, 62, 62, 62 },
	}, {
		.mapbase	= 0xff925000,
		.flags		= UPF_BOOT_AUTOCONF,
		.type		= PORT_SCIF,
		.irqs		= { 63, 63, 63, 63 },
	}, {
		.mapbase	= 0xff926000,
		.flags		= UPF_BOOT_AUTOCONF,
		.type		= PORT_SCIF,
		.irqs		= { 64, 64, 64, 64 },
	}, {
		.mapbase	= 0xff927000,
		.flags		= UPF_BOOT_AUTOCONF,
		.type		= PORT_SCIF,
		.irqs		= { 65, 65, 65, 65 },
	}, {
		.mapbase	= 0xff928000,
		.flags		= UPF_BOOT_AUTOCONF,
		.type		= PORT_SCIF,
		.irqs		= { 66, 66, 66, 66 },
	}, {
		.mapbase	= 0xff929000,
		.flags		= UPF_BOOT_AUTOCONF,
		.type		= PORT_SCIF,
		.irqs		= { 67, 67, 67, 67 },
	}, {
		.mapbase	= 0xff92a000,
		.flags		= UPF_BOOT_AUTOCONF,
		.type		= PORT_SCIF,
		.irqs		= { 68, 68, 68, 68 },
	}, {
		.mapbase	= 0xff92b000,
		.flags		= UPF_BOOT_AUTOCONF,
		.type		= PORT_SCIF,
		.irqs		= { 69, 69, 69, 69 },
	}, {
		.mapbase	= 0xff92c000,
		.flags		= UPF_BOOT_AUTOCONF,
		.type		= PORT_SCIF,
		.irqs		= { 70, 70, 70, 70 },
	}, {
		.flags = 0,
	}
};

static struct platform_device sci_device = {
	.name		= "sh-sci",
	.id		= -1,
	.dev		= {
		.platform_data	= sci_platform_data,
	},
};

static struct platform_device *sh7770_devices[] __initdata = {
	&sci_device,
};

static int __init sh7770_devices_setup(void)
{
	return platform_add_devices(sh7770_devices,
				    ARRAY_SIZE(sh7770_devices));
}
__initcall(sh7770_devices_setup);

void __init plat_irq_setup(void)
{
}
