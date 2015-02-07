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

    bttv-if.c  --  old gpio interface to other kernel modules
		   don't use in new code, will go away in 2.7
		   have a look at bttv-gpio.c instead.

    bttv - Bt848 frame grabber driver

    Copyright (C) 1996,97,98 Ralph  Metzler (rjkm@thp.uni-koeln.de)
			   & Marcus Metzler (mocm@thp.uni-koeln.de)
    (c) 1999-2003 Gerd Knorr <kraxel@bytesex.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/io.h>

#include "bttvp.h"

EXPORT_SYMBOL(bttv_get_pcidev);
EXPORT_SYMBOL(bttv_gpio_enable);
EXPORT_SYMBOL(bttv_read_gpio);
EXPORT_SYMBOL(bttv_write_gpio);

/* ----------------------------------------------------------------------- */
/* Exported functions - for other modules which want to access the         */
/*                      gpio ports (IR for example)                        */
/*                      see bttv.h for comments                            */

struct pci_dev* bttv_get_pcidev(unsigned int card)
{
	if (card >= bttv_num)
		return NULL;
	return bttvs[card].c.pci;
}


int bttv_gpio_enable(unsigned int card, unsigned long mask, unsigned long data)
{
	struct bttv *btv;

	if (card >= bttv_num) {
		return -EINVAL;
	}

	btv = &bttvs[card];
	gpio_inout(mask,data);
	if (bttv_gpio)
		bttv_gpio_tracking(btv,"extern enable");
	return 0;
}

int bttv_read_gpio(unsigned int card, unsigned long *data)
{
	struct bttv *btv;

	if (card >= bttv_num) {
		return -EINVAL;
	}

	btv = &bttvs[card];

	if(btv->shutdown) {
		return -ENODEV;
	}

/* prior setting BT848_GPIO_REG_INP is (probably) not needed
   because we set direct input on init */
	*data = gpio_read();
	return 0;
}

int bttv_write_gpio(unsigned int card, unsigned long mask, unsigned long data)
{
	struct bttv *btv;

	if (card >= bttv_num) {
		return -EINVAL;
	}

	btv = &bttvs[card];

/* prior setting BT848_GPIO_REG_INP is (probably) not needed
   because direct input is set on init */
	gpio_bits(mask,data);
	if (bttv_gpio)
		bttv_gpio_tracking(btv,"extern write");
	return 0;
}

/*
 * Local variables:
 * c-basic-offset: 8
 * End:
 */
