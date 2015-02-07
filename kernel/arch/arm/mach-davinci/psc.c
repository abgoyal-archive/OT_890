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
 * TI DaVinci Power and Sleep Controller (PSC)
 *
 * Copyright (C) 2006 Texas Instruments.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <mach/psc.h>
#include <mach/mux.h>

/* PSC register offsets */
#define EPCPR		0x070
#define PTCMD		0x120
#define PTSTAT		0x128
#define PDSTAT		0x200
#define PDCTL1		0x304
#define MDSTAT		0x800
#define MDCTL		0xA00

/* System control register offsets */
#define VDD3P3V_PWDN	0x48

static void davinci_psc_mux(unsigned int id)
{
	switch (id) {
	case DAVINCI_LPSC_ATA:
		davinci_mux_peripheral(DAVINCI_MUX_HDIREN, 1);
		davinci_mux_peripheral(DAVINCI_MUX_ATAEN, 1);
		break;
	case DAVINCI_LPSC_MMC_SD:
		/* VDD power manupulations are done in U-Boot for CPMAC
		 * so applies to MMC as well
		 */
		/*Set up the pull regiter for MMC */
		davinci_writel(0, DAVINCI_SYSTEM_MODULE_BASE + VDD3P3V_PWDN);
		davinci_mux_peripheral(DAVINCI_MUX_MSTK, 0);
		break;
	case DAVINCI_LPSC_I2C:
		davinci_mux_peripheral(DAVINCI_MUX_I2C, 1);
		break;
	case DAVINCI_LPSC_McBSP:
		davinci_mux_peripheral(DAVINCI_MUX_ASP, 1);
		break;
	default:
		break;
	}
}

/* Enable or disable a PSC domain */
void davinci_psc_config(unsigned int domain, unsigned int id, char enable)
{
	u32 epcpr, ptcmd, ptstat, pdstat, pdctl1, mdstat, mdctl, mdstat_mask;

	mdctl = davinci_readl(DAVINCI_PWR_SLEEP_CNTRL_BASE + MDCTL + 4 * id);
	if (enable)
		mdctl |= 0x00000003;	/* Enable Module */
	else
		mdctl &= 0xFFFFFFF2;	/* Disable Module */
	davinci_writel(mdctl, DAVINCI_PWR_SLEEP_CNTRL_BASE + MDCTL + 4 * id);

	pdstat = davinci_readl(DAVINCI_PWR_SLEEP_CNTRL_BASE + PDSTAT);
	if ((pdstat & 0x00000001) == 0) {
		pdctl1 = davinci_readl(DAVINCI_PWR_SLEEP_CNTRL_BASE + PDCTL1);
		pdctl1 |= 0x1;
		davinci_writel(pdctl1, DAVINCI_PWR_SLEEP_CNTRL_BASE + PDCTL1);

		ptcmd = 1 << domain;
		davinci_writel(ptcmd, DAVINCI_PWR_SLEEP_CNTRL_BASE + PTCMD);

		do {
			epcpr = davinci_readl(DAVINCI_PWR_SLEEP_CNTRL_BASE +
					      EPCPR);
		} while ((((epcpr >> domain) & 1) == 0));

		pdctl1 = davinci_readl(DAVINCI_PWR_SLEEP_CNTRL_BASE + PDCTL1);
		pdctl1 |= 0x100;
		davinci_writel(pdctl1, DAVINCI_PWR_SLEEP_CNTRL_BASE + PDCTL1);

		do {
			ptstat = davinci_readl(DAVINCI_PWR_SLEEP_CNTRL_BASE +
					       PTSTAT);
		} while (!(((ptstat >> domain) & 1) == 0));
	} else {
		ptcmd = 1 << domain;
		davinci_writel(ptcmd, DAVINCI_PWR_SLEEP_CNTRL_BASE + PTCMD);

		do {
			ptstat = davinci_readl(DAVINCI_PWR_SLEEP_CNTRL_BASE +
					       PTSTAT);
		} while (!(((ptstat >> domain) & 1) == 0));
	}

	if (enable)
		mdstat_mask = 0x3;
	else
		mdstat_mask = 0x2;

	do {
		mdstat = davinci_readl(DAVINCI_PWR_SLEEP_CNTRL_BASE +
				       MDSTAT + 4 * id);
	} while (!((mdstat & 0x0000001F) == mdstat_mask));

	if (enable)
		davinci_psc_mux(id);
}

void __init davinci_psc_init(void)
{
	davinci_psc_config(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_VPSSMSTR, 1);
	davinci_psc_config(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_VPSSSLV, 1);
	davinci_psc_config(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_TPCC, 1);
	davinci_psc_config(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_TPTC0, 1);
	davinci_psc_config(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_TPTC1, 1);
	davinci_psc_config(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_GPIO, 1);

	/* Turn on WatchDog timer LPSC.	 Needed for RESET to work */
	davinci_psc_config(DAVINCI_GPSC_ARMDOMAIN, DAVINCI_LPSC_TIMER2, 1);
}
