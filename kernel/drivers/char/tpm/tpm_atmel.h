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
 * Copyright (C) 2005 IBM Corporation
 *
 * Authors:
 * Kylene Hall <kjhall@us.ibm.com>
 *
 * Maintained by: <tpmdd-devel@lists.sourceforge.net>
 *
 * Device driver for TCG/TCPA TPM (trusted platform module).
 * Specifications at www.trustedcomputinggroup.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 *
 * These difference are required on power because the device must be
 * discovered through the device tree and iomap must be used to get
 * around the need for holes in the io_page_mask.  This does not happen
 * automatically because the tpm is not a normal pci device and lives
 * under the root node.
 *
 */

#ifdef CONFIG_PPC64

#include <asm/prom.h>

#define atmel_getb(chip, offset) readb(chip->vendor->iobase + offset);
#define atmel_putb(val, chip, offset) writeb(val, chip->vendor->iobase + offset)
#define atmel_request_region request_mem_region
#define atmel_release_region release_mem_region

static inline void atmel_put_base_addr(void __iomem *iobase)
{
	iounmap(iobase);
}

static void __iomem * atmel_get_base_addr(unsigned long *base, int *region_size)
{
	struct device_node *dn;
	unsigned long address, size;
	const unsigned int *reg;
	int reglen;
	int naddrc;
	int nsizec;

	dn = of_find_node_by_name(NULL, "tpm");

	if (!dn)
		return NULL;

	if (!of_device_is_compatible(dn, "AT97SC3201")) {
		of_node_put(dn);
		return NULL;
	}

	reg = of_get_property(dn, "reg", &reglen);
	naddrc = of_n_addr_cells(dn);
	nsizec = of_n_size_cells(dn);

	of_node_put(dn);


	if (naddrc == 2)
		address = ((unsigned long) reg[0] << 32) | reg[1];
	else
		address = reg[0];

	if (nsizec == 2)
		size =
		    ((unsigned long) reg[naddrc] << 32) | reg[naddrc + 1];
	else
		size = reg[naddrc];

	*base = address;
	*region_size = size;
	return ioremap(*base, *region_size);
}
#else
#define atmel_getb(chip, offset) inb(chip->vendor->base + offset)
#define atmel_putb(val, chip, offset) outb(val, chip->vendor->base + offset)
#define atmel_request_region request_region
#define atmel_release_region release_region
/* Atmel definitions */
enum tpm_atmel_addr {
	TPM_ATMEL_BASE_ADDR_LO = 0x08,
	TPM_ATMEL_BASE_ADDR_HI = 0x09
};

/* Verify this is a 1.1 Atmel TPM */
static int atmel_verify_tpm11(void)
{

	/* verify that it is an Atmel part */
	if (tpm_read_index(TPM_ADDR, 4) != 'A' ||
	    tpm_read_index(TPM_ADDR, 5) != 'T' ||
	    tpm_read_index(TPM_ADDR, 6) != 'M' ||
	    tpm_read_index(TPM_ADDR, 7) != 'L')
		return 1;

	/* query chip for its version number */
	if (tpm_read_index(TPM_ADDR, 0x00) != 1 ||
	    tpm_read_index(TPM_ADDR, 0x01) != 1)
		return 1;

	/* This is an atmel supported part */
	return 0;
}

static inline void atmel_put_base_addr(void __iomem *iobase)
{
}

/* Determine where to talk to device */
static void __iomem * atmel_get_base_addr(unsigned long *base, int *region_size)
{
	int lo, hi;

	if (atmel_verify_tpm11() != 0)
		return NULL;

	lo = tpm_read_index(TPM_ADDR, TPM_ATMEL_BASE_ADDR_LO);
	hi = tpm_read_index(TPM_ADDR, TPM_ATMEL_BASE_ADDR_HI);

	*base = (hi << 8) | lo;
	*region_size = 2;

	return ioport_map(*base, *region_size);
}
#endif
