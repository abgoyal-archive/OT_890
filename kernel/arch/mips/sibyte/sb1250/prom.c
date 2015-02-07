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
 * Copyright (C) 2000, 2001 Broadcom Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/blkdev.h>
#include <linux/bootmem.h>
#include <linux/smp.h>
#include <linux/initrd.h>
#include <linux/pm.h>

#include <asm/bootinfo.h>
#include <asm/reboot.h>

#define MAX_RAM_SIZE ((CONFIG_SIBYTE_STANDALONE_RAM_SIZE * 1024 * 1024) - 1)

static __init void prom_meminit(void)
{
#ifdef CONFIG_BLK_DEV_INITRD
	unsigned long initrd_pstart;
	unsigned long initrd_pend;

	initrd_pstart = __pa(initrd_start);
	initrd_pend = __pa(initrd_end);
	if (initrd_start &&
	    ((initrd_pstart > MAX_RAM_SIZE)
	     || (initrd_pend > MAX_RAM_SIZE))) {
		panic("initrd out of addressable memory");
	}

	add_memory_region(0, initrd_pstart,
			  BOOT_MEM_RAM);
	add_memory_region(initrd_pstart, initrd_pend - initrd_pstart,
			  BOOT_MEM_RESERVED);
	add_memory_region(initrd_pend,
			  (CONFIG_SIBYTE_STANDALONE_RAM_SIZE * 1024 * 1024) - initrd_pend,
			  BOOT_MEM_RAM);
#else
	add_memory_region(0, CONFIG_SIBYTE_STANDALONE_RAM_SIZE * 1024 * 1024,
			  BOOT_MEM_RAM);
#endif
}

void prom_cpu0_exit(void *unused)
{
        while (1) ;
}

static void prom_linux_exit(void)
{
#ifdef CONFIG_SMP
	if (smp_processor_id()) {
		smp_call_function(prom_cpu0_exit, NULL, 1);
	}
#endif
	while(1);
}

/*
 * prom_init is called just after the cpu type is determined, from setup_arch()
 */
void __init prom_init(void)
{
	_machine_restart   = (void (*)(char *))prom_linux_exit;
	_machine_halt      = prom_linux_exit;
	pm_power_off = prom_linux_exit;

	strcpy(arcs_cmdline, "root=/dev/ram0 ");

	prom_meminit();
}

void __init prom_free_prom_memory(void)
{
	/* Not sure what I'm supposed to do here.  Nothing, I think */
}

void prom_putchar(char c)
{
}
