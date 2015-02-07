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
 *  linux/arch/m68knommu/kernel/setup.c
 *
 *  Copyright (C) 1999-2007  Greg Ungerer (gerg@snapgear.com)
 *  Copyright (C) 1998,1999  D. Jeff Dionne <jeff@uClinux.org>
 *  Copyleft  ()) 2000       James D. Schettine {james@telos-systems.com}
 *  Copyright (C) 1998       Kenneth Albanowski <kjahds@kjahds.com>
 *  Copyright (C) 1995       Hamish Macdonald
 *  Copyright (C) 2000       Lineo Inc. (www.lineo.com)
 *  Copyright (C) 2001 	     Lineo, Inc. <www.lineo.com>
 *
 *  68VZ328 Fixes/support    Evan Stawnyczy <e@lineo.ca>
 */

/*
 * This file handles the architecture-dependent parts of system setup
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/fb.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/console.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/bootmem.h>
#include <linux/seq_file.h>
#include <linux/init.h>

#include <asm/setup.h>
#include <asm/irq.h>
#include <asm/machdep.h>
#include <asm/pgtable.h>

unsigned long memory_start;
unsigned long memory_end;

EXPORT_SYMBOL(memory_start);
EXPORT_SYMBOL(memory_end);

char __initdata command_line[COMMAND_LINE_SIZE];

/* machine dependent timer functions */
void (*mach_gettod)(int*, int*, int*, int*, int*, int*);
int (*mach_set_clock_mmss)(unsigned long);

/* machine dependent reboot functions */
void (*mach_reset)(void);
void (*mach_halt)(void);
void (*mach_power_off)(void);


#ifdef CONFIG_M68000
	#define CPU "MC68000"
#endif
#ifdef CONFIG_M68328
	#define CPU "MC68328"
#endif
#ifdef CONFIG_M68EZ328
	#define CPU "MC68EZ328"
#endif
#ifdef CONFIG_M68VZ328
	#define CPU "MC68VZ328"
#endif
#ifdef CONFIG_M68360
	#define CPU "MC68360"
#endif
#if defined(CONFIG_M5206)
	#define	CPU "COLDFIRE(m5206)"
#endif
#if defined(CONFIG_M5206e)
	#define	CPU "COLDFIRE(m5206e)"
#endif
#if defined(CONFIG_M520x)
	#define CPU "COLDFIRE(m520x)"
#endif
#if defined(CONFIG_M523x)
	#define CPU "COLDFIRE(m523x)"
#endif
#if defined(CONFIG_M5249)
	#define CPU "COLDFIRE(m5249)"
#endif
#if defined(CONFIG_M5271)
	#define CPU "COLDFIRE(m5270/5271)"
#endif
#if defined(CONFIG_M5272)
	#define CPU "COLDFIRE(m5272)"
#endif
#if defined(CONFIG_M5275)
	#define CPU "COLDFIRE(m5274/5275)"
#endif
#if defined(CONFIG_M528x)
	#define CPU "COLDFIRE(m5280/5282)"
#endif
#if defined(CONFIG_M5307)
	#define	CPU "COLDFIRE(m5307)"
#endif
#if defined(CONFIG_M532x)
	#define	CPU "COLDFIRE(m532x)"
#endif
#if defined(CONFIG_M5407)
	#define	CPU "COLDFIRE(m5407)"
#endif
#ifndef CPU
	#define	CPU "UNKNOWN"
#endif

extern int _stext, _etext, _sdata, _edata, _sbss, _ebss, _end;
extern int _ramstart, _ramend;

