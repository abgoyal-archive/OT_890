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
 * vSMPowered(tm) systems specific initialization
 * Copyright (C) 2005 ScaleMP Inc.
 *
 * Use of this code is subject to the terms and conditions of the
 * GNU general public license version 2. See "COPYING" or
 * http://www.gnu.org/licenses/gpl.html
 *
 * Ravikiran Thirumalai <kiran@scalemp.com>,
 * Shai Fultheim <shai@scalemp.com>
 * Paravirt ops integration: Glauber de Oliveira Costa <gcosta@redhat.com>,
 *			     Ravikiran Thirumalai <kiran@scalemp.com>
 */

#include <linux/init.h>
#include <linux/pci_ids.h>
#include <linux/pci_regs.h>

#include <asm/apic.h>
#include <asm/pci-direct.h>
#include <asm/io.h>
#include <asm/paravirt.h>
#include <asm/setup.h>

#if defined CONFIG_PCI && defined CONFIG_PARAVIRT
/*
 * Interrupt control on vSMPowered systems:
 * ~AC is a shadow of IF.  If IF is 'on' AC should be 'off'
 * and vice versa.
 */

static unsigned long vsmp_save_fl(void)
{
	unsigned long flags = native_save_fl();

	if (!(flags & X86_EFLAGS_IF) || (flags & X86_EFLAGS_AC))
		flags &= ~X86_EFLAGS_IF;
	return flags;
}

static void vsmp_restore_fl(unsigned long flags)
{
	if (flags & X86_EFLAGS_IF)
		flags &= ~X86_EFLAGS_AC;
	else
		flags |= X86_EFLAGS_AC;
	native_restore_fl(flags);
}

static void vsmp_irq_disable(void)
{
	unsigned long flags = native_save_fl();

	native_restore_fl((flags & ~X86_EFLAGS_IF) | X86_EFLAGS_AC);
}

static void vsmp_irq_enable(void)
{
	unsigned long flags = native_save_fl();

	native_restore_fl((flags | X86_EFLAGS_IF) & (~X86_EFLAGS_AC));
}

static unsigned __init_or_module vsmp_patch(u8 type, u16 clobbers, void *ibuf,
				  unsigned long addr, unsigned len)
{
	switch (type) {
	case PARAVIRT_PATCH(pv_irq_ops.irq_enable):
	case PARAVIRT_PATCH(pv_irq_ops.irq_disable):
	case PARAVIRT_PATCH(pv_irq_ops.save_fl):
	case PARAVIRT_PATCH(pv_irq_ops.restore_fl):
		return paravirt_patch_default(type, clobbers, ibuf, addr, len);
	default:
		return native_patch(type, clobbers, ibuf, addr, len);
	}

}

static void __init set_vsmp_pv_ops(void)
{
	void __iomem *address;
	unsigned int cap, ctl, cfg;

	/* set vSMP magic bits to indicate vSMP capable kernel */
	cfg = read_pci_config(0, 0x1f, 0, PCI_BASE_ADDRESS_0);
	address = early_ioremap(cfg, 8);
	cap = readl(address);
	ctl = readl(address + 4);
	printk(KERN_INFO "vSMP CTL: capabilities:0x%08x  control:0x%08x\n",
	       cap, ctl);
	if (cap & ctl & (1 << 4)) {
		/* Setup irq ops and turn on vSMP  IRQ fastpath handling */
		pv_irq_ops.irq_disable = vsmp_irq_disable;
		pv_irq_ops.irq_enable  = vsmp_irq_enable;
		pv_irq_ops.save_fl  = vsmp_save_fl;
		pv_irq_ops.restore_fl  = vsmp_restore_fl;
		pv_init_ops.patch = vsmp_patch;

		ctl &= ~(1 << 4);
		writel(ctl, address + 4);
		ctl = readl(address + 4);
		printk(KERN_INFO "vSMP CTL: control set to:0x%08x\n", ctl);
	}

	early_iounmap(address, 8);
}
#else
static void __init set_vsmp_pv_ops(void)
{
}
#endif

#ifdef CONFIG_PCI
static int is_vsmp = -1;

static void __init detect_vsmp_box(void)
{
	is_vsmp = 0;

	if (!early_pci_allowed())
		return;

	/* Check if we are running on a ScaleMP vSMPowered box */
	if (read_pci_config(0, 0x1f, 0, PCI_VENDOR_ID) ==
	     (PCI_VENDOR_ID_SCALEMP | (PCI_DEVICE_ID_SCALEMP_VSMP_CTL << 16)))
		is_vsmp = 1;
}

int is_vsmp_box(void)
{
	if (is_vsmp != -1)
		return is_vsmp;
	else {
		WARN_ON_ONCE(1);
		return 0;
	}
}
#else
static void __init detect_vsmp_box(void)
{
}
int is_vsmp_box(void)
{
	return 0;
}
#endif

void __init vsmp_init(void)
{
	detect_vsmp_box();
	if (!is_vsmp_box())
		return;

	set_vsmp_pv_ops();
	return;
}