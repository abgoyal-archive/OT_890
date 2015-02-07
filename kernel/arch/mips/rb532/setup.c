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
 * setup.c - boot time setup code
 */

#include <linux/init.h>

#include <asm/bootinfo.h>
#include <asm/reboot.h>
#include <asm/time.h>
#include <linux/ioport.h>

#include <asm/mach-rc32434/rb.h>
#include <asm/mach-rc32434/pci.h>

struct pci_reg __iomem *pci_reg;
EXPORT_SYMBOL(pci_reg);

static struct resource pci0_res[] = {
	{
		.name = "pci_reg0",
		.start = PCI0_BASE_ADDR,
		.end = PCI0_BASE_ADDR + sizeof(struct pci_reg),
		.flags = IORESOURCE_MEM,
	}
};

static void rb_machine_restart(char *command)
{
	/* just jump to the reset vector */
	writel(0x80000001, IDT434_REG_BASE + RST);
	((void (*)(void)) KSEG1ADDR(0x1FC00000u))();
}

static void rb_machine_halt(void)
{
	for (;;)
		continue;
}

void __init plat_mem_setup(void)
{
	u32 val;

	_machine_restart = rb_machine_restart;
	_machine_halt = rb_machine_halt;
	pm_power_off = rb_machine_halt;

	set_io_port_base(KSEG1);

	pci_reg = ioremap_nocache(pci0_res[0].start,
				pci0_res[0].end - pci0_res[0].start);
	if (!pci_reg) {
		printk(KERN_ERR "Could not remap PCI registers\n");
		return;
	}

	val = __raw_readl(&pci_reg->pcic);
	val &= 0xFFFFFF7;
	__raw_writel(val, (void *)&pci_reg->pcic);

#ifdef CONFIG_PCI
	/* Enable PCI interrupts in EPLD Mask register */
	*epld_mask = 0x0;
	*(epld_mask + 1) = 0x0;
#endif
	write_c0_wired(0);
}

const char *get_system_type(void)
{
	switch (mips_machtype) {
	case MACH_MIKROTIK_RB532A:
		return "Mikrotik RB532A";
		break;
	default:
		return "Mikrotik RB532";
		break;
	}
}