void __init setup_arch(char **cmdline_p)
{
	int bootmap_size;

	memory_start = PAGE_ALIGN(_ramstart);
	memory_end = _ramend;

	init_mm.start_code = (unsigned long) &_stext;
	init_mm.end_code = (unsigned long) &_etext;
	init_mm.end_data = (unsigned long) &_edata;
	init_mm.brk = (unsigned long) 0;

	config_BSP(&command_line[0], sizeof(command_line));

#if defined(CONFIG_BOOTPARAM)
	strncpy(&command_line[0], CONFIG_BOOTPARAM_STRING, sizeof(command_line));
	command_line[sizeof(command_line) - 1] = 0;
#endif

	printk(KERN_INFO "\x0F\r\n\nuClinux/" CPU "\n");

#ifdef CONFIG_UCDIMM
	printk(KERN_INFO "uCdimm by Lineo, Inc. <www.lineo.com>\n");
#endif
#ifdef CONFIG_M68VZ328
	printk(KERN_INFO "M68VZ328 support by Evan Stawnyczy <e@lineo.ca>\n");
#endif
#ifdef CONFIG_COLDFIRE
	printk(KERN_INFO "COLDFIRE port done by Greg Ungerer, gerg@snapgear.com\n");
#ifdef CONFIG_M5307
	printk(KERN_INFO "Modified for M5307 by Dave Miller, dmiller@intellistor.com\n");
#endif
#ifdef CONFIG_ELITE
	printk(KERN_INFO "Modified for M5206eLITE by Rob Scott, rscott@mtrob.fdns.net\n");
#endif
#endif
	printk(KERN_INFO "Flat model support (C) 1998,1999 Kenneth Albanowski, D. Jeff Dionne\n");

#if defined( CONFIG_PILOT ) && defined( CONFIG_M68328 )
	printk(KERN_INFO "TRG SuperPilot FLASH card support <info@trgnet.com>\n");
#endif
#if defined( CONFIG_PILOT ) && defined( CONFIG_M68EZ328 )
	printk(KERN_INFO "PalmV support by Lineo Inc. <jeff@uclinux.com>\n");
#endif
#if defined (CONFIG_M68360)
	printk(KERN_INFO "QUICC port done by SED Systems <hamilton@sedsystems.ca>,\n");
	printk(KERN_INFO "based on 2.0.38 port by Lineo Inc. <mleslie@lineo.com>.\n");
#endif
#ifdef CONFIG_DRAGEN2
	printk(KERN_INFO "DragonEngine II board support by Georges Menie\n");
#endif
#ifdef CONFIG_M5235EVB
	printk(KERN_INFO "Motorola M5235EVB support (C)2005 Syn-tech Systems, Inc. (Jate Sujjavanich)\n");
#endif

#ifdef DEBUG
	printk(KERN_DEBUG "KERNEL -> TEXT=0x%06x-0x%06x DATA=0x%06x-0x%06x "
		"BSS=0x%06x-0x%06x\n", (int) &_stext, (int) &_etext,
		(int) &_sdata, (int) &_edata,
		(int) &_sbss, (int) &_ebss);
	printk(KERN_DEBUG "MEMORY -> ROMFS=0x%06x-0x%06x MEM=0x%06x-0x%06x\n ",
		(int) &_ebss, (int) memory_start,
		(int) memory_start, (int) memory_end);
#endif

	/* Keep a copy of command line */
	*cmdline_p = &command_line[0];
	memcpy(boot_command_line, command_line, COMMAND_LINE_SIZE);
	boot_command_line[COMMAND_LINE_SIZE-1] = 0;

#ifdef DEBUG
	if (strlen(*cmdline_p))
		printk(KERN_DEBUG "Command line: '%s'\n", *cmdline_p);
#endif

#if defined(CONFIG_FRAMEBUFFER_CONSOLE) && defined(CONFIG_DUMMY_CONSOLE)
	conswitchp = &dummy_con;
#endif

	/*
	 * Give all the memory to the bootmap allocator, tell it to put the
	 * boot mem_map at the start of memory.
	 */
	bootmap_size = init_bootmem_node(
			NODE_DATA(0),
			memory_start >> PAGE_SHIFT, /* map goes here */
			PAGE_OFFSET >> PAGE_SHIFT,	/* 0 on coldfire */
			memory_end >> PAGE_SHIFT);
	/*
	 * Free the usable memory, we have to make sure we do not free
	 * the bootmem bitmap so we then reserve it after freeing it :-)
	 */
	free_bootmem(memory_start, memory_end - memory_start);
	reserve_bootmem(memory_start, bootmap_size, BOOTMEM_DEFAULT);

	/*
	 * Get kmalloc into gear.
	 */
	paging_init();
}

/*
 *	Get CPU information for use by the procfs.
 */
static int show_cpuinfo(struct seq_file *m, void *v)
{
	char *cpu, *mmu, *fpu;
	u_long clockfreq;

	cpu = CPU;
	mmu = "none";
	fpu = "none";

#ifdef CONFIG_COLDFIRE
	clockfreq = (loops_per_jiffy * HZ) * 3;
#else
	clockfreq = (loops_per_jiffy * HZ) * 16;
#endif

	seq_printf(m, "CPU:\t\t%s\n"
		      "MMU:\t\t%s\n"
		      "FPU:\t\t%s\n"
		      "Clocking:\t%lu.%1luMHz\n"
		      "BogoMips:\t%lu.%02lu\n"
		      "Calibration:\t%lu loops\n",
		      cpu, mmu, fpu,
		      clockfreq / 1000000,
		      (clockfreq / 100000) % 10,
		      (loops_per_jiffy * HZ) / 500000,
		      ((loops_per_jiffy * HZ) / 5000) % 100,
		      (loops_per_jiffy * HZ));

	return 0;
}

static void *c_start(struct seq_file *m, loff_t *pos)
{
	return *pos < NR_CPUS ? ((void *) 0x12345678) : NULL;
}

static void *c_next(struct seq_file *m, void *v, loff_t *pos)
{
	++*pos;
	return c_start(m, pos);
}

static void c_stop(struct seq_file *m, void *v)
{
}

const struct seq_operations cpuinfo_op = {
	.start	= c_start,
	.next	= c_next,
	.stop	= c_stop,
	.show	= show_cpuinfo,
};

