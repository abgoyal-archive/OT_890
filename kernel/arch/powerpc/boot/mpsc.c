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
 * MPSC/UART driver for the Marvell mv64360, mv64460, ...
 *
 * Author: Mark A. Greer <mgreer@mvista.com>
 *
 * 2007 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#include <stdarg.h>
#include <stddef.h>
#include "types.h"
#include "string.h"
#include "stdio.h"
#include "io.h"
#include "ops.h"


#define MPSC_CHR_1		0x000c

#define MPSC_CHR_2		0x0010
#define MPSC_CHR_2_TA		(1<<7)
#define MPSC_CHR_2_TCS		(1<<9)
#define MPSC_CHR_2_RA		(1<<23)
#define MPSC_CHR_2_CRD		(1<<25)
#define MPSC_CHR_2_EH		(1<<31)

#define MPSC_CHR_4		0x0018
#define MPSC_CHR_4_Z		(1<<29)

#define MPSC_CHR_5		0x001c
#define MPSC_CHR_5_CTL1_INTR	(1<<12)
#define MPSC_CHR_5_CTL1_VALID	(1<<15)

#define MPSC_CHR_10		0x0030

#define MPSC_INTR_CAUSE		0x0000
#define MPSC_INTR_CAUSE_RCC	(1<<6)
#define MPSC_INTR_MASK		0x0080

#define SDMA_SDCM		0x0008
#define SDMA_SDCM_AR		(1<<15)
#define SDMA_SDCM_AT		(1<<31)

static volatile char *mpsc_base;
static volatile char *mpscintr_base;
static u32 chr1, chr2;

static int mpsc_open(void)
{
	chr1 = in_le32((u32 *)(mpsc_base + MPSC_CHR_1)) & 0x00ff0000;
	chr2 = in_le32((u32 *)(mpsc_base + MPSC_CHR_2)) & ~(MPSC_CHR_2_TA
			| MPSC_CHR_2_TCS | MPSC_CHR_2_RA | MPSC_CHR_2_CRD
			| MPSC_CHR_2_EH);
	out_le32((u32 *)(mpsc_base + MPSC_CHR_4), MPSC_CHR_4_Z);
	out_le32((u32 *)(mpsc_base + MPSC_CHR_5),
			MPSC_CHR_5_CTL1_INTR | MPSC_CHR_5_CTL1_VALID);
	out_le32((u32 *)(mpsc_base + MPSC_CHR_2), chr2 | MPSC_CHR_2_EH);
	return 0;
}

static void mpsc_putc(unsigned char c)
{
	while (in_le32((u32 *)(mpsc_base + MPSC_CHR_2)) & MPSC_CHR_2_TCS);

	out_le32((u32 *)(mpsc_base + MPSC_CHR_1), chr1 | c);
	out_le32((u32 *)(mpsc_base + MPSC_CHR_2), chr2 | MPSC_CHR_2_TCS);
}

static unsigned char mpsc_getc(void)
{
	u32 cause = 0;
	unsigned char c;

	while (!(cause & MPSC_INTR_CAUSE_RCC))
		cause = in_le32((u32 *)(mpscintr_base + MPSC_INTR_CAUSE));

	c = in_8((u8 *)(mpsc_base + MPSC_CHR_10 + 2));
	out_8((u8 *)(mpsc_base + MPSC_CHR_10 + 2), c);
	out_le32((u32 *)(mpscintr_base + MPSC_INTR_CAUSE),
			cause & ~MPSC_INTR_CAUSE_RCC);

	return c;
}

static u8 mpsc_tstc(void)
{
	return (u8)((in_le32((u32 *)(mpscintr_base + MPSC_INTR_CAUSE))
				& MPSC_INTR_CAUSE_RCC) != 0);
}

static void mpsc_stop_dma(volatile char *sdma_base)
{
	out_le32((u32 *)(mpsc_base + MPSC_CHR_2),MPSC_CHR_2_TA | MPSC_CHR_2_RA);
	out_le32((u32 *)(sdma_base + SDMA_SDCM), SDMA_SDCM_AR | SDMA_SDCM_AT);

	while ((in_le32((u32 *)(sdma_base + SDMA_SDCM))
				& (SDMA_SDCM_AR | SDMA_SDCM_AT)) != 0)
		udelay(100);
}

static volatile char *mpsc_get_virtreg_of_phandle(void *devp, char *prop)
{
	void *v;
	int n;

	n = getprop(devp, prop, &v, sizeof(v));
	if (n != sizeof(v))
		goto err_out;

	devp = find_node_by_linuxphandle((u32)v);
	if (devp == NULL)
		goto err_out;

	n = getprop(devp, "virtual-reg", &v, sizeof(v));
	if (n == sizeof(v))
		return v;

err_out:
	return NULL;
}

int mpsc_console_init(void *devp, struct serial_console_data *scdp)
{
	void *v;
	int n, reg_set;
	volatile char *sdma_base;

	n = getprop(devp, "virtual-reg", &v, sizeof(v));
	if (n != sizeof(v))
		goto err_out;
	mpsc_base = v;

	sdma_base = mpsc_get_virtreg_of_phandle(devp, "sdma");
	if (sdma_base == NULL)
		goto err_out;

	mpscintr_base = mpsc_get_virtreg_of_phandle(devp, "mpscintr");
	if (mpscintr_base == NULL)
		goto err_out;

	n = getprop(devp, "cell-index", &v, sizeof(v));
	if (n != sizeof(v))
		goto err_out;
	reg_set = (int)v;

	mpscintr_base += (reg_set == 0) ? 0x4 : 0xc;

	/* Make sure the mpsc ctlrs are shutdown */
	out_le32((u32 *)(mpscintr_base + MPSC_INTR_CAUSE), 0);
	out_le32((u32 *)(mpscintr_base + MPSC_INTR_CAUSE), 0);
	out_le32((u32 *)(mpscintr_base + MPSC_INTR_MASK), 0);
	out_le32((u32 *)(mpscintr_base + MPSC_INTR_MASK), 0);

	mpsc_stop_dma(sdma_base);

	scdp->open = mpsc_open;
	scdp->putc = mpsc_putc;
	scdp->getc = mpsc_getc;
	scdp->tstc = mpsc_tstc;
	scdp->close = NULL;

	return 0;

err_out:
	return -1;
}
