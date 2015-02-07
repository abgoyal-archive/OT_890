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

/***************************************************************************/

/*
 *  linux/arch/m68knommu/platform/520x/config.c
 *
 *  Copyright (C) 2005,      Freescale (www.freescale.com)
 *  Copyright (C) 2005,      Intec Automation (mike@steroidmicros.com)
 *  Copyright (C) 1999-2007, Greg Ungerer (gerg@snapgear.com)
 *  Copyright (C) 2001-2003, SnapGear Inc. (www.snapgear.com)
 */

/***************************************************************************/

#include <linux/kernel.h>
#include <linux/param.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <asm/machdep.h>
#include <asm/coldfire.h>
#include <asm/mcfsim.h>
#include <asm/mcfuart.h>

/***************************************************************************/

void coldfire_reset(void);

/***************************************************************************/

static struct mcf_platform_uart m520x_uart_platform[] = {
	{
		.mapbase	= MCF_MBAR + MCFUART_BASE1,
		.irq		= MCFINT_VECBASE + MCFINT_UART0,
	},
	{
		.mapbase 	= MCF_MBAR + MCFUART_BASE2,
		.irq		= MCFINT_VECBASE + MCFINT_UART1,
	},
	{
		.mapbase 	= MCF_MBAR + MCFUART_BASE3,
		.irq		= MCFINT_VECBASE + MCFINT_UART2,
	},
	{ },
};

static struct platform_device m520x_uart = {
	.name			= "mcfuart",
	.id			= 0,
	.dev.platform_data	= m520x_uart_platform,
};

static struct platform_device *m520x_devices[] __initdata = {
	&m520x_uart,
};

/***************************************************************************/

#define	INTC0	(MCF_MBAR + MCFICM_INTC0)

static void __init m520x_uart_init_line(int line, int irq)
{
	u32 imr;
	u16 par;
	u8 par2;

	writeb(0x03, INTC0 + MCFINTC_ICR0 + MCFINT_UART0 + line);

	imr = readl(INTC0 + MCFINTC_IMRL);
	imr &= ~((1 << (irq - MCFINT_VECBASE)) | 1);
	writel(imr, INTC0 + MCFINTC_IMRL);

	switch (line) {
	case 0:
		par = readw(MCF_IPSBAR + MCF_GPIO_PAR_UART);
		par |= MCF_GPIO_PAR_UART_PAR_UTXD0 |
		       MCF_GPIO_PAR_UART_PAR_URXD0;
		writew(par, MCF_IPSBAR + MCF_GPIO_PAR_UART);
		break;
	case 1:
		par = readw(MCF_IPSBAR + MCF_GPIO_PAR_UART);
		par |= MCF_GPIO_PAR_UART_PAR_UTXD1 |
		       MCF_GPIO_PAR_UART_PAR_URXD1;
		writew(par, MCF_IPSBAR + MCF_GPIO_PAR_UART);
		break;
	case 2:
		par2 = readb(MCF_IPSBAR + MCF_GPIO_PAR_FECI2C);
		par2 &= ~0x0F;
		par2 |= MCF_GPIO_PAR_FECI2C_PAR_SCL_UTXD2 |
			MCF_GPIO_PAR_FECI2C_PAR_SDA_URXD2;
		writeb(par2, MCF_IPSBAR + MCF_GPIO_PAR_FECI2C);
		break;
	}
}

static void __init m520x_uarts_init(void)
{
	const int nrlines = ARRAY_SIZE(m520x_uart_platform);
	int line;

	for (line = 0; (line < nrlines); line++)
		m520x_uart_init_line(line, m520x_uart_platform[line].irq);
}

/***************************************************************************/

/*
 *  Program the vector to be an auto-vectored.
 */

void mcf_autovector(unsigned int vec)
{
    /* Everything is auto-vectored on the 520x devices */
}

/***************************************************************************/

void __init config_BSP(char *commandp, int size)
{
	mach_reset = coldfire_reset;
	m520x_uarts_init();
}

/***************************************************************************/

static int __init init_BSP(void)
{
	platform_add_devices(m520x_devices, ARRAY_SIZE(m520x_devices));
	return 0;
}

arch_initcall(init_BSP);

/***************************************************************************/
