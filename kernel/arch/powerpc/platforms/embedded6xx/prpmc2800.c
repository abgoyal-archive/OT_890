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
 * Board setup routines for the Motorola PrPMC2800
 *
 * Author: Dale Farnsworth <dale@farnsworth.org>
 *
 * 2007 (c) MontaVista, Software, Inc.  This file is licensed under
 * the terms of the GNU General Public License version 2.  This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/seq_file.h>

#include <asm/machdep.h>
#include <asm/prom.h>
#include <asm/system.h>
#include <asm/time.h>

#include <mm/mmu_decl.h>

#include <sysdev/mv64x60.h>

#define MV64x60_MPP_CNTL_0	0x0000
#define MV64x60_MPP_CNTL_2	0x0008

#define MV64x60_GPP_IO_CNTL	0x0000
#define MV64x60_GPP_LEVEL_CNTL	0x0010
#define MV64x60_GPP_VALUE_SET	0x0018

#define PLATFORM_NAME_MAX	32

static char prpmc2800_platform_name[PLATFORM_NAME_MAX];

static void __iomem *mv64x60_mpp_reg_base;
static void __iomem *mv64x60_gpp_reg_base;

static void __init prpmc2800_setup_arch(void)
{
	struct device_node *np;
	phys_addr_t paddr;
	const unsigned int *reg;

	/*
	 * ioremap mpp and gpp registers in case they are later
	 * needed by prpmc2800_reset_board().
	 */
	np = of_find_compatible_node(NULL, NULL, "marvell,mv64360-mpp");
	reg = of_get_property(np, "reg", NULL);
	paddr = of_translate_address(np, reg);
	of_node_put(np);
	mv64x60_mpp_reg_base = ioremap(paddr, reg[1]);

	np = of_find_compatible_node(NULL, NULL, "marvell,mv64360-gpp");
	reg = of_get_property(np, "reg", NULL);
	paddr = of_translate_address(np, reg);
	of_node_put(np);
	mv64x60_gpp_reg_base = ioremap(paddr, reg[1]);

#ifdef CONFIG_PCI
	mv64x60_pci_init();
#endif

	printk("Motorola %s\n", prpmc2800_platform_name);
}

static void prpmc2800_reset_board(void)
{
	u32 temp;

	local_irq_disable();

	temp = in_le32(mv64x60_mpp_reg_base + MV64x60_MPP_CNTL_0);
	temp &= 0xFFFF0FFF;
	out_le32(mv64x60_mpp_reg_base + MV64x60_MPP_CNTL_0, temp);

	temp = in_le32(mv64x60_gpp_reg_base + MV64x60_GPP_LEVEL_CNTL);
	temp |= 0x00000004;
	out_le32(mv64x60_gpp_reg_base + MV64x60_GPP_LEVEL_CNTL, temp);

	temp = in_le32(mv64x60_gpp_reg_base + MV64x60_GPP_IO_CNTL);
	temp |= 0x00000004;
	out_le32(mv64x60_gpp_reg_base + MV64x60_GPP_IO_CNTL, temp);

	temp = in_le32(mv64x60_mpp_reg_base + MV64x60_MPP_CNTL_2);
	temp &= 0xFFFF0FFF;
	out_le32(mv64x60_mpp_reg_base + MV64x60_MPP_CNTL_2, temp);

	temp = in_le32(mv64x60_gpp_reg_base + MV64x60_GPP_LEVEL_CNTL);
	temp |= 0x00080000;
	out_le32(mv64x60_gpp_reg_base + MV64x60_GPP_LEVEL_CNTL, temp);

	temp = in_le32(mv64x60_gpp_reg_base + MV64x60_GPP_IO_CNTL);
	temp |= 0x00080000;
	out_le32(mv64x60_gpp_reg_base + MV64x60_GPP_IO_CNTL, temp);

	out_le32(mv64x60_gpp_reg_base + MV64x60_GPP_VALUE_SET, 0x00080004);
}

static void prpmc2800_restart(char *cmd)
{
	volatile ulong i = 10000000;

	prpmc2800_reset_board();

	while (i-- > 0);
	panic("restart failed\n");
}

#ifdef CONFIG_NOT_COHERENT_CACHE
#define PPRPM2800_COHERENCY_SETTING "off"
#else
#define PPRPM2800_COHERENCY_SETTING "on"
#endif

void prpmc2800_show_cpuinfo(struct seq_file *m)
{
	seq_printf(m, "Vendor\t\t: Motorola\n");
	seq_printf(m, "coherency\t: %s\n", PPRPM2800_COHERENCY_SETTING);
}

/*
 * Called very early, device-tree isn't unflattened
 */
static int __init prpmc2800_probe(void)
{
	unsigned long root = of_get_flat_dt_root();
	unsigned long len = PLATFORM_NAME_MAX;
	void *m;

	if (!of_flat_dt_is_compatible(root, "motorola,PrPMC2800"))
		return 0;

	/* Update ppc_md.name with name from dt */
	m = of_get_flat_dt_prop(root, "model", &len);
	if (m)
		strncpy(prpmc2800_platform_name, m,
			min((int)len, PLATFORM_NAME_MAX - 1));

	_set_L2CR(_get_L2CR() | L2CR_L2E);
	return 1;
}

define_machine(prpmc2800){
	.name			= prpmc2800_platform_name,
	.probe			= prpmc2800_probe,
	.setup_arch		= prpmc2800_setup_arch,
	.init_early		= mv64x60_init_early,
	.show_cpuinfo		= prpmc2800_show_cpuinfo,
	.init_IRQ		= mv64x60_init_irq,
	.get_irq		= mv64x60_get_irq,
	.restart		= prpmc2800_restart,
	.calibrate_decr		= generic_calibrate_decr,
};
