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

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mtd/physmap.h>
#include <linux/serial_8250.h>
#include <linux/serial_reg.h>
#include <linux/usb/isp116x.h>
#include <linux/delay.h>
#include <asm/machvec.h>
#include <mach-se/mach/se7343.h>
#include <asm/heartbeat.h>
#include <asm/irq.h>
#include <asm/io.h>

static struct resource heartbeat_resources[] = {
	[0] = {
		.start	= PA_LED,
		.end	= PA_LED,
		.flags	= IORESOURCE_MEM,
	},
};

static struct heartbeat_data heartbeat_data = {
	.regsize = 16,
};

static struct platform_device heartbeat_device = {
	.name		= "heartbeat",
	.id		= -1,
	.dev = {
		.platform_data = &heartbeat_data,
	},
	.num_resources	= ARRAY_SIZE(heartbeat_resources),
	.resource	= heartbeat_resources,
};

static struct mtd_partition nor_flash_partitions[] = {
	{
		.name		= "loader",
		.offset		= 0x00000000,
		.size		= 128 * 1024,
	},
	{
		.name		= "rootfs",
		.offset		= MTDPART_OFS_APPEND,
		.size		= 31 * 1024 * 1024,
	},
	{
		.name		= "data",
		.offset		= MTDPART_OFS_APPEND,
		.size		= MTDPART_SIZ_FULL,
	},
};

static struct physmap_flash_data nor_flash_data = {
	.width		= 2,
	.parts		= nor_flash_partitions,
	.nr_parts	= ARRAY_SIZE(nor_flash_partitions),
};

static struct resource nor_flash_resources[] = {
	[0]	= {
		.start	= 0x00000000,
		.end	= 0x01ffffff,
		.flags	= IORESOURCE_MEM,
	}
};

static struct platform_device nor_flash_device = {
	.name		= "physmap-flash",
	.dev		= {
		.platform_data	= &nor_flash_data,
	},
	.num_resources	= ARRAY_SIZE(nor_flash_resources),
	.resource	= nor_flash_resources,
};

#define ST16C2550C_FLAGS (UPF_BOOT_AUTOCONF | UPF_IOREMAP)

static struct plat_serial8250_port serial_platform_data[] = {
	[0] = {
		.iotype		= UPIO_MEM,
		.mapbase	= 0x16000000,
		.regshift	= 1,
		.flags		= ST16C2550C_FLAGS,
		.irq		= UARTA_IRQ,
		.uartclk	= 7372800,
	},
	[1] = {
		.iotype		= UPIO_MEM,
		.mapbase	= 0x17000000,
		.regshift	= 1,
		.flags		= ST16C2550C_FLAGS,
		.irq		= UARTB_IRQ,
		.uartclk	= 7372800,
	},
	{ },
};

static struct platform_device uart_device = {
	.name			= "serial8250",
	.id			= PLAT8250_DEV_PLATFORM,
	.dev			= {
		.platform_data	= serial_platform_data,
	},
};

static void isp116x_delay(struct device *dev, int delay)
{
	ndelay(delay);
}

static struct resource usb_resources[] = {
	[0] = {
		.start  = 0x11800000,
		.end    = 0x11800001,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start  = 0x11800002,
		.end    = 0x11800003,
		.flags  = IORESOURCE_MEM,
	},
	[2] = {
		.start  = USB_IRQ,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct isp116x_platform_data usb_platform_data = {
	.sel15Kres		= 1,
	.oc_enable		= 1,
	.int_act_high		= 0,
	.int_edge_triggered	= 0,
	.remote_wakeup_enable	= 0,
	.delay			= isp116x_delay,
};

static struct platform_device usb_device = {
	.name			= "isp116x-hcd",
	.id			= -1,
	.num_resources  	= ARRAY_SIZE(usb_resources),
	.resource       	= usb_resources,
	.dev			= {
		.platform_data	= &usb_platform_data,
	},

};

static struct platform_device *sh7343se_platform_devices[] __initdata = {
	&heartbeat_device,
	&nor_flash_device,
	&uart_device,
	&usb_device,
};

static int __init sh7343se_devices_setup(void)
{
	return platform_add_devices(sh7343se_platform_devices,
				    ARRAY_SIZE(sh7343se_platform_devices));
}
device_initcall(sh7343se_devices_setup);

/*
 * Initialize the board
 */
static void __init sh7343se_setup(char **cmdline_p)
{
	ctrl_outw(0xf900, FPGA_OUT);	/* FPGA */

	ctrl_outw(0x0002, PORT_PECR);	/* PORT E 1 = IRQ5 */
	ctrl_outw(0x0020, PORT_PSELD);

	printk(KERN_INFO "MS7343CP01 Setup...done\n");
}

/*
 * The Machine Vector
 */
static struct sh_machine_vector mv_7343se __initmv = {
	.mv_name = "SolutionEngine 7343",
	.mv_setup = sh7343se_setup,
	.mv_nr_irqs = SE7343_FPGA_IRQ_BASE + SE7343_FPGA_IRQ_NR,
	.mv_init_irq = init_7343se_IRQ,
};
