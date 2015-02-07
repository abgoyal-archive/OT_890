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
 * arch/arm/mach-ns9xxx/plat-serial8250.c
 *
 * Copyright (C) 2008 by Digi International Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */
#include <linux/platform_device.h>
#include <linux/serial_8250.h>

#include <mach/regs-board-a9m9750dev.h>
#include <mach/board.h>

#define DRIVER_NAME "serial8250"

static int __init ns9xxx_plat_serial8250_init(void)
{
	struct plat_serial8250_port *pdata;
	struct platform_device *pdev;
	int ret = -ENOMEM;
	int i;

	if (!board_is_a9m9750dev())
		return -ENODEV;

	pdev = platform_device_alloc(DRIVER_NAME, 0);
	if (!pdev)
		goto err;

	pdata = kzalloc(5 * sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		goto err;

	pdev->dev.platform_data = pdata;

	pdata[0].iobase = FPGA_UARTA_BASE;
	pdata[1].iobase = FPGA_UARTB_BASE;
	pdata[2].iobase = FPGA_UARTC_BASE;
	pdata[3].iobase = FPGA_UARTD_BASE;

	for (i = 0; i < 4; ++i) {
		pdata[i].membase = (void __iomem *)pdata[i].iobase;
		pdata[i].mapbase = pdata[i].iobase;
		pdata[i].iotype = UPIO_MEM;
		pdata[i].uartclk = 18432000;
		pdata[i].flags = UPF_BOOT_AUTOCONF | UPF_SHARE_IRQ;
	}

	pdata[0].irq = IRQ_FPGA_UARTA;
	pdata[1].irq = IRQ_FPGA_UARTB;
	pdata[2].irq = IRQ_FPGA_UARTC;
	pdata[3].irq = IRQ_FPGA_UARTD;

	ret = platform_device_add(pdev);
	if (ret) {
err:
		platform_device_put(pdev);

		printk(KERN_WARNING "Could not add %s (errno=%d)\n",
				DRIVER_NAME, ret);
	}

	return 0;
}

arch_initcall(ns9xxx_plat_serial8250_init);
