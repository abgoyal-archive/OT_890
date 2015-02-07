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

/* linux/drivers/i2c/busses/scx200_i2c.c

   Copyright (c) 2001,2002 Christer Weinigel <wingel@nano-system.com>

   National Semiconductor SCx200 I2C bus on GPIO pins

   Based on i2c-velleman.c Copyright (C) 1995-96, 2000 Simon G. Vogl

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
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
#include <asm/io.h>

#include <linux/scx200_gpio.h>

#define NAME "scx200_i2c"

MODULE_AUTHOR("Christer Weinigel <wingel@nano-system.com>");
MODULE_DESCRIPTION("NatSemi SCx200 I2C Driver");
MODULE_LICENSE("GPL");

static int scl = CONFIG_SCx200_I2C_SCL;
static int sda = CONFIG_SCx200_I2C_SDA;

module_param(scl, int, 0);
MODULE_PARM_DESC(scl, "GPIO line for SCL");
module_param(sda, int, 0);
MODULE_PARM_DESC(sda, "GPIO line for SDA");

static void scx200_i2c_setscl(void *data, int state)
{
	scx200_gpio_set(scl, state);
}

static void scx200_i2c_setsda(void *data, int state)
{
	scx200_gpio_set(sda, state);
} 

static int scx200_i2c_getscl(void *data)
{
	return scx200_gpio_get(scl);
}

static int scx200_i2c_getsda(void *data)
{
	return scx200_gpio_get(sda);
}

/* ------------------------------------------------------------------------
 * Encapsulate the above functions in the correct operations structure.
 * This is only done when more than one hardware adapter is supported.
 */

static struct i2c_algo_bit_data scx200_i2c_data = {
	.setsda		= scx200_i2c_setsda,
	.setscl		= scx200_i2c_setscl,
	.getsda		= scx200_i2c_getsda,
	.getscl		= scx200_i2c_getscl,
	.udelay		= 10,
	.timeout	= HZ,
};

static struct i2c_adapter scx200_i2c_ops = {
	.owner		   = THIS_MODULE,
	.class             = I2C_CLASS_HWMON | I2C_CLASS_SPD,
	.algo_data	   = &scx200_i2c_data,
	.name	= "NatSemi SCx200 I2C",
};

static int scx200_i2c_init(void)
{
	pr_debug(NAME ": NatSemi SCx200 I2C Driver\n");

	if (!scx200_gpio_present()) {
		printk(KERN_ERR NAME ": no SCx200 gpio pins available\n");
		return -ENODEV;
	}

	pr_debug(NAME ": SCL=GPIO%02u, SDA=GPIO%02u\n", scl, sda);

	if (scl == -1 || sda == -1 || scl == sda) {
		printk(KERN_ERR NAME ": scl and sda must be specified\n");
		return -EINVAL;
	}

	/* Configure GPIOs as open collector outputs */
	scx200_gpio_configure(scl, ~2, 5);
	scx200_gpio_configure(sda, ~2, 5);

	if (i2c_bit_add_bus(&scx200_i2c_ops) < 0) {
		printk(KERN_ERR NAME ": adapter %s registration failed\n", 
		       scx200_i2c_ops.name);
		return -ENODEV;
	}
	
	return 0;
}

static void scx200_i2c_cleanup(void)
{
	i2c_del_adapter(&scx200_i2c_ops);
}

module_init(scx200_i2c_init);
module_exit(scx200_i2c_cleanup);

/*
    Local variables:
        compile-command: "make -k -C ../.. SUBDIRS=drivers/i2c modules"
        c-basic-offset: 8
    End:
*/
