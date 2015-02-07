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
 *	linux/arch/m68knommu/platform/5249/config.c
 *
 *	Copyright (C) 2002, Greg Ungerer (gerg@snapgear.com)
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

static struct mcf_platform_uart m5249_uart_platform[] = {
	{
		.mapbase	= MCF_MBAR + MCFUART_BASE1,
		.irq		= 73,
	},
	{
		.mapbase 	= MCF_MBAR + MCFUART_BASE2,
		.irq		= 74,
	}
};

static struct platform_device m5249_uart = {
	.name			= "mcfuart",
	.id			= 0,
	.dev.platform_data	= m5249_uart_platform,
};

static struct platform_device *m5249_devices[] __initdata = {
	&m5249_uart,
};

/***************************************************************************/

static void __init m5249_uart_init_line(int line, int irq)
{
	if (line == 0) {
		writel(MCFSIM_ICR_LEVEL6 | MCFSIM_ICR_PRI1, MCF_MBAR + MCFSIM_UART1ICR);
		writeb(irq, MCFUART_BASE1 + MCFUART_UIVR);
		mcf_setimr(mcf_getimr() & ~MCFSIM_IMR_UART1);
	} else if (line == 1) {
		writel(MCFSIM_ICR_LEVEL6 | MCFSIM_ICR_PRI2, MCF_MBAR + MCFSIM_UART2ICR);
		writeb(irq, MCFUART_BASE2 + MCFUART_UIVR);
		mcf_setimr(mcf_getimr() & ~MCFSIM_IMR_UART2);
	}
}

static void __init m5249_uarts_init(void)
{
	const int nrlines = ARRAY_SIZE(m5249_uart_platform);
	int line;

	for (line = 0; (line < nrlines); line++)
		m5249_uart_init_line(line, m5249_uart_platform[line].irq);
}


/***************************************************************************/

void mcf_autovector(unsigned int vec)
{
	volatile unsigned char  *mbar;

	if ((vec >= 25) && (vec <= 31)) {
		mbar = (volatile unsigned char *) MCF_MBAR;
		vec = 0x1 << (vec - 24);
		*(mbar + MCFSIM_AVR) |= vec;
		mcf_setimr(mcf_getimr() & ~vec);
	}
}

/***************************************************************************/

void mcf_settimericr(unsigned int timer, unsigned int level)
{
	volatile unsigned char *icrp;
	unsigned int icr, imr;

	if (timer <= 2) {
		switch (timer) {
		case 2:  icr = MCFSIM_TIMER2ICR; imr = MCFSIM_IMR_TIMER2; break;
		default: icr = MCFSIM_TIMER1ICR; imr = MCFSIM_IMR_TIMER1; break;
		}

		icrp = (volatile unsigned char *) (MCF_MBAR + icr);
		*icrp = MCFSIM_ICR_AUTOVEC | (level << 2) | MCFSIM_ICR_PRI3;
		mcf_setimr(mcf_getimr() & ~imr);
	}
}

/***************************************************************************/

void __init config_BSP(char *commandp, int size)
{
	mcf_setimr(MCFSIM_IMR_MASKALL);
	mach_reset = coldfire_reset;
}

/***************************************************************************/

static int __init init_BSP(void)
{
	m5249_uarts_init();
	platform_add_devices(m5249_devices, ARRAY_SIZE(m5249_devices));
	return 0;
}

arch_initcall(init_BSP);

/***************************************************************************/
