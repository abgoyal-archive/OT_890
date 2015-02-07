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
 * Old U-boot compatibility for Acadia
 *
 * Author: Josh Boyer <jwboyer@linux.vnet.ibm.com>
 *
 * Copyright 2008 IBM Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 */

#include "ops.h"
#include "io.h"
#include "dcr.h"
#include "stdio.h"
#include "4xx.h"
#include "44x.h"
#include "cuboot.h"

#define TARGET_4xx
#include "ppcboot.h"

static bd_t bd;

#define CPR_PERD0_SPIDV_MASK   0x000F0000     /* SPI Clock Divider */

#define PLLC_SRC_MASK	       0x20000000     /* PLL feedback source */

#define PLLD_FBDV_MASK	       0x1F000000     /* PLL feedback divider value */
#define PLLD_FWDVA_MASK        0x000F0000     /* PLL forward divider A value */
#define PLLD_FWDVB_MASK        0x00000700     /* PLL forward divider B value */

#define PRIMAD_CPUDV_MASK      0x0F000000     /* CPU Clock Divisor Mask */
#define PRIMAD_PLBDV_MASK      0x000F0000     /* PLB Clock Divisor Mask */
#define PRIMAD_OPBDV_MASK      0x00000F00     /* OPB Clock Divisor Mask */
#define PRIMAD_EBCDV_MASK      0x0000000F     /* EBC Clock Divisor Mask */

#define PERD0_PWMDV_MASK       0xFF000000     /* PWM Divider Mask */
#define PERD0_SPIDV_MASK       0x000F0000     /* SPI Divider Mask */
#define PERD0_U0DV_MASK        0x0000FF00     /* UART 0 Divider Mask */
#define PERD0_U1DV_MASK        0x000000FF     /* UART 1 Divider Mask */

static void get_clocks(void)
{
	unsigned long sysclk, cpr_plld, cpr_pllc, cpr_primad, plloutb, i;
	unsigned long pllFwdDiv, pllFwdDivB, pllFbkDiv, pllPlbDiv, pllExtBusDiv;
	unsigned long pllOpbDiv, freqEBC, freqUART, freqOPB;
	unsigned long div;		/* total divisor udiv * bdiv */
	unsigned long umin;		/* minimum udiv	*/
	unsigned short diff;		/* smallest diff */
	unsigned long udiv;		/* best udiv */
	unsigned short idiff;		/* current diff */
	unsigned short ibdiv;		/* current bdiv */
	unsigned long est;		/* current estimate */
	unsigned long baud;
	void *np;

	/* read the sysclk value from the CPLD */
	sysclk = (in_8((unsigned char *)0x80000000) == 0xc) ? 66666666 : 33333000;

	/*
	 * Read PLL Mode registers
	 */
	cpr_plld = CPR0_READ(DCRN_CPR0_PLLD);
	cpr_pllc = CPR0_READ(DCRN_CPR0_PLLC);

	/*
	 * Determine forward divider A
	 */
	pllFwdDiv = ((cpr_plld & PLLD_FWDVA_MASK) >> 16);

	/*
	 * Determine forward divider B
	 */
	pllFwdDivB = ((cpr_plld & PLLD_FWDVB_MASK) >> 8);
	if (pllFwdDivB == 0)
		pllFwdDivB = 8;

	/*
	 * Determine FBK_DIV.
	 */
	pllFbkDiv = ((cpr_plld & PLLD_FBDV_MASK) >> 24);
	if (pllFbkDiv == 0)
		pllFbkDiv = 256;

	/*
	 * Read CPR_PRIMAD register
	 */
	cpr_primad = CPR0_READ(DCRN_CPR0_PRIMAD);

	/*
	 * Determine PLB_DIV.
	 */
	pllPlbDiv = ((cpr_primad & PRIMAD_PLBDV_MASK) >> 16);
	if (pllPlbDiv == 0)
		pllPlbDiv = 16;

	/*
	 * Determine EXTBUS_DIV.
	 */
	pllExtBusDiv = (cpr_primad & PRIMAD_EBCDV_MASK);
	if (pllExtBusDiv == 0)
		pllExtBusDiv = 16;

	/*
	 * Determine OPB_DIV.
	 */
	pllOpbDiv = ((cpr_primad & PRIMAD_OPBDV_MASK) >> 8);
	if (pllOpbDiv == 0)
		pllOpbDiv = 16;

	/* There is a bug in U-Boot that prevents us from using
	 * bd.bi_opbfreq because U-Boot doesn't populate it for
	 * 405EZ.  We get to calculate it, yay!
	 */
	freqOPB = (sysclk *pllFbkDiv) /pllOpbDiv;

	freqEBC = (sysclk * pllFbkDiv) / pllExtBusDiv;

	plloutb = ((sysclk * ((cpr_pllc & PLLC_SRC_MASK) ?
					   pllFwdDivB : pllFwdDiv) *
		    pllFbkDiv) / pllFwdDivB);

	np = find_node_by_alias("serial0");
	if (getprop(np, "current-speed", &baud, sizeof(baud)) != sizeof(baud))
		fatal("no current-speed property\n\r");

	udiv = 256;			/* Assume lowest possible serial clk */
	div = plloutb / (16 * baud); /* total divisor */
	umin = (plloutb / freqOPB) << 1;	/* 2 x OPB divisor */
	diff = 256;			/* highest possible */

	/* i is the test udiv value -- start with the largest
	 * possible (256) to minimize serial clock and constrain
	 * search to umin.
	 */
	for (i = 256; i > umin; i--) {
		ibdiv = div / i;
		est = i * ibdiv;
		idiff = (est > div) ? (est-div) : (div-est);
		if (idiff == 0) {
			udiv = i;
			break;      /* can't do better */
		} else if (idiff < diff) {
			udiv = i;       /* best so far */
			diff = idiff;   /* update lowest diff*/
		}
	}
	freqUART = plloutb / udiv;

	dt_fixup_cpu_clocks(bd.bi_procfreq, bd.bi_intfreq, bd.bi_plb_busfreq);
	dt_fixup_clock("/plb/ebc", freqEBC);
	dt_fixup_clock("/plb/opb", freqOPB);
	dt_fixup_clock("/plb/opb/serial@ef600300", freqUART);
	dt_fixup_clock("/plb/opb/serial@ef600400", freqUART);
}

static void acadia_fixups(void)
{
	dt_fixup_memory(bd.bi_memstart, bd.bi_memsize);
	get_clocks();
	dt_fixup_mac_address_by_alias("ethernet0", bd.bi_enetaddr);
}
	
void platform_init(unsigned long r3, unsigned long r4, unsigned long r5,
		unsigned long r6, unsigned long r7)
{
	CUBOOT_INIT();
	platform_ops.fixups = acadia_fixups;
	platform_ops.exit = ibm40x_dbcr_reset;
	fdt_init(_dtb_start);
	serial_console_init();
}
