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
 *  Copyright (C) 2004, 2005 by Thomas Koeller (thomas.koeller@baslerweb.com)
 *  Based on the PMC-Sierra Yosemite board support by Ralf Baechle and
 *  Manish Lachwani.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
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

#include <linux/init.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/smp.h>
#include <linux/module.h>
#include <asm/io.h>
#include <asm/pgtable.h>
#include <asm/processor.h>
#include <asm/reboot.h>
#include <asm/system.h>
#include <asm/bootinfo.h>
#include <asm/string.h>

#include <excite.h>

/* This struct is used by Redboot to pass arguments to the kernel */
typedef struct
{
	char *name;
	char *val;
} t_env_var;

struct parmblock {
	t_env_var memsize;
	t_env_var modetty0;
	t_env_var ethaddr;
	t_env_var env_end;
	char *argv[2];
	char text[0];
};

static unsigned int prom_argc;
static const char ** prom_argv;
static const t_env_var * prom_env;

static void prom_halt(void) __attribute__((noreturn));
static void prom_exit(void) __attribute__((noreturn));



const char *get_system_type(void)
{
	return "Basler eXcite";
}

/*
 * Halt the system
 */
static void prom_halt(void)
{
	printk(KERN_NOTICE "\n** System halted.\n");
	while (1)
		asm volatile (
			"\t.set\tmips3\n"
			"\twait\n"
			"\t.set\tmips0\n"
		);
}

/*
 * Reset the CPU and re-enter Redboot
 */
static void prom_exit(void)
{
	unsigned int i;
	volatile unsigned char * const flg =
		(volatile unsigned char *) (EXCITE_ADDR_FPGA + EXCITE_FPGA_DPR);

	/* Clear the watchdog reset flag, set the reboot flag */
	*flg &= ~0x01;
	*flg |= 0x80;

	for (i = 0; i < 10; i++) {
		*(volatile unsigned char *)  (EXCITE_ADDR_FPGA + EXCITE_FPGA_SYSCTL) = 0x02;
		iob();
		mdelay(1000);
	}

	printk(KERN_NOTICE "Reset failed\n");
	prom_halt();
}

static const char __init *prom_getenv(char *name)
{
	const t_env_var * p;
	for (p = prom_env; p->name != NULL; p++)
		if(strcmp(name, p->name) == 0)
			break;
	return p->val;
}

/*
 * Init routine which accepts the variables from Redboot
 */
void __init prom_init(void)
{
	const struct parmblock * const pb = (struct parmblock *) fw_arg2;

	prom_argc = fw_arg0;
	prom_argv = (const char **) fw_arg1;
	prom_env = &pb->memsize;

	/* Callbacks for halt, restart */
	_machine_restart = (void (*)(char *)) prom_exit;
	_machine_halt = prom_halt;

#ifdef CONFIG_32BIT
	/* copy command line */
	strcpy(arcs_cmdline, prom_argv[1]);
	memsize = simple_strtol(prom_getenv("memsize"), NULL, 16);
	strcpy(modetty, prom_getenv("modetty0"));
#endif /* CONFIG_32BIT */

#ifdef CONFIG_64BIT
#	error 64 bit support not implemented
#endif /* CONFIG_64BIT */
}

/* This is called from free_initmem(), so we need to provide it */
void __init prom_free_prom_memory(void)
{
	/* Nothing to do */
}
