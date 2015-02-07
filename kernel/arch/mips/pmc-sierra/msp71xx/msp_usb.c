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
 * The setup file for USB related hardware on PMC-Sierra MSP processors.
 *
 * Copyright 2006-2007 PMC-Sierra, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/dma-mapping.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>

#include <asm/mipsregs.h>

#include <msp_regs.h>
#include <msp_int.h>
#include <msp_prom.h>

#if defined(CONFIG_USB_EHCI_HCD)
static struct resource msp_usbhost_resources [] = {
	[0] = {
		.start	= MSP_USB_BASE_START,
		.end	= MSP_USB_BASE_END,
		.flags 	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= MSP_INT_USB,
		.end	= MSP_INT_USB,
		.flags	= IORESOURCE_IRQ,
	},
};

static u64 msp_usbhost_dma_mask = DMA_32BIT_MASK;

static struct platform_device msp_usbhost_device = {
	.name	= "pmcmsp-ehci",
	.id	= 0,
	.dev	= {
		.dma_mask = &msp_usbhost_dma_mask,
		.coherent_dma_mask = DMA_32BIT_MASK,
	},
	.num_resources 	= ARRAY_SIZE(msp_usbhost_resources),
	.resource	= msp_usbhost_resources,
};
#endif /* CONFIG_USB_EHCI_HCD */

#if defined(CONFIG_USB_GADGET)
static struct resource msp_usbdev_resources [] = {
	[0] = {
		.start	= MSP_USB_BASE,
		.end	= MSP_USB_BASE_END,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= MSP_INT_USB,
		.end	= MSP_INT_USB,
		.flags	= IORESOURCE_IRQ,
	},
};

static u64 msp_usbdev_dma_mask = DMA_32BIT_MASK;

static struct platform_device msp_usbdev_device = {
	.name	= "msp71xx_udc",
	.id	= 0,
	.dev	= {
		.dma_mask = &msp_usbdev_dma_mask,
		.coherent_dma_mask = DMA_32BIT_MASK,
	},
	.num_resources	= ARRAY_SIZE(msp_usbdev_resources),
	.resource	= msp_usbdev_resources,
};
#endif /* CONFIG_USB_GADGET */

#if defined(CONFIG_USB_EHCI_HCD) || defined(CONFIG_USB_GADGET)
static struct platform_device *msp_devs[1];
#endif


static int __init msp_usb_setup(void)
{
#if defined(CONFIG_USB_EHCI_HCD) || defined(CONFIG_USB_GADGET)
	char *strp;
	char envstr[32];
	unsigned int val = 0;
	int result = 0;

	/*
	 * construct environment name usbmode
	 * set usbmode <host/device> as pmon environment var
	 */
	snprintf((char *)&envstr[0], sizeof(envstr), "usbmode");

#if defined(CONFIG_USB_EHCI_HCD)
	/* default to host mode */
	val = 1;
#endif

	/* get environment string */
	strp = prom_getenv((char *)&envstr[0]);
	if (strp) {
		if (!strcmp(strp, "device"))
			val = 0;
	}

	if (val) {
#if defined(CONFIG_USB_EHCI_HCD)
		/* get host mode device */
		msp_devs[0] = &msp_usbhost_device;
		ppfinit("platform add USB HOST done %s.\n",
			    msp_devs[0]->name);

		result = platform_add_devices(msp_devs, ARRAY_SIZE(msp_devs));
#endif /* CONFIG_USB_EHCI_HCD */
	}
#if defined(CONFIG_USB_GADGET)
	else {
		/* get device mode structure */
		msp_devs[0] = &msp_usbdev_device;
		ppfinit("platform add USB DEVICE done %s.\n",
			    msp_devs[0]->name);

		result = platform_add_devices(msp_devs, ARRAY_SIZE(msp_devs));
	}
#endif /* CONFIG_USB_GADGET */
#endif /* CONFIG_USB_EHCI_HCD || CONFIG_USB_GADGET */

	return result;
}

subsys_initcall(msp_usb_setup);
