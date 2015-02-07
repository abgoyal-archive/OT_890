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
 * udbg serial input/output routines for the Marvell MV64x60 (Discovery).
 *
 * Author: Dale Farnsworth <dale@farnsworth.org>
 *
 * 2007 (c) MontaVista Software, Inc.  This file is licensed under
 * the terms of the GNU General Public License version 2.  This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#include <asm/io.h>
#include <asm/prom.h>
#include <asm/udbg.h>

#include <sysdev/mv64x60.h>

#define MPSC_0_CR1_OFFSET	0x000c

#define MPSC_0_CR2_OFFSET	0x0010
#define MPSC_CHR_2_TCS		(1 << 9)

#define MPSC_0_CHR_10_OFFSET	0x0030

#define MPSC_INTR_CAUSE_OFF_0	0x0004
#define MPSC_INTR_CAUSE_OFF_1	0x000c
#define MPSC_INTR_CAUSE_RCC	(1<<6)

static void __iomem *mpsc_base;
static void __iomem *mpsc_intr_cause;

static void mv64x60_udbg_putc(char c)
{
	if (c == '\n')
		mv64x60_udbg_putc('\r');

	while(in_le32(mpsc_base + MPSC_0_CR2_OFFSET) & MPSC_CHR_2_TCS)
		;
	out_le32(mpsc_base + MPSC_0_CR1_OFFSET, c);
	out_le32(mpsc_base + MPSC_0_CR2_OFFSET, MPSC_CHR_2_TCS);
}

static int mv64x60_udbg_testc(void)
{
	return (in_le32(mpsc_intr_cause) & MPSC_INTR_CAUSE_RCC) != 0;
}

static int mv64x60_udbg_getc(void)
{
	int cause = 0;
	int c;

	while (!mv64x60_udbg_testc())
		;

	c = in_8(mpsc_base + MPSC_0_CHR_10_OFFSET + 2);
	out_8(mpsc_base + MPSC_0_CHR_10_OFFSET + 2, c);
	out_le32(mpsc_intr_cause, cause & ~MPSC_INTR_CAUSE_RCC);
	return c;
}

static int mv64x60_udbg_getc_poll(void)
{
	if (!mv64x60_udbg_testc())
		return -1;

	return mv64x60_udbg_getc();
}

static void mv64x60_udbg_init(void)
{
	struct device_node *np, *mpscintr, *stdout = NULL;
	const char *path;
	const phandle *ph;
	struct resource r[2];
	const int *block_index;
	int intr_cause_offset;
	int err;

	path = of_get_property(of_chosen, "linux,stdout-path", NULL);
	if (!path)
		return;

	stdout = of_find_node_by_path(path);
	if (!stdout)
		return;

	for_each_compatible_node(np, "serial", "marvell,mv64360-mpsc") {
		if (np == stdout)
			break;
	}

	of_node_put(stdout);
	if (!np)
		return;

	block_index = of_get_property(np, "cell-index", NULL);
	if (!block_index)
		goto error;

	switch (*block_index) {
	case 0:
		intr_cause_offset = MPSC_INTR_CAUSE_OFF_0;
		break;
	case 1:
		intr_cause_offset = MPSC_INTR_CAUSE_OFF_1;
		break;
	default:
		goto error;
	}

	err = of_address_to_resource(np, 0, &r[0]);
	if (err)
		goto error;

	ph = of_get_property(np, "mpscintr", NULL);
	mpscintr = of_find_node_by_phandle(*ph);
	if (!mpscintr)
		goto error;

	err = of_address_to_resource(mpscintr, 0, &r[1]);
	of_node_put(mpscintr);
	if (err)
		goto error;

	of_node_put(np);

	mpsc_base = ioremap(r[0].start, r[0].end - r[0].start + 1);
	if (!mpsc_base)
		return;

	mpsc_intr_cause = ioremap(r[1].start, r[1].end - r[1].start + 1);
	if (!mpsc_intr_cause) {
		iounmap(mpsc_base);
		return;
	}
	mpsc_intr_cause += intr_cause_offset;

	udbg_putc = mv64x60_udbg_putc;
	udbg_getc = mv64x60_udbg_getc;
	udbg_getc_poll = mv64x60_udbg_getc_poll;

	return;

error:
	of_node_put(np);
}

void mv64x60_init_early(void)
{
	mv64x60_udbg_init();
}
