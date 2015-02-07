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
 * PROM interface routines.
 */
#include <linux/types.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/bootmem.h>
#include <linux/ioport.h>
#include <asm/bootinfo.h>
#include <asm/lasat/lasat.h>
#include <asm/cpu.h>

#include "at93c.h"
#include <asm/lasat/eeprom.h>
#include "prom.h"

#define RESET_VECTOR	0xbfc00000
#define PROM_JUMP_TABLE_ENTRY(n) (*((u32 *)(RESET_VECTOR + 0x20) + n))
#define PROM_DISPLAY_ADDR	PROM_JUMP_TABLE_ENTRY(0)
#define PROM_PUTC_ADDR		PROM_JUMP_TABLE_ENTRY(1)
#define PROM_MONITOR_ADDR	PROM_JUMP_TABLE_ENTRY(2)

static void null_prom_display(const char *string, int pos, int clear)
{
}

static void null_prom_monitor(void)
{
}

static void null_prom_putc(char c)
{
}

/* these are functions provided by the bootloader */
static void (*__prom_putc)(char c) = null_prom_putc;

void prom_putchar(char c)
{
	__prom_putc(c);
}

void (*prom_display)(const char *string, int pos, int clear) =
		null_prom_display;
void (*prom_monitor)(void) = null_prom_monitor;

unsigned int lasat_ndelay_divider;

static void setup_prom_vectors(void)
{
	u32 version = *(u32 *)(RESET_VECTOR + 0x90);

	if (version >= 307) {
		prom_display = (void *)PROM_DISPLAY_ADDR;
		__prom_putc = (void *)PROM_PUTC_ADDR;
		prom_monitor = (void *)PROM_MONITOR_ADDR;
	}
	printk(KERN_DEBUG "prom vectors set up\n");
}

static struct at93c_defs at93c_defs[N_MACHTYPES] = {
	{
		.reg		= (void *)AT93C_REG_100,
		.rdata_reg	= (void *)AT93C_RDATA_REG_100,
		.rdata_shift	= AT93C_RDATA_SHIFT_100,
		.wdata_shift	= AT93C_WDATA_SHIFT_100,
		.cs		= AT93C_CS_M_100,
		.clk		= AT93C_CLK_M_100
	}, {
		.reg		= (void *)AT93C_REG_200,
		.rdata_reg	= (void *)AT93C_RDATA_REG_200,
		.rdata_shift	= AT93C_RDATA_SHIFT_200,
		.wdata_shift	= AT93C_WDATA_SHIFT_200,
		.cs		= AT93C_CS_M_200,
		.clk		= AT93C_CLK_M_200
	},
};

void __init prom_init(void)
{
	int argc = fw_arg0;
	char **argv = (char **) fw_arg1;

	setup_prom_vectors();

	if (IS_LASAT_200()) {
		printk(KERN_INFO "LASAT 200 board\n");
		lasat_ndelay_divider = LASAT_200_DIVIDER;
		at93c = &at93c_defs[1];
	} else {
		printk(KERN_INFO "LASAT 100 board\n");
		lasat_ndelay_divider = LASAT_100_DIVIDER;
		at93c = &at93c_defs[0];
	}

	lasat_init_board_info();		/* Read info from EEPROM */

	/* Get the command line */
	if (argc > 0) {
		strncpy(arcs_cmdline, argv[0], CL_SIZE-1);
		arcs_cmdline[CL_SIZE-1] = '\0';
	}

	/* Set the I/O base address */
	set_io_port_base(KSEG1);

	/* Set memory regions */
	ioport_resource.start = 0;
	ioport_resource.end = 0xffffffff;	/* Wrong, fixme.  */

	add_memory_region(0, lasat_board_info.li_memsize, BOOT_MEM_RAM);
}

void __init prom_free_prom_memory(void)
{
}

const char *get_system_type(void)
{
	return lasat_board_info.li_bmstr;
}
