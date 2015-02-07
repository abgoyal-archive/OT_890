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
 * linux/drivers/pcmcia/sa1100_neponset.c
 *
 * Neponset PCMCIA specific routines
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/init.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <mach/neponset.h>
#include <asm/hardware/sa1111.h>

#include "sa1111_generic.h"

/*
 * Neponset uses the Maxim MAX1600, with the following connections:
 *
 *   MAX1600      Neponset
 *
 *    A0VCC        SA-1111 GPIO A<1>
 *    A1VCC        SA-1111 GPIO A<0>
 *    A0VPP        CPLD NCR A0VPP
 *    A1VPP        CPLD NCR A1VPP
 *    B0VCC        SA-1111 GPIO A<2>
 *    B1VCC        SA-1111 GPIO A<3>
 *    B0VPP        ground (slot B is CF)
 *    B1VPP        ground (slot B is CF)
 *
 *     VX          VCC (5V)
 *     VY          VCC3_3 (3.3V)
 *     12INA       12V
 *     12INB       ground (slot B is CF)
 *
 * The MAX1600 CODE pin is tied to ground, placing the device in 
 * "Standard Intel code" mode. Refer to the Maxim data sheet for
 * the corresponding truth table.
 */

static int
neponset_pcmcia_configure_socket(struct soc_pcmcia_socket *skt, const socket_state_t *state)
{
	unsigned int ncr_mask, ncr_set, pa_dwr_mask, pa_dwr_set;
	int ret;

	switch (skt->nr) {
	case 0:
		pa_dwr_mask = GPIO_A0 | GPIO_A1;
		ncr_mask = NCR_A0VPP | NCR_A1VPP;

		if (state->Vpp == 0)
			ncr_set = 0;
		else if (state->Vpp == 120)
			ncr_set = NCR_A1VPP;
		else if (state->Vpp == state->Vcc)
			ncr_set = NCR_A0VPP;
		else {
			printk(KERN_ERR "%s(): unrecognized VPP %u\n",
			       __func__, state->Vpp);
			return -1;
		}
		break;

	case 1:
		pa_dwr_mask = GPIO_A2 | GPIO_A3;
		ncr_mask = 0;
		ncr_set = 0;

		if (state->Vpp != state->Vcc && state->Vpp != 0) {
			printk(KERN_ERR "%s(): CF slot cannot support VPP %u\n",
			       __func__, state->Vpp);
			return -1;
		}
		break;

	default:
		return -1;
	}

	/*
	 * pa_dwr_set is the mask for selecting Vcc on both sockets.
	 * pa_dwr_mask selects which bits (and therefore socket) we change.
	 */
	switch (state->Vcc) {
	default:
	case 0:  pa_dwr_set = 0;		break;
	case 33: pa_dwr_set = GPIO_A1|GPIO_A2;	break;
	case 50: pa_dwr_set = GPIO_A0|GPIO_A3;	break;
	}

	ret = sa1111_pcmcia_configure_socket(skt, state);
	if (ret == 0) {
		unsigned long flags;

		local_irq_save(flags);
		NCR_0 = (NCR_0 & ~ncr_mask) | ncr_set;

		local_irq_restore(flags);
		sa1111_set_io(SA1111_DEV(skt->dev), pa_dwr_mask, pa_dwr_set);
	}

	return 0;
}

static void neponset_pcmcia_socket_init(struct soc_pcmcia_socket *skt)
{
	if (skt->nr == 0)
		NCR_0 &= ~(NCR_A0VPP | NCR_A1VPP);

	sa1111_pcmcia_socket_init(skt);
}

static struct pcmcia_low_level neponset_pcmcia_ops = {
	.owner			= THIS_MODULE,
	.hw_init		= sa1111_pcmcia_hw_init,
	.hw_shutdown		= sa1111_pcmcia_hw_shutdown,
	.socket_state		= sa1111_pcmcia_socket_state,
	.configure_socket	= neponset_pcmcia_configure_socket,
	.socket_init		= neponset_pcmcia_socket_init,
	.socket_suspend 	= sa1111_pcmcia_socket_suspend,
};

int __init pcmcia_neponset_init(struct sa1111_dev *sadev)
{
	int ret = -ENODEV;

	if (machine_is_assabet()) {
		/*
		 * Set GPIO_A<3:0> to be outputs for the MAX1600,
		 * and switch to standby mode.
		 */
		sa1111_set_io_dir(sadev, GPIO_A0|GPIO_A1|GPIO_A2|GPIO_A3, 0, 0);
		sa1111_set_io(sadev, GPIO_A0|GPIO_A1|GPIO_A2|GPIO_A3, 0);
		sa1111_set_sleep_io(sadev, GPIO_A0|GPIO_A1|GPIO_A2|GPIO_A3, 0);
		ret = sa11xx_drv_pcmcia_probe(&sadev->dev, &neponset_pcmcia_ops, 0, 2);
	}

	return ret;
}
