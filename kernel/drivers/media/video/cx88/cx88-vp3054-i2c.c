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

    cx88-vp3054-i2c.c  --  support for the secondary I2C bus of the
			   DNTV Live! DVB-T Pro (VP-3054), wired as:
			   GPIO[0] -> SCL, GPIO[1] -> SDA

    (c) 2005 Chris Pascoe <c.pascoe@itee.uq.edu.au>

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

#include <asm/io.h>

#include "cx88.h"
#include "cx88-vp3054-i2c.h"

MODULE_DESCRIPTION("driver for cx2388x VP3054 design");
MODULE_AUTHOR("Chris Pascoe <c.pascoe@itee.uq.edu.au>");
MODULE_LICENSE("GPL");

/* ----------------------------------------------------------------------- */

static void vp3054_bit_setscl(void *data, int state)
{
	struct cx8802_dev *dev = data;
	struct cx88_core *core = dev->core;
	struct vp3054_i2c_state *vp3054_i2c = dev->vp3054;

	if (state) {
		vp3054_i2c->state |=  0x0001;	/* SCL high */
		vp3054_i2c->state &= ~0x0100;	/* external pullup */
	} else {
		vp3054_i2c->state &= ~0x0001;	/* SCL low */
		vp3054_i2c->state |=  0x0100;	/* drive pin */
	}
	cx_write(MO_GP0_IO, 0x010000 | vp3054_i2c->state);
	cx_read(MO_GP0_IO);
}

static void vp3054_bit_setsda(void *data, int state)
{
	struct cx8802_dev *dev = data;
	struct cx88_core *core = dev->core;
	struct vp3054_i2c_state *vp3054_i2c = dev->vp3054;

	if (state) {
		vp3054_i2c->state |=  0x0002;	/* SDA high */
		vp3054_i2c->state &= ~0x0200;	/* tristate pin */
	} else {
		vp3054_i2c->state &= ~0x0002;	/* SDA low */
		vp3054_i2c->state |=  0x0200;	/* drive pin */
	}
	cx_write(MO_GP0_IO, 0x020000 | vp3054_i2c->state);
	cx_read(MO_GP0_IO);
}

static int vp3054_bit_getscl(void *data)
{
	struct cx8802_dev *dev = data;
	struct cx88_core *core = dev->core;
	u32 state;

	state = cx_read(MO_GP0_IO);
	return (state & 0x01) ? 1 : 0;
}

static int vp3054_bit_getsda(void *data)
{
	struct cx8802_dev *dev = data;
	struct cx88_core *core = dev->core;
	u32 state;

	state = cx_read(MO_GP0_IO);
	return (state & 0x02) ? 1 : 0;
}

/* ----------------------------------------------------------------------- */

static const struct i2c_algo_bit_data vp3054_i2c_algo_template = {
	.setsda  = vp3054_bit_setsda,
	.setscl  = vp3054_bit_setscl,
	.getsda  = vp3054_bit_getsda,
	.getscl  = vp3054_bit_getscl,
	.udelay  = 16,
	.timeout = 200,
};

/* ----------------------------------------------------------------------- */

int vp3054_i2c_probe(struct cx8802_dev *dev)
{
	struct cx88_core *core = dev->core;
	struct vp3054_i2c_state *vp3054_i2c;
	int rc;

	if (core->boardnr != CX88_BOARD_DNTV_LIVE_DVB_T_PRO)
		return 0;

	vp3054_i2c = kzalloc(sizeof(*vp3054_i2c), GFP_KERNEL);
	if (vp3054_i2c == NULL)
		return -ENOMEM;
	dev->vp3054 = vp3054_i2c;

	memcpy(&vp3054_i2c->algo, &vp3054_i2c_algo_template,
	       sizeof(vp3054_i2c->algo));

	vp3054_i2c->adap.class |= I2C_CLASS_TV_DIGITAL;

	vp3054_i2c->adap.dev.parent = &dev->pci->dev;
	strlcpy(vp3054_i2c->adap.name, core->name,
		sizeof(vp3054_i2c->adap.name));
	vp3054_i2c->adap.owner = THIS_MODULE;
	vp3054_i2c->adap.id = I2C_HW_B_CX2388x;
	vp3054_i2c->algo.data = dev;
	i2c_set_adapdata(&vp3054_i2c->adap, dev);
	vp3054_i2c->adap.algo_data = &vp3054_i2c->algo;

	vp3054_bit_setscl(dev,1);
	vp3054_bit_setsda(dev,1);

	rc = i2c_bit_add_bus(&vp3054_i2c->adap);
	if (0 != rc) {
		printk("%s: vp3054_i2c register FAILED\n", core->name);

		kfree(dev->vp3054);
		dev->vp3054 = NULL;
	}

	return rc;
}

void vp3054_i2c_remove(struct cx8802_dev *dev)
{
	struct vp3054_i2c_state *vp3054_i2c = dev->vp3054;

	if (vp3054_i2c == NULL ||
	    dev->core->boardnr != CX88_BOARD_DNTV_LIVE_DVB_T_PRO)
		return;

	i2c_del_adapter(&vp3054_i2c->adap);
	kfree(vp3054_i2c);
}

EXPORT_SYMBOL(vp3054_i2c_probe);
EXPORT_SYMBOL(vp3054_i2c_remove);
