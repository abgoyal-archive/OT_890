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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2007 Aurelien Jarno <aurelien@aurel32.net>
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/serial.h>
#include <linux/serial_8250.h>
#include <linux/ssb/ssb.h>
#include <bcm47xx.h>

static struct plat_serial8250_port uart8250_data[5];

static struct platform_device uart8250_device = {
	.name			= "serial8250",
	.id			= PLAT8250_DEV_PLATFORM,
	.dev			= {
		.platform_data	= uart8250_data,
	},
};

static int __init uart8250_init(void)
{
	int i;
	struct ssb_mipscore *mcore = &(ssb_bcm47xx.mipscore);

	memset(&uart8250_data, 0,  sizeof(uart8250_data));

	for (i = 0; i < mcore->nr_serial_ports; i++) {
		struct plat_serial8250_port *p = &(uart8250_data[i]);
		struct ssb_serial_port *ssb_port = &(mcore->serial_ports[i]);

		p->mapbase = (unsigned int) ssb_port->regs;
		p->membase = (void *) ssb_port->regs;
		p->irq = ssb_port->irq + 2;
		p->uartclk = ssb_port->baud_base;
		p->regshift = ssb_port->reg_shift;
		p->iotype = UPIO_MEM;
		p->flags = UPF_BOOT_AUTOCONF | UPF_SHARE_IRQ;
	}
	return platform_device_register(&uart8250_device);
}

module_init(uart8250_init);

MODULE_AUTHOR("Aurelien Jarno <aurelien@aurel32.net>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("8250 UART probe driver for the BCM47XX platforms");
