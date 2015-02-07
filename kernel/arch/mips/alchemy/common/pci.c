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
 * BRIEF MODULE DESCRIPTION
 *	Alchemy/AMD Au1x00 PCI support.
 *
 * Copyright 2001-2003, 2007-2008 MontaVista Software Inc.
 * Author: MontaVista Software, Inc. <source@mvista.com>
 *
 * Copyright (C) 2004 by Ralf Baechle (ralf@linux-mips.org)
 *
 *  Support for all devices (greater than 16) added by David Gathright.
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <asm/mach-au1x00/au1000.h>

/* TBD */
static struct resource pci_io_resource = {
	.start	= PCI_IO_START,
	.end	= PCI_IO_END,
	.name	= "PCI IO space",
	.flags	= IORESOURCE_IO
};

static struct resource pci_mem_resource = {
	.start	= PCI_MEM_START,
	.end	= PCI_MEM_END,
	.name	= "PCI memory space",
	.flags	= IORESOURCE_MEM
};

extern struct pci_ops au1x_pci_ops;

static struct pci_controller au1x_controller = {
	.pci_ops	= &au1x_pci_ops,
	.io_resource	= &pci_io_resource,
	.mem_resource	= &pci_mem_resource,
};

#if defined(CONFIG_SOC_AU1500) || defined(CONFIG_SOC_AU1550)
static unsigned long virt_io_addr;
#endif

static int __init au1x_pci_setup(void)
{
	extern void au1x_pci_cfg_init(void);

#if defined(CONFIG_SOC_AU1500) || defined(CONFIG_SOC_AU1550)
	virt_io_addr = (unsigned long)ioremap(Au1500_PCI_IO_START,
			Au1500_PCI_IO_END - Au1500_PCI_IO_START + 1);

	if (!virt_io_addr) {
		printk(KERN_ERR "Unable to ioremap pci space\n");
		return 1;
	}
	au1x_controller.io_map_base = virt_io_addr;

#ifdef CONFIG_DMA_NONCOHERENT
	{
		/*
		 *  Set the NC bit in controller for Au1500 pre-AC silicon
		 */
		u32 prid = read_c0_prid();

		if ((prid & 0xFF000000) == 0x01000000 && prid < 0x01030202) {
			au_writel((1 << 16) | au_readl(Au1500_PCI_CFG),
				  Au1500_PCI_CFG);
			printk(KERN_INFO "Non-coherent PCI accesses enabled\n");
		}
	}
#endif

	set_io_port_base(virt_io_addr);
#endif

	au1x_pci_cfg_init();

	register_pci_controller(&au1x_controller);
	return 0;
}

arch_initcall(au1x_pci_setup);
