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

#include "radeonfb.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/fb.h>


#include <linux/i2c.h>
#include <linux/i2c-id.h>
#include <linux/i2c-algo-bit.h>

#include <asm/io.h>

#include <video/radeon.h>
#include "../edid.h"

static void radeon_gpio_setscl(void* data, int state)
{
	struct radeon_i2c_chan 	*chan = data;
	struct radeonfb_info	*rinfo = chan->rinfo;
	u32			val;
	
	val = INREG(chan->ddc_reg) & ~(VGA_DDC_CLK_OUT_EN);
	if (!state)
		val |= VGA_DDC_CLK_OUT_EN;

	OUTREG(chan->ddc_reg, val);
	(void)INREG(chan->ddc_reg);
}

static void radeon_gpio_setsda(void* data, int state)
{
	struct radeon_i2c_chan 	*chan = data;
	struct radeonfb_info	*rinfo = chan->rinfo;
	u32			val;
	
	val = INREG(chan->ddc_reg) & ~(VGA_DDC_DATA_OUT_EN);
	if (!state)
		val |= VGA_DDC_DATA_OUT_EN;

	OUTREG(chan->ddc_reg, val);
	(void)INREG(chan->ddc_reg);
}

static int radeon_gpio_getscl(void* data)
{
	struct radeon_i2c_chan 	*chan = data;
	struct radeonfb_info	*rinfo = chan->rinfo;
	u32			val;
	
	val = INREG(chan->ddc_reg);

	return (val & VGA_DDC_CLK_INPUT) ? 1 : 0;
}

static int radeon_gpio_getsda(void* data)
{
	struct radeon_i2c_chan 	*chan = data;
	struct radeonfb_info	*rinfo = chan->rinfo;
	u32			val;
	
	val = INREG(chan->ddc_reg);

	return (val & VGA_DDC_DATA_INPUT) ? 1 : 0;
}

static int radeon_setup_i2c_bus(struct radeon_i2c_chan *chan, const char *name)
{
	int rc;

	snprintf(chan->adapter.name, sizeof(chan->adapter.name),
		 "radeonfb %s", name);
	chan->adapter.owner		= THIS_MODULE;
	chan->adapter.algo_data		= &chan->algo;
	chan->adapter.dev.parent	= &chan->rinfo->pdev->dev;
	chan->algo.setsda		= radeon_gpio_setsda;
	chan->algo.setscl		= radeon_gpio_setscl;
	chan->algo.getsda		= radeon_gpio_getsda;
	chan->algo.getscl		= radeon_gpio_getscl;
	chan->algo.udelay		= 10;
	chan->algo.timeout		= 20;
	chan->algo.data 		= chan;	
	
	i2c_set_adapdata(&chan->adapter, chan);
	
	/* Raise SCL and SDA */
	radeon_gpio_setsda(chan, 1);
	radeon_gpio_setscl(chan, 1);
	udelay(20);

	rc = i2c_bit_add_bus(&chan->adapter);
	if (rc == 0)
		dev_dbg(&chan->rinfo->pdev->dev, "I2C bus %s registered.\n", name);
	else
		dev_warn(&chan->rinfo->pdev->dev, "Failed to register I2C bus %s.\n", name);
	return rc;
}

void radeon_create_i2c_busses(struct radeonfb_info *rinfo)
{
	rinfo->i2c[0].rinfo	= rinfo;
	rinfo->i2c[0].ddc_reg	= GPIO_MONID;
	radeon_setup_i2c_bus(&rinfo->i2c[0], "monid");

	rinfo->i2c[1].rinfo	= rinfo;
	rinfo->i2c[1].ddc_reg	= GPIO_DVI_DDC;
	radeon_setup_i2c_bus(&rinfo->i2c[1], "dvi");

	rinfo->i2c[2].rinfo	= rinfo;
	rinfo->i2c[2].ddc_reg	= GPIO_VGA_DDC;
	radeon_setup_i2c_bus(&rinfo->i2c[2], "vga");

	rinfo->i2c[3].rinfo	= rinfo;
	rinfo->i2c[3].ddc_reg	= GPIO_CRT2_DDC;
	radeon_setup_i2c_bus(&rinfo->i2c[3], "crt2");
}

void radeon_delete_i2c_busses(struct radeonfb_info *rinfo)
{
	if (rinfo->i2c[0].rinfo)
		i2c_del_adapter(&rinfo->i2c[0].adapter);
	rinfo->i2c[0].rinfo = NULL;

	if (rinfo->i2c[1].rinfo)
		i2c_del_adapter(&rinfo->i2c[1].adapter);
	rinfo->i2c[1].rinfo = NULL;

	if (rinfo->i2c[2].rinfo)
		i2c_del_adapter(&rinfo->i2c[2].adapter);
	rinfo->i2c[2].rinfo = NULL;

	if (rinfo->i2c[3].rinfo)
		i2c_del_adapter(&rinfo->i2c[3].adapter);
	rinfo->i2c[3].rinfo = NULL;
}

int radeon_probe_i2c_connector(struct radeonfb_info *rinfo, int conn,
			       u8 **out_edid)
{
	u8 *edid;

	edid = fb_ddc_read(&rinfo->i2c[conn-1].adapter);

	if (out_edid)
		*out_edid = edid;
	if (!edid) {
		pr_debug("radeonfb: I2C (port %d) ... not found\n", conn);
		return MT_NONE;
	}
	if (edid[0x14] & 0x80) {
		/* Fix detection using BIOS tables */
		if (rinfo->is_mobility /*&& conn == ddc_dvi*/ &&
		    (INREG(LVDS_GEN_CNTL) & LVDS_ON)) {
			pr_debug("radeonfb: I2C (port %d) ... found LVDS panel\n", conn);
			return MT_LCD;
		} else {
			pr_debug("radeonfb: I2C (port %d) ... found TMDS panel\n", conn);
			return MT_DFP;
		}
	}
	pr_debug("radeonfb: I2C (port %d) ... found CRT display\n", conn);
	return MT_CRT;
}

