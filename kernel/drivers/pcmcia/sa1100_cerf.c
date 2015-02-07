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
 * drivers/pcmcia/sa1100_cerf.c
 *
 * PCMCIA implementation routines for CerfBoard
 * Based off the Assabet.
 *
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/delay.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/irq.h>
#include <mach/cerf.h>
#include "sa1100_generic.h"

#define CERF_SOCKET	1

static struct pcmcia_irqs irqs[] = {
	{ CERF_SOCKET, CERF_IRQ_GPIO_CF_CD,   "CF_CD"   },
	{ CERF_SOCKET, CERF_IRQ_GPIO_CF_BVD2, "CF_BVD2" },
	{ CERF_SOCKET, CERF_IRQ_GPIO_CF_BVD1, "CF_BVD1" }
};

static int cerf_pcmcia_hw_init(struct soc_pcmcia_socket *skt)
{
	skt->irq = CERF_IRQ_GPIO_CF_IRQ;

	return soc_pcmcia_request_irqs(skt, irqs, ARRAY_SIZE(irqs));
}

static void cerf_pcmcia_hw_shutdown(struct soc_pcmcia_socket *skt)
{
	soc_pcmcia_free_irqs(skt, irqs, ARRAY_SIZE(irqs));
}

static void
cerf_pcmcia_socket_state(struct soc_pcmcia_socket *skt, struct pcmcia_state *state)
{
	unsigned long levels = GPLR;

	state->detect	= (levels & CERF_GPIO_CF_CD)  ?0:1;
	state->ready	= (levels & CERF_GPIO_CF_IRQ) ?1:0;
	state->bvd1	= (levels & CERF_GPIO_CF_BVD1)?1:0;
	state->bvd2	= (levels & CERF_GPIO_CF_BVD2)?1:0;
	state->wrprot	= 0;
	state->vs_3v	= 1;
	state->vs_Xv	= 0;
}

static int
cerf_pcmcia_configure_socket(struct soc_pcmcia_socket *skt,
			     const socket_state_t *state)
{
	switch (state->Vcc) {
	case 0:
	case 50:
	case 33:
		break;

	default:
		printk(KERN_ERR "%s(): unrecognized Vcc %u\n",
			__func__, state->Vcc);
		return -1;
	}

	if (state->flags & SS_RESET) {
		GPSR = CERF_GPIO_CF_RESET;
	} else {
		GPCR = CERF_GPIO_CF_RESET;
	}

	return 0;
}

static void cerf_pcmcia_socket_init(struct soc_pcmcia_socket *skt)
{
	soc_pcmcia_enable_irqs(skt, irqs, ARRAY_SIZE(irqs));
}

static void cerf_pcmcia_socket_suspend(struct soc_pcmcia_socket *skt)
{
	soc_pcmcia_disable_irqs(skt, irqs, ARRAY_SIZE(irqs));
}

static struct pcmcia_low_level cerf_pcmcia_ops = { 
	.owner			= THIS_MODULE,
	.hw_init		= cerf_pcmcia_hw_init,
	.hw_shutdown		= cerf_pcmcia_hw_shutdown,
	.socket_state		= cerf_pcmcia_socket_state,
	.configure_socket	= cerf_pcmcia_configure_socket,

	.socket_init		= cerf_pcmcia_socket_init,
	.socket_suspend		= cerf_pcmcia_socket_suspend,
};

int __init pcmcia_cerf_init(struct device *dev)
{
	int ret = -ENODEV;

	if (machine_is_cerf())
		ret = sa11xx_drv_pcmcia_probe(dev, &cerf_pcmcia_ops, CERF_SOCKET, 1);

	return ret;
}
