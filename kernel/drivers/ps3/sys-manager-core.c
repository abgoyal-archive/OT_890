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
 *  PS3 System Manager core.
 *
 *  Copyright (C) 2007 Sony Computer Entertainment Inc.
 *  Copyright 2007 Sony Corp.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/kernel.h>
#include <asm/lv1call.h>
#include <asm/ps3.h>

/**
 * Staticly linked routines that allow late binding of a loaded sys-manager
 * module.
 */

static struct ps3_sys_manager_ops ps3_sys_manager_ops;

/**
 * ps3_register_sys_manager_ops - Bind ps3_sys_manager_ops to a module.
 * @ops: struct ps3_sys_manager_ops.
 *
 * To be called from ps3_sys_manager_probe() and ps3_sys_manager_remove() to
 * register call back ops for power control.  Copies data to the static
 * variable ps3_sys_manager_ops.
 */

void ps3_sys_manager_register_ops(const struct ps3_sys_manager_ops *ops)
{
	BUG_ON(!ops);
	BUG_ON(!ops->dev);
	ps3_sys_manager_ops = ops ? *ops : ps3_sys_manager_ops;
}
EXPORT_SYMBOL_GPL(ps3_sys_manager_register_ops);

void ps3_sys_manager_power_off(void)
{
	if (ps3_sys_manager_ops.power_off)
		ps3_sys_manager_ops.power_off(ps3_sys_manager_ops.dev);

	ps3_sys_manager_halt();
}

void ps3_sys_manager_restart(void)
{
	if (ps3_sys_manager_ops.restart)
		ps3_sys_manager_ops.restart(ps3_sys_manager_ops.dev);

	ps3_sys_manager_halt();
}

void ps3_sys_manager_halt(void)
{
	pr_emerg("System Halted, OK to turn off power\n");
	local_irq_disable();
	while (1)
		lv1_pause(1);
}

