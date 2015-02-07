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
 *  linux/arch/arm/kernel/isa.c
 *
 *  Copyright (C) 1999 Phil Blundell
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 *  ISA shared memory and I/O port support, and is required to support
 *  iopl, inb, outb and friends in userspace via glibc emulation.
 */
#include <linux/stddef.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/sysctl.h>
#include <linux/init.h>
#include <linux/io.h>

static unsigned int isa_membase, isa_portbase, isa_portshift;

static ctl_table ctl_isa_vars[4] = {
	{
		.ctl_name	= BUS_ISA_MEM_BASE,
		.procname	= "membase",
		.data		= &isa_membase, 
		.maxlen		= sizeof(isa_membase),
		.mode		= 0444,
		.proc_handler	= &proc_dointvec,
	}, {
		.ctl_name	= BUS_ISA_PORT_BASE,
		.procname	= "portbase",
		.data		= &isa_portbase, 
		.maxlen		= sizeof(isa_portbase),
		.mode		= 0444,
		.proc_handler	= &proc_dointvec,
	}, {
		.ctl_name	= BUS_ISA_PORT_SHIFT,
		.procname	= "portshift",
		.data		= &isa_portshift, 
		.maxlen		= sizeof(isa_portshift),
		.mode		= 0444,
		.proc_handler	= &proc_dointvec,
	}, {0}
};

static struct ctl_table_header *isa_sysctl_header;

static ctl_table ctl_isa[2] = {
	{
		.ctl_name	= CTL_BUS_ISA,
		.procname	= "isa",
		.mode		= 0555,
		.child		= ctl_isa_vars,
	}, {0}
};

static ctl_table ctl_bus[2] = {
	{
		.ctl_name	= CTL_BUS,
		.procname	= "bus",
		.mode		= 0555,
		.child		= ctl_isa,
	}, {0}
};

void __init
register_isa_ports(unsigned int membase, unsigned int portbase, unsigned int portshift)
{
	isa_membase = membase;
	isa_portbase = portbase;
	isa_portshift = portshift;
	isa_sysctl_header = register_sysctl_table(ctl_bus);
}
