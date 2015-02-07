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
 * Setup pointers to hardware-dependent routines.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 1997, 1998, 2001, 07, 08 by Ralf Baechle
 * Copyright (C) 2001 MIPS Technologies, Inc.
 * Copyright (C) 2007 by Thomas Bogendoerfer
 */
#include <linux/eisa.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/console.h>
#include <linux/screen_info.h>
#include <linux/platform_device.h>
#include <linux/serial_8250.h>

#include <asm/jazz.h>
#include <asm/jazzdma.h>
#include <asm/reboot.h>
#include <asm/pgtable.h>

extern asmlinkage void jazz_handle_int(void);

extern void jazz_machine_restart(char *command);

static struct resource jazz_io_resources[] = {
	{
		.start	= 0x00,
		.end	= 0x1f,
		.name	= "dma1",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	= 0x40,
		.end	= 0x5f,
		.name	= "timer",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	= 0x80,
		.end	= 0x8f,
		.name	= "dma page reg",
		.flags	= IORESOURCE_BUSY
	}, {
		.start	= 0xc0,
		.end	= 0xdf,
		.name	= "dma2",
		.flags	= IORESOURCE_BUSY
	}
};

void __init plat_mem_setup(void)
{
	int i;

	/* Map 0xe0000000 -> 0x0:800005C0, 0xe0010000 -> 0x1:30000580 */
	add_wired_entry(0x02000017, 0x03c00017, 0xe0000000, PM_64K);
	/* Map 0xe2000000 -> 0x0:900005C0, 0xe3010000 -> 0x0:910005C0 */
	add_wired_entry(0x02400017, 0x02440017, 0xe2000000, PM_16M);
	/* Map 0xe4000000 -> 0x0:600005C0, 0xe4100000 -> 400005C0 */
	add_wired_entry(0x01800017, 0x01000017, 0xe4000000, PM_4M);

	set_io_port_base(JAZZ_PORT_BASE);
#ifdef CONFIG_EISA
	EISA_bus = 1;
#endif

	/* request I/O space for devices used on all i[345]86 PCs */
	for (i = 0; i < ARRAY_SIZE(jazz_io_resources); i++)
		request_resource(&ioport_resource, jazz_io_resources + i);

	/* The RTC is outside the port address space */

	_machine_restart = jazz_machine_restart;

#ifdef CONFIG_VT
	screen_info = (struct screen_info) {
		0, 0,		/* orig-x, orig-y */
		0,		/* unused */
		0,		/* orig_video_page */
		0,		/* orig_video_mode */
		160,		/* orig_video_cols */
		0, 0, 0,	/* unused, ega_bx, unused */
		64,		/* orig_video_lines */
		0,		/* orig_video_isVGA */
		16		/* orig_video_points */
	};
#endif

	add_preferred_console("ttyS", 0, "9600");
}

#ifdef CONFIG_OLIVETTI_M700
#define UART_CLK  1843200
#else
/* Some Jazz machines seem to have an 8MHz crystal clock but I don't know
   exactly which ones ... XXX */
#define UART_CLK (8000000 / 16) /* ( 3072000 / 16) */
#endif

#define MEMPORT(_base, _irq)				\
	{						\
		.mapbase	= (_base),		\
		.membase	= (void *)(_base),	\
		.irq		= (_irq),		\
		.uartclk	= UART_CLK,		\
		.iotype		= UPIO_MEM,		\
		.flags		= UPF_BOOT_AUTOCONF,	\
	}

static struct plat_serial8250_port jazz_serial_data[] = {
	MEMPORT(JAZZ_SERIAL1_BASE, JAZZ_SERIAL1_IRQ),
	MEMPORT(JAZZ_SERIAL2_BASE, JAZZ_SERIAL2_IRQ),
	{ },
};

static struct platform_device jazz_serial8250_device = {
	.name			= "serial8250",
	.id			= PLAT8250_DEV_PLATFORM,
	.dev			= {
		.platform_data	= jazz_serial_data,
	},
};

static struct resource jazz_esp_rsrc[] = {
	{
		.start = JAZZ_SCSI_BASE,
		.end   = JAZZ_SCSI_BASE + 31,
		.flags = IORESOURCE_MEM
	},
	{
		.start = JAZZ_SCSI_DMA,
		.end   = JAZZ_SCSI_DMA,
		.flags = IORESOURCE_MEM
	},
	{
		.start = JAZZ_SCSI_IRQ,
		.end   = JAZZ_SCSI_IRQ,
		.flags = IORESOURCE_IRQ
	}
};

static struct platform_device jazz_esp_pdev = {
	.name           = "jazz_esp",
	.num_resources  = ARRAY_SIZE(jazz_esp_rsrc),
	.resource       = jazz_esp_rsrc
};

static struct resource jazz_sonic_rsrc[] = {
	{
		.start = JAZZ_ETHERNET_BASE,
		.end   = JAZZ_ETHERNET_BASE + 0xff,
		.flags = IORESOURCE_MEM
	},
	{
		.start = JAZZ_ETHERNET_IRQ,
		.end   = JAZZ_ETHERNET_IRQ,
		.flags = IORESOURCE_IRQ
	}
};

static struct platform_device jazz_sonic_pdev = {
	.name           = "jazzsonic",
	.num_resources  = ARRAY_SIZE(jazz_sonic_rsrc),
	.resource       = jazz_sonic_rsrc
};

static struct resource jazz_cmos_rsrc[] = {
	{
		.start = 0x70,
		.end   = 0x71,
		.flags = IORESOURCE_IO
	},
	{
		.start = 8,
		.end   = 8,
		.flags = IORESOURCE_IRQ
	}
};

static struct platform_device jazz_cmos_pdev = {
	.name           = "rtc_cmos",
	.num_resources  = ARRAY_SIZE(jazz_cmos_rsrc),
	.resource       = jazz_cmos_rsrc
};

static struct platform_device pcspeaker_pdev = {
	.name           = "pcspkr",
	.id		= -1,
};

static int __init jazz_setup_devinit(void)
{
	platform_device_register(&jazz_serial8250_device);
	platform_device_register(&jazz_esp_pdev);
	platform_device_register(&jazz_sonic_pdev);
	platform_device_register(&jazz_cmos_pdev);
	platform_device_register(&pcspeaker_pdev);

	return 0;
}

device_initcall(jazz_setup_devinit);