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
 * Populate sysfs with topology information
 *
 * Written by: Matthew Dobson, IBM Corporation
 * Original Code: Paul Dorwin, IBM Corporation, Patrick Mochel, OSDL
 *
 * Copyright (C) 2002, IBM Corp.
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Send feedback to <colpatch@us.ibm.com>
 */
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/nodemask.h>
#include <linux/mmzone.h>
#include <asm/cpu.h>

static DEFINE_PER_CPU(struct x86_cpu, cpu_devices);

#ifdef CONFIG_HOTPLUG_CPU
int __ref arch_register_cpu(int num)
{
	/*
	 * CPU0 cannot be offlined due to several
	 * restrictions and assumptions in kernel. This basically
	 * doesnt add a control file, one cannot attempt to offline
	 * BSP.
	 *
	 * Also certain PCI quirks require not to enable hotplug control
	 * for all CPU's.
	 */
	if (num)
		per_cpu(cpu_devices, num).cpu.hotpluggable = 1;
	return register_cpu(&per_cpu(cpu_devices, num).cpu, num);
}
EXPORT_SYMBOL(arch_register_cpu);

void arch_unregister_cpu(int num)
{
	unregister_cpu(&per_cpu(cpu_devices, num).cpu);
}
EXPORT_SYMBOL(arch_unregister_cpu);
#else
static int __init arch_register_cpu(int num)
{
	return register_cpu(&per_cpu(cpu_devices, num).cpu, num);
}
#endif /*CONFIG_HOTPLUG_CPU*/

static int __init topology_init(void)
{
	int i;

#ifdef CONFIG_NUMA
	for_each_online_node(i)
		register_one_node(i);
#endif /* CONFIG_NUMA */

	for_each_present_cpu(i)
		arch_register_cpu(i);
	return 0;
}

subsys_initcall(topology_init);
