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
 * drivers/pcmcia/sa1100_simpad.c
 *
 * PCMCIA implementation routines for simpad
 *
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/init.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/irq.h>
#include <mach/simpad.h>
#include "sa1100_generic.h"
 
extern long get_cs3_shadow(void);
extern void set_cs3_bit(int value); 
extern void clear_cs3_bit(int value);

static struct pcmcia_irqs irqs[] = {
	{ 1, IRQ_GPIO_CF_CD, "CF_CD" },
};

static int simpad_pcmcia_hw_init(struct soc_pcmcia_socket *skt)
{

	clear_cs3_bit(VCC_3V_EN|VCC_5V_EN|EN0|EN1);

	skt->irq = IRQ_GPIO_CF_IRQ;

	return soc_pcmcia_request_irqs(skt, irqs, ARRAY_SIZE(irqs));
}

static void simpad_pcmcia_hw_shutdown(struct soc_pcmcia_socket *skt)
{
	soc_pcmcia_free_irqs(skt, irqs, ARRAY_SIZE(irqs));

	/* Disable CF bus: */
	//set_cs3_bit(PCMCIA_BUFF_DIS);
	clear_cs3_bit(PCMCIA_RESET);       
}

static void
simpad_pcmcia_socket_state(struct soc_pcmcia_socket *skt,
			   struct pcmcia_state *state)
{
	unsigned long levels = GPLR;
	long cs3reg = get_cs3_shadow();

	state->detect=((levels & GPIO_CF_CD)==0)?1:0;
	state->ready=(levels & GPIO_CF_IRQ)?1:0;
	state->bvd1=1; /* Not available on Simpad. */
	state->bvd2=1; /* Not available on Simpad. */
	state->wrprot=0; /* Not available on Simpad. */
  
	if((cs3reg & 0x0c) == 0x0c) {
		state->vs_3v=0;
		state->vs_Xv=0;
	} else {
		state->vs_3v=1;
		state->vs_Xv=0;
	}
}

static int
simpad_pcmcia_configure_socket(struct soc_pcmcia_socket *skt,
			       const socket_state_t *state)
{
	unsigned long flags;

	local_irq_save(flags);

	/* Murphy: see table of MIC2562a-1 */
	switch (state->Vcc) {
	case 0:
		clear_cs3_bit(VCC_3V_EN|VCC_5V_EN|EN0|EN1);
		break;

	case 33:  
		clear_cs3_bit(VCC_3V_EN|EN1);
		set_cs3_bit(VCC_5V_EN|EN0);
		break;

	case 50:
		clear_cs3_bit(VCC_5V_EN|EN1);
		set_cs3_bit(VCC_3V_EN|EN0);
		break;

	default:
		printk(KERN_ERR "%s(): unrecognized Vcc %u\n",
			__func__, state->Vcc);
		clear_cs3_bit(VCC_3V_EN|VCC_5V_EN|EN0|EN1);
		local_irq_restore(flags);
		return -1;
	}


	local_irq_restore(flags);

	return 0;
}

static void simpad_pcmcia_socket_init(struct soc_pcmcia_socket *skt)
{
	soc_pcmcia_enable_irqs(skt, irqs, ARRAY_SIZE(irqs));
}

static void simpad_pcmcia_socket_suspend(struct soc_pcmcia_socket *skt)
{
	soc_pcmcia_disable_irqs(skt, irqs, ARRAY_SIZE(irqs));
	set_cs3_bit(PCMCIA_RESET);
}

static struct pcmcia_low_level simpad_pcmcia_ops = { 
	.owner			= THIS_MODULE,
	.hw_init		= simpad_pcmcia_hw_init,
	.hw_shutdown		= simpad_pcmcia_hw_shutdown,
	.socket_state		= simpad_pcmcia_socket_state,
	.configure_socket	= simpad_pcmcia_configure_socket,
	.socket_init		= simpad_pcmcia_socket_init,
	.socket_suspend		= simpad_pcmcia_socket_suspend,
};

int __init pcmcia_simpad_init(struct device *dev)
{
	int ret = -ENODEV;

	if (machine_is_simpad())
		ret = sa11xx_drv_pcmcia_probe(dev, &simpad_pcmcia_ops, 1, 1);

	return ret;
}
