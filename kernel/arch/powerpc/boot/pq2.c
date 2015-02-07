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
 * PowerQUICC II support functions
 *
 * Author: Scott Wood <scottwood@freescale.com>
 *
 * Copyright (c) 2007 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 */

#include "ops.h"
#include "types.h"
#include "fsl-soc.h"
#include "pq2.h"
#include "stdio.h"
#include "io.h"

#define PQ2_SCCR (0x10c80/4) /* System Clock Configuration Register */
#define PQ2_SCMR (0x10c88/4) /* System Clock Mode Register */

static int pq2_corecnf_map[] = {
	3, 2, 2, 2, 4, 4, 5, 9, 6, 11, 8, 10, 3, 12, 7, -1,
	6, 5, 13, 2, 14, 4, 15, 9, 0, 11, 8, 10, 16, 12, 7, -1
};

/* Get various clocks from crystal frequency.
 * Returns zero on failure and non-zero on success.
 */
int pq2_get_clocks(u32 crystal, u32 *sysfreq, u32 *corefreq,
                   u32 *timebase, u32 *brgfreq)
{
	u32 *immr;
	u32 sccr, scmr, mainclk, busclk;
	int corecnf, busdf, plldf, pllmf, dfbrg;

	immr = fsl_get_immr();
	if (!immr) {
		printf("pq2_get_clocks: Couldn't get IMMR base.\r\n");
		return 0;
	}

	sccr = in_be32(&immr[PQ2_SCCR]);
	scmr = in_be32(&immr[PQ2_SCMR]);

	dfbrg = sccr & 3;
	corecnf = (scmr >> 24) & 0x1f;
	busdf = (scmr >> 20) & 0xf;
	plldf = (scmr >> 12) & 1;
	pllmf = scmr & 0xfff;

	mainclk = crystal * (pllmf + 1) / (plldf + 1);
	busclk = mainclk / (busdf + 1);

	if (sysfreq)
		*sysfreq = mainclk / 2;
	if (timebase)
		*timebase = busclk / 4;
	if (brgfreq)
		*brgfreq = mainclk / (1 << ((dfbrg + 1) * 2));

	if (corefreq) {
		int coremult = pq2_corecnf_map[corecnf];

		if (coremult < 0)
			*corefreq = mainclk / 2;
		else if (coremult == 0)
			return 0;
		else
			*corefreq = busclk * coremult / 2;
	}

	return 1;
}

/* Set common device tree fields based on the given clock frequencies. */
void pq2_set_clocks(u32 sysfreq, u32 corefreq, u32 timebase, u32 brgfreq)
{
	void *node;

	dt_fixup_cpu_clocks(corefreq, timebase, sysfreq);

	node = finddevice("/soc/cpm");
	if (node)
		setprop(node, "clock-frequency", &sysfreq, 4);

	node = finddevice("/soc/cpm/brg");
	if (node)
		setprop(node, "clock-frequency", &brgfreq, 4);
}

int pq2_fixup_clocks(u32 crystal)
{
	u32 sysfreq, corefreq, timebase, brgfreq;

	if (!pq2_get_clocks(crystal, &sysfreq, &corefreq, &timebase, &brgfreq))
		return 0;

	pq2_set_clocks(sysfreq, corefreq, timebase, brgfreq);
	return 1;
}
