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
 *  RouterBoard 500 specific prom routines
 *
 *  Copyright (C) 2003, Peter Sadik <peter.sadik@idt.com>
 *  Copyright (C) 2005-2006, P.Christeas <p_christ@hol.gr>
 *  Copyright (C) 2007, Gabor Juhos <juhosg@openwrt.org>
 *			Felix Fietkau <nbd@openwrt.org>
 *			Florian Fainelli <florian@openwrt.org>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the
 *  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 *
 */

#include <linux/init.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/console.h>
#include <linux/bootmem.h>
#include <linux/ioport.h>
#include <linux/blkdev.h>

#include <asm/bootinfo.h>
#include <asm/mach-rc32434/ddr.h>
#include <asm/mach-rc32434/prom.h>

unsigned int idt_cpu_freq = 132000000;
EXPORT_SYMBOL(idt_cpu_freq);

static struct resource ddr_reg[] = {
	{
		.name = "ddr-reg",
		.start = DDR0_PHYS_ADDR,
		.end = DDR0_PHYS_ADDR + sizeof(struct ddr_ram),
		.flags = IORESOURCE_MEM,
	}
};

void __init prom_free_prom_memory(void)
{
	/* No prom memory to free */
}

static inline int match_tag(char *arg, const char *tag)
{
	return strncmp(arg, tag, strlen(tag)) == 0;
}

static inline unsigned long tag2ul(char *arg, const char *tag)
{
	char *num;

	num = arg + strlen(tag);
	return simple_strtoul(num, 0, 10);
}

void __init prom_setup_cmdline(void)
{
	char cmd_line[CL_SIZE];
	char *cp, *board;
	int prom_argc;
	char **prom_argv, **prom_envp;
	int i;

	prom_argc = fw_arg0;
	prom_argv = (char **) fw_arg1;
	prom_envp = (char **) fw_arg2;

	cp = cmd_line;
		/* Note: it is common that parameters start
		 * at argv[1] and not argv[0],
		 * however, our elf loader starts at [0] */
	for (i = 0; i < prom_argc; i++) {
		if (match_tag(prom_argv[i], FREQ_TAG)) {
			idt_cpu_freq = tag2ul(prom_argv[i], FREQ_TAG);
			continue;
		}
#ifdef IGNORE_CMDLINE_MEM
		/* parses out the "mem=xx" arg */
		if (match_tag(prom_argv[i], MEM_TAG))
			continue;
#endif
		if (i > 0)
			*(cp++) = ' ';
		if (match_tag(prom_argv[i], BOARD_TAG)) {
			board = prom_argv[i] + strlen(BOARD_TAG);

			if (match_tag(board, BOARD_RB532A))
				mips_machtype = MACH_MIKROTIK_RB532A;
			else
				mips_machtype = MACH_MIKROTIK_RB532;
		}

		strcpy(cp, prom_argv[i]);
		cp += strlen(prom_argv[i]);
	}
	*(cp++) = ' ';

	i = strlen(arcs_cmdline);
	if (i > 0) {
		*(cp++) = ' ';
		strcpy(cp, arcs_cmdline);
		cp += strlen(arcs_cmdline);
	}
	cmd_line[CL_SIZE-1] = '\0';

	strcpy(arcs_cmdline, cmd_line);
}

void __init prom_init(void)
{
	struct ddr_ram __iomem *ddr;
	phys_t memsize;
	phys_t ddrbase;

	ddr = ioremap_nocache(ddr_reg[0].start,
			ddr_reg[0].end - ddr_reg[0].start);

	if (!ddr) {
		printk(KERN_ERR "Unable to remap DDR register\n");
		return;
	}

	ddrbase = (phys_t)&ddr->ddrbase;
	memsize = (phys_t)&ddr->ddrmask;
	memsize = 0 - memsize;

	prom_setup_cmdline();

	/* give all RAM to boot allocator,
	 * except for the first 0x400 and the last 0x200 bytes */
	add_memory_region(ddrbase + 0x400, memsize - 0x600, BOOT_MEM_RAM);
}
