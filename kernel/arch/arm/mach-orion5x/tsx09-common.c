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
 * QNAP TS-x09 Boards common functions
 *
 * Maintainers: Lennert Buytenhek <buytenh@marvell.com>
 *		Byron Bradley <byron.bbradley@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/mv643xx_eth.h>
#include <linux/timex.h>
#include <linux/serial_reg.h>
#include "tsx09-common.h"
#include "common.h"

/*****************************************************************************
 * QNAP TS-x09 specific power off method via UART1-attached PIC
 ****************************************************************************/

#define UART1_REG(x)	(UART1_VIRT_BASE + ((UART_##x) << 2))

void qnap_tsx09_power_off(void)
{
	/* 19200 baud divisor */
	const unsigned divisor = ((orion5x_tclk + (8 * 19200)) / (16 * 19200));

	pr_info("%s: triggering power-off...\n", __func__);

	/* hijack uart1 and reset into sane state (19200,8n1) */
	writel(0x83, UART1_REG(LCR));
	writel(divisor & 0xff, UART1_REG(DLL));
	writel((divisor >> 8) & 0xff, UART1_REG(DLM));
	writel(0x03, UART1_REG(LCR));
	writel(0x00, UART1_REG(IER));
	writel(0x00, UART1_REG(FCR));
	writel(0x00, UART1_REG(MCR));

	/* send the power-off command 'A' to PIC */
	writel('A', UART1_REG(TX));
}

/*****************************************************************************
 * Ethernet
 ****************************************************************************/

struct mv643xx_eth_platform_data qnap_tsx09_eth_data = {
	.phy_addr	= MV643XX_ETH_PHY_ADDR(8),
};

static int __init qnap_tsx09_parse_hex_nibble(char n)
{
	if (n >= '0' && n <= '9')
		return n - '0';

	if (n >= 'A' && n <= 'F')
		return n - 'A' + 10;

	if (n >= 'a' && n <= 'f')
		return n - 'a' + 10;

	return -1;
}

static int __init qnap_tsx09_parse_hex_byte(const char *b)
{
	int hi;
	int lo;

	hi = qnap_tsx09_parse_hex_nibble(b[0]);
	lo = qnap_tsx09_parse_hex_nibble(b[1]);

	if (hi < 0 || lo < 0)
		return -1;

	return (hi << 4) | lo;
}

static int __init qnap_tsx09_check_mac_addr(const char *addr_str)
{
	u_int8_t addr[6];
	int i;

	for (i = 0; i < 6; i++) {
		int byte;

		/*
		 * Enforce "xx:xx:xx:xx:xx:xx\n" format.
		 */
		if (addr_str[(i * 3) + 2] != ((i < 5) ? ':' : '\n'))
			return -1;

		byte = qnap_tsx09_parse_hex_byte(addr_str + (i * 3));
		if (byte < 0)
			return -1;
		addr[i] = byte;
	}

	printk(KERN_INFO "tsx09: found ethernet mac address ");
	for (i = 0; i < 6; i++)
		printk("%.2x%s", addr[i], (i < 5) ? ":" : ".\n");

	memcpy(qnap_tsx09_eth_data.mac_addr, addr, 6);

	return 0;
}

/*
 * The 'NAS Config' flash partition has an ext2 filesystem which
 * contains a file that has the ethernet MAC address in plain text
 * (format "xx:xx:xx:xx:xx:xx\n").
 */
void __init qnap_tsx09_find_mac_addr(u32 mem_base, u32 size)
{
	unsigned long addr;

	for (addr = mem_base; addr < (mem_base + size); addr += 1024) {
		char *nor_page;
		int ret = 0;

		nor_page = ioremap(addr, 1024);
		if (nor_page != NULL) {
			ret = qnap_tsx09_check_mac_addr(nor_page);
			iounmap(nor_page);
		}

		if (ret == 0)
			break;
	}
}
