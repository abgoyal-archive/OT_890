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
 * Copyright (C) 2002  David McCullough <davidm@snapgear.com>
 * Copyright (C) 2001  Ian da Silva, Jeremy Siegel
 * Based largely on io_se.c.
 *
 * I/O routine for Hitachi 7751 SolutionEngine.
 *
 * Initial version only to support LAN access; some
 * placeholder code from io_se.c left in with the
 * expectation of later SuperIO and PCMCIA access.
 */
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <asm/io.h>
#include <asm/addrspace.h>

#ifdef CONFIG_SH_SECUREEDGE5410
unsigned short secureedge5410_ioport;
#endif

static inline volatile __u16 *port2adr(unsigned int port)
{
	maybebadio((unsigned long)port);
	return (volatile __u16*)port;
}

/*
 * General outline: remap really low stuff [eventually] to SuperIO,
 * stuff in PCI IO space (at or above window at pci.h:PCIBIOS_MIN_IO)
 * is mapped through the PCI IO window.  Stuff with high bits (PXSEG)
 * should be way beyond the window, and is used  w/o translation for
 * compatibility.
 */
unsigned char snapgear_inb(unsigned long port)
{
	if (PXSEG(port))
		return *(volatile unsigned char *)port;
	else if (is_pci_ioaddr(port))
		return *(volatile unsigned char *)pci_ioaddr(port);
	else
		return (*port2adr(port)) & 0xff;
}

unsigned char snapgear_inb_p(unsigned long port)
{
	unsigned char v;

	if (PXSEG(port))
		v = *(volatile unsigned char *)port;
	else if (is_pci_ioaddr(port))
		v = *(volatile unsigned char *)pci_ioaddr(port);
	else
		v = (*port2adr(port))&0xff;
	ctrl_delay();
	return v;
}

unsigned short snapgear_inw(unsigned long port)
{
	if (PXSEG(port))
		return *(volatile unsigned short *)port;
	else if (is_pci_ioaddr(port))
		return *(volatile unsigned short *)pci_ioaddr(port);
	else if (port >= 0x2000)
		return *port2adr(port);
	else
		maybebadio(port);
	return 0;
}

unsigned int snapgear_inl(unsigned long port)
{
	if (PXSEG(port))
		return *(volatile unsigned long *)port;
	else if (is_pci_ioaddr(port))
		return *(volatile unsigned int *)pci_ioaddr(port);
	else if (port >= 0x2000)
		return *port2adr(port);
	else
		maybebadio(port);
	return 0;
}

void snapgear_outb(unsigned char value, unsigned long port)
{

	if (PXSEG(port))
		*(volatile unsigned char *)port = value;
	else if (is_pci_ioaddr(port))
		*((unsigned char*)pci_ioaddr(port)) = value;
	else
		*(port2adr(port)) = value;
}

void snapgear_outb_p(unsigned char value, unsigned long port)
{
	if (PXSEG(port))
		*(volatile unsigned char *)port = value;
	else if (is_pci_ioaddr(port))
		*((unsigned char*)pci_ioaddr(port)) = value;
	else
		*(port2adr(port)) = value;
	ctrl_delay();
}

void snapgear_outw(unsigned short value, unsigned long port)
{
	if (PXSEG(port))
		*(volatile unsigned short *)port = value;
	else if (is_pci_ioaddr(port))
		*((unsigned short *)pci_ioaddr(port)) = value;
	else if (port >= 0x2000)
		*port2adr(port) = value;
	else
		maybebadio(port);
}

void snapgear_outl(unsigned int value, unsigned long port)
{
	if (PXSEG(port))
		*(volatile unsigned long *)port = value;
	else if (is_pci_ioaddr(port))
		*((unsigned long*)pci_ioaddr(port)) = value;
	else
		maybebadio(port);
}

void snapgear_insl(unsigned long port, void *addr, unsigned long count)
{
	maybebadio(port);
}

void snapgear_outsl(unsigned long port, const void *addr, unsigned long count)
{
	maybebadio(port);
}
