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
 *  NEC VR4100 series RTC platform device.
 *
 *  Copyright (C) 2007  Yoichi Yuasa <yoichi_yuasa@tripeaks.co.jp>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>

#include <asm/cpu.h>
#include <asm/vr41xx/irq.h>

static struct resource rtc_type1_resource[] __initdata = {
	{
		.start	= 0x0b0000c0,
		.end	= 0x0b0000df,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= 0x0b0001c0,
		.end	= 0x0b0001df,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= ELAPSEDTIME_IRQ,
		.end	= ELAPSEDTIME_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= RTCLONG1_IRQ,
		.end	= RTCLONG1_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct resource rtc_type2_resource[] __initdata = {
	{
		.start	= 0x0f000100,
		.end	= 0x0f00011f,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= 0x0f000120,
		.end	= 0x0f00013f,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= ELAPSEDTIME_IRQ,
		.end	= ELAPSEDTIME_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= RTCLONG1_IRQ,
		.end	= RTCLONG1_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

static int __init vr41xx_rtc_add(void)
{
	struct platform_device *pdev;
	struct resource *res;
	unsigned int num;
	int retval;

	pdev = platform_device_alloc("RTC", -1);
	if (!pdev)
		return -ENOMEM;

	switch (current_cpu_type()) {
	case CPU_VR4111:
	case CPU_VR4121:
		res = rtc_type1_resource;
		num = ARRAY_SIZE(rtc_type1_resource);
		break;
	case CPU_VR4122:
	case CPU_VR4131:
	case CPU_VR4133:
		res = rtc_type2_resource;
		num = ARRAY_SIZE(rtc_type2_resource);
		break;
	default:
		retval = -ENODEV;
		goto err_free_device;
	}

	retval = platform_device_add_resources(pdev, res, num);
	if (retval)
		goto err_free_device;

	retval = platform_device_add(pdev);
	if (retval)
		goto err_free_device;

	return 0;

err_free_device:
	platform_device_put(pdev);

	return retval;
}
device_initcall(vr41xx_rtc_add);
