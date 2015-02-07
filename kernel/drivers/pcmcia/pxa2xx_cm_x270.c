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
 * linux/drivers/pcmcia/pxa/pxa_cm_x270.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Compulab Ltd., 2003, 2007, 2008
 * Mike Rapoport <mike@compulab.co.il>
 *
 */

#include <linux/platform_device.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/gpio.h>

#include <asm/mach-types.h>
#include <mach/pxa-regs.h>

#include "soc_common.h"

#define GPIO_PCMCIA_S0_CD_VALID	(84)
#define GPIO_PCMCIA_S0_RDYINT	(82)
#define GPIO_PCMCIA_RESET	(53)

#define PCMCIA_S0_CD_VALID	IRQ_GPIO(GPIO_PCMCIA_S0_CD_VALID)
#define PCMCIA_S0_RDYINT	IRQ_GPIO(GPIO_PCMCIA_S0_RDYINT)


static struct pcmcia_irqs irqs[] = {
	{ 0, PCMCIA_S0_CD_VALID, "PCMCIA0 CD" },
};

static int cmx270_pcmcia_hw_init(struct soc_pcmcia_socket *skt)
{
	int ret = gpio_request(GPIO_PCMCIA_RESET, "PCCard reset");
	if (ret)
		return ret;
	gpio_direction_output(GPIO_PCMCIA_RESET, 0);

	skt->irq = PCMCIA_S0_RDYINT;
	ret = soc_pcmcia_request_irqs(skt, irqs, ARRAY_SIZE(irqs));
	if (!ret)
		gpio_free(GPIO_PCMCIA_RESET);

	return ret;
}

static void cmx270_pcmcia_shutdown(struct soc_pcmcia_socket *skt)
{
	soc_pcmcia_free_irqs(skt, irqs, ARRAY_SIZE(irqs));
	gpio_free(GPIO_PCMCIA_RESET);
}


static void cmx270_pcmcia_socket_state(struct soc_pcmcia_socket *skt,
				       struct pcmcia_state *state)
{
	state->detect = (gpio_get_value(GPIO_PCMCIA_S0_CD_VALID) == 0) ? 1 : 0;
	state->ready  = (gpio_get_value(GPIO_PCMCIA_S0_RDYINT) == 0) ? 0 : 1;
	state->bvd1   = 1;
	state->bvd2   = 1;
	state->vs_3v  = 0;
	state->vs_Xv  = 0;
	state->wrprot = 0;  /* not available */
}


static int cmx270_pcmcia_configure_socket(struct soc_pcmcia_socket *skt,
					  const socket_state_t *state)
{
	switch (skt->nr) {
	case 0:
		if (state->flags & SS_RESET) {
			gpio_set_value(GPIO_PCMCIA_RESET, 1);
			udelay(10);
			gpio_set_value(GPIO_PCMCIA_RESET, 0);
		}
		break;
	}

	return 0;
}

static void cmx270_pcmcia_socket_init(struct soc_pcmcia_socket *skt)
{
}

static void cmx270_pcmcia_socket_suspend(struct soc_pcmcia_socket *skt)
{
}


static struct pcmcia_low_level cmx270_pcmcia_ops __initdata = {
	.owner			= THIS_MODULE,
	.hw_init		= cmx270_pcmcia_hw_init,
	.hw_shutdown		= cmx270_pcmcia_shutdown,
	.socket_state		= cmx270_pcmcia_socket_state,
	.configure_socket	= cmx270_pcmcia_configure_socket,
	.socket_init		= cmx270_pcmcia_socket_init,
	.socket_suspend		= cmx270_pcmcia_socket_suspend,
	.nr			= 1,
};

static struct platform_device *cmx270_pcmcia_device;

int __init cmx270_pcmcia_init(void)
{
	int ret;

	cmx270_pcmcia_device = platform_device_alloc("pxa2xx-pcmcia", -1);

	if (!cmx270_pcmcia_device)
		return -ENOMEM;

	ret = platform_device_add_data(cmx270_pcmcia_device, &cmx270_pcmcia_ops,
				       sizeof(cmx270_pcmcia_ops));

	if (ret == 0) {
		printk(KERN_INFO "Registering cm-x270 PCMCIA interface.\n");
		ret = platform_device_add(cmx270_pcmcia_device);
	}

	if (ret)
		platform_device_put(cmx270_pcmcia_device);

	return ret;
}

void __exit cmx270_pcmcia_exit(void)
{
	platform_device_unregister(cmx270_pcmcia_device);
}
