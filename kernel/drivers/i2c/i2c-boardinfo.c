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
 * i2c-boardinfo.h - collect pre-declarations of I2C devices
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/kernel.h>
#include <linux/i2c.h>

#include "i2c-core.h"


/* These symbols are exported ONLY FOR the i2c core.
 * No other users will be supported.
 */
DEFINE_MUTEX(__i2c_board_lock);
EXPORT_SYMBOL_GPL(__i2c_board_lock);

LIST_HEAD(__i2c_board_list);
EXPORT_SYMBOL_GPL(__i2c_board_list);

int __i2c_first_dynamic_bus_num;
EXPORT_SYMBOL_GPL(__i2c_first_dynamic_bus_num);


/**
 * i2c_register_board_info - statically declare I2C devices
 * @busnum: identifies the bus to which these devices belong
 * @info: vector of i2c device descriptors
 * @len: how many descriptors in the vector; may be zero to reserve
 *	the specified bus number.
 *
 * Systems using the Linux I2C driver stack can declare tables of board info
 * while they initialize.  This should be done in board-specific init code
 * near arch_initcall() time, or equivalent, before any I2C adapter driver is
 * registered.  For example, mainboard init code could define several devices,
 * as could the init code for each daughtercard in a board stack.
 *
 * The I2C devices will be created later, after the adapter for the relevant
 * bus has been registered.  After that moment, standard driver model tools
 * are used to bind "new style" I2C drivers to the devices.  The bus number
 * for any device declared using this routine is not available for dynamic
 * allocation.
 *
 * The board info passed can safely be __initdata, but be careful of embedded
 * pointers (for platform_data, functions, etc) since that won't be copied.
 */
int __init
i2c_register_board_info(int busnum,
	struct i2c_board_info const *info, unsigned len)
{
	int status;

	mutex_lock(&__i2c_board_lock);

	/* dynamic bus numbers will be assigned after the last static one */
	if (busnum >= __i2c_first_dynamic_bus_num)
		__i2c_first_dynamic_bus_num = busnum + 1;

	for (status = 0; len; len--, info++) {
		struct i2c_devinfo	*devinfo;

		devinfo = kzalloc(sizeof(*devinfo), GFP_KERNEL);
		if (!devinfo) {
			pr_debug("i2c-core: can't register boardinfo!\n");
			status = -ENOMEM;
			break;
		}

		devinfo->busnum = busnum;
		devinfo->board_info = *info;
		list_add_tail(&devinfo->list, &__i2c_board_list);
	}

	mutex_unlock(&__i2c_board_lock);

	return status;
}
