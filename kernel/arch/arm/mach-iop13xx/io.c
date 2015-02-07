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
 * iop13xx custom ioremap implementation
 * Copyright (c) 2005-2006, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>
#include <mach/hardware.h>

void * __iomem __iop13xx_io(unsigned long io_addr)
{
	void __iomem * io_virt;

	switch (io_addr) {
	case IOP13XX_PCIE_LOWER_IO_PA ... IOP13XX_PCIE_UPPER_IO_PA:
		io_virt = (void *) IOP13XX_PCIE_IO_PHYS_TO_VIRT(io_addr);
		break;
	case IOP13XX_PCIX_LOWER_IO_PA ... IOP13XX_PCIX_UPPER_IO_PA:
		io_virt = (void *) IOP13XX_PCIX_IO_PHYS_TO_VIRT(io_addr);
		break;
	default:
		BUG();
	}

	return io_virt;
}
EXPORT_SYMBOL(__iop13xx_io);

void * __iomem __iop13xx_ioremap(unsigned long cookie, size_t size,
	unsigned int mtype)
{
	void __iomem * retval;

	switch (cookie) {
	case IOP13XX_PCIX_LOWER_MEM_RA ... IOP13XX_PCIX_UPPER_MEM_RA:
		if (unlikely(!iop13xx_atux_mem_base))
			retval = NULL;
		else
			retval = (void *)(iop13xx_atux_mem_base +
			         (cookie - IOP13XX_PCIX_LOWER_MEM_RA));
		break;
	case IOP13XX_PCIE_LOWER_MEM_RA ... IOP13XX_PCIE_UPPER_MEM_RA:
		if (unlikely(!iop13xx_atue_mem_base))
			retval = NULL;
		else
			retval = (void *)(iop13xx_atue_mem_base +
			         (cookie - IOP13XX_PCIE_LOWER_MEM_RA));
		break;
	case IOP13XX_PBI_LOWER_MEM_RA ... IOP13XX_PBI_UPPER_MEM_RA:
		retval = __arm_ioremap(IOP13XX_PBI_LOWER_MEM_PA +
				       (cookie - IOP13XX_PBI_LOWER_MEM_RA),
				       size, mtype);
		break;
	case IOP13XX_PCIE_LOWER_IO_PA ... IOP13XX_PCIE_UPPER_IO_PA:
		retval = (void *) IOP13XX_PCIE_IO_PHYS_TO_VIRT(cookie);
		break;
	case IOP13XX_PCIX_LOWER_IO_PA ... IOP13XX_PCIX_UPPER_IO_PA:
		retval = (void *) IOP13XX_PCIX_IO_PHYS_TO_VIRT(cookie);
		break;
	case IOP13XX_PMMR_PHYS_MEM_BASE ... IOP13XX_PMMR_UPPER_MEM_PA:
		retval = (void *) IOP13XX_PMMR_PHYS_TO_VIRT(cookie);
		break;
	default:
		retval = __arm_ioremap(cookie, size, mtype);
	}

	return retval;
}
EXPORT_SYMBOL(__iop13xx_ioremap);

void __iop13xx_iounmap(void __iomem *addr)
{
	extern void __iounmap(volatile void __iomem *addr);

	if (iop13xx_atue_mem_base)
		if (addr >= (void __iomem *) iop13xx_atue_mem_base &&
	 	    addr < (void __iomem *) (iop13xx_atue_mem_base +
	 	    			     iop13xx_atue_mem_size))
		    goto skip;

	if (iop13xx_atux_mem_base)
		if (addr >= (void __iomem *) iop13xx_atux_mem_base &&
	 	    addr < (void __iomem *) (iop13xx_atux_mem_base +
	 	    			     iop13xx_atux_mem_size))
		    goto skip;

	switch ((u32) addr) {
	case IOP13XX_PCIE_LOWER_IO_VA ... IOP13XX_PCIE_UPPER_IO_VA:
	case IOP13XX_PCIX_LOWER_IO_VA ... IOP13XX_PCIX_UPPER_IO_VA:
	case IOP13XX_PMMR_VIRT_MEM_BASE ... IOP13XX_PMMR_UPPER_MEM_VA:
		goto skip;
	}
	__iounmap(addr);

skip:
	return;
}
EXPORT_SYMBOL(__iop13xx_iounmap);
