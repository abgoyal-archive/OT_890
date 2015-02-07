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
 *  linux/arch/arm/mach-shark/arch.c
 *
 *  Architecture specific stuff.
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/serial_8250.h>
#include <linux/io.h>

#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/leds.h>
#include <asm/param.h>

#include <mach/hardware.h>

#include <asm/mach/map.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>

static struct plat_serial8250_port serial_platform_data[] = {
	{
		.iobase		= 0x3f8,
		.irq		= 4,
		.uartclk	= 1843200,
		.regshift	= 0,
		.iotype		= UPIO_PORT,
		.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
	},
	{
		.iobase		= 0x2f8,
		.irq		= 3,
		.uartclk	= 1843200,
		.regshift	= 0,
		.iotype		= UPIO_PORT,
		.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,
	},
	{ },
};

static struct platform_device serial_device = {
	.name			= "serial8250",
	.id			= PLAT8250_DEV_PLATFORM,
	.dev			= {
		.platform_data	= serial_platform_data,
	},
};

static int __init shark_init(void)
{
	int ret;

	if (machine_is_shark())
		ret = platform_device_register(&serial_device);

	return ret;
}

arch_initcall(shark_init);

extern void shark_init_irq(void);

static struct map_desc shark_io_desc[] __initdata = {
	{
		.virtual	= IO_BASE,
		.pfn		= __phys_to_pfn(IO_START),
		.length		= IO_SIZE,
		.type		= MT_DEVICE
	}
};

static void __init shark_map_io(void)
{
	iotable_init(shark_io_desc, ARRAY_SIZE(shark_io_desc));
}

#define IRQ_TIMER 0
#define HZ_TIME ((1193180 + HZ/2) / HZ)

static irqreturn_t
shark_timer_interrupt(int irq, void *dev_id)
{
	timer_tick();
	return IRQ_HANDLED;
}

static struct irqaction shark_timer_irq = {
	.name		= "Shark Timer Tick",
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
	.handler	= shark_timer_interrupt,
};

/*
 * Set up timer interrupt, and return the current time in seconds.
 */
static void __init shark_timer_init(void)
{
	outb(0x34, 0x43);               /* binary, mode 0, LSB/MSB, Ch 0 */
	outb(HZ_TIME & 0xff, 0x40);     /* LSB of count */
	outb(HZ_TIME >> 8, 0x40);

	setup_irq(IRQ_TIMER, &shark_timer_irq);
}

static struct sys_timer shark_timer = {
	.init		= shark_timer_init,
};

MACHINE_START(SHARK, "Shark")
	/* Maintainer: Alexander Schulz */
	.phys_io	= 0x40000000,
	.io_pg_offst	= ((0xe0000000) >> 18) & 0xfffc,
	.boot_params	= 0x08003000,
	.map_io		= shark_map_io,
	.init_irq	= shark_init_irq,
	.timer		= &shark_timer,
MACHINE_END
