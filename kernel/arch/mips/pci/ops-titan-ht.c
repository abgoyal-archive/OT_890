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
 * Copyright 2003 PMC-Sierra
 * Author: Manish Lachwani (lachwani@pmc-sierra.com)
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

#include <linux/types.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <asm/io.h>

#include <asm/titan_dep.h>

static int titan_ht_config_read_dword(struct pci_bus *bus, unsigned int devfn,
	int offset, u32 * val)
{
	volatile uint32_t address;
	int busno;

	busno = bus->number;

	address = (busno << 16) | (devfn << 8) | (offset & 0xfc) | 0x80000000;
	if (busno != 0)
		address |= 1;

	/*
	 * RM9000 HT Errata: Issue back to back HT config
	 * transcations. Issue a BIU sync before and
	 * after the HT cycle
	 */

	*(volatile int32_t *) 0xfb0000f0 |= 0x2;

	udelay(30);

	*(volatile int32_t *) 0xfb0006f8 = address;
	*(val) = *(volatile int32_t *) 0xfb0006fc;

	udelay(30);

	* (volatile int32_t *) 0xfb0000f0 |= 0x2;

	return PCIBIOS_SUCCESSFUL;
}

static int titan_ht_config_read(struct pci_bus *bus, unsigned int devfn,
	int offset, int size, u32 * val)
{
	uint32_t dword;

	titan_ht_config_read_dword(bus, devfn, offset, &dword);

	dword >>= ((offset & 3) << 3);
	dword &= (0xffffffffU >> ((4 - size) << 8));

	return PCIBIOS_SUCCESSFUL;
}

static inline int titan_ht_config_write_dword(struct pci_bus *bus,
	unsigned int devfn, int offset, u32 val)
{
	volatile uint32_t address;
	int busno;

	busno = bus->number;

	address = (busno << 16) | (devfn << 8) | (offset & 0xfc) | 0x80000000;
	if (busno != 0)
		address |= 1;

	*(volatile int32_t *) 0xfb0000f0 |= 0x2;

	udelay(30);

	*(volatile int32_t *) 0xfb0006f8 = address;
	*(volatile int32_t *) 0xfb0006fc = val;

	udelay(30);

	*(volatile int32_t *) 0xfb0000f0 |= 0x2;

	return PCIBIOS_SUCCESSFUL;
}

static int titan_ht_config_write(struct pci_bus *bus, unsigned int devfn,
	int offset, int size, u32 val)
{
	uint32_t val1, val2, mask;

	titan_ht_config_read_dword(bus, devfn, offset, &val2);

	val1 = val << ((offset & 3) << 3);
	mask = ~(0xffffffffU >> ((4 - size) << 8));
	val2 &= ~(mask << ((offset & 3) << 8));

	titan_ht_config_write_dword(bus, devfn, offset, val1 | val2);

	return PCIBIOS_SUCCESSFUL;
}

struct pci_ops titan_ht_pci_ops = {
	.read	= titan_ht_config_read,
	.write	= titan_ht_config_write,
};
