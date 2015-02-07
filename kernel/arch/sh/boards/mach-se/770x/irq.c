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
 * linux/arch/sh/boards/se/770x/irq.c
 *
 * Copyright (C) 2000  Kazumoto Kojima
 * Copyright (C) 2006  Nobuhiro Iwamatsu
 *
 * Hitachi SolutionEngine Support.
 *
 */

#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <mach-se/mach/se.h>

static struct ipr_data ipr_irq_table[] = {
	/*
	* Super I/O (Just mimic PC):
	*  1: keyboard
	*  3: serial 0
	*  4: serial 1
	*  5: printer
	*  6: floppy
	*  8: rtc
	* 12: mouse
	* 14: ide0
	*/
#if defined(CONFIG_CPU_SUBTYPE_SH7705)
	/* This is default value */
	{ 13, 0, 8,  0x0f-13, },
	{ 5 , 0, 4,  0x0f- 5, },
	{ 10, 1, 0,  0x0f-10, },
	{ 7 , 2, 4,  0x0f- 7, },
	{ 3 , 2, 0,  0x0f- 3, },
	{ 1 , 3, 12, 0x0f- 1, },
	{ 12, 3, 4,  0x0f-12, }, /* LAN */
	{ 2 , 4, 8,  0x0f- 2, }, /* PCIRQ2 */
	{ 6 , 4, 4,  0x0f- 6, }, /* PCIRQ1 */
	{ 14, 4, 0,  0x0f-14, }, /* PCIRQ0 */
	{ 0 , 5, 12, 0x0f   , }, 
	{ 4 , 5, 4,  0x0f- 4, },
	{ 8 , 6, 12, 0x0f- 8, },
	{ 9 , 6, 8,  0x0f- 9, },
	{ 11, 6, 4,  0x0f-11, },
#else
	{ 14, 0,  8, 0x0f-14, },
	{ 12, 0,  4, 0x0f-12, },
	{  8, 1,  4, 0x0f- 8, },
	{  6, 2, 12, 0x0f- 6, },
	{  5, 2,  8, 0x0f- 5, },
	{  4, 2,  4, 0x0f- 4, },
	{  3, 2,  0, 0x0f- 3, },
	{  1, 3, 12, 0x0f- 1, },
#if defined(CONFIG_STNIC)
	/* ST NIC */
	{ 10, 3,  4, 0x0f-10, }, 	/* LAN */
#endif
	/* MRSHPC IRQs setting */
	{  0, 4, 12, 0x0f- 0, },	/* PCIRQ3 */
	{ 11, 4,  8, 0x0f-11, }, 	/* PCIRQ2 */
	{  9, 4,  4, 0x0f- 9, }, 	/* PCIRQ1 */
	{  7, 4,  0, 0x0f- 7, }, 	/* PCIRQ0 */
	/* #2, #13 are allocated for SLOT IRQ #1 and #2 (for now) */
	/* NOTE: #2 and #13 are not used on PC */
	{ 13, 6,  4, 0x0f-13, }, 	/* SLOTIRQ2 */
	{  2, 6,  0, 0x0f- 2, }, 	/* SLOTIRQ1 */
#endif
};

static unsigned long ipr_offsets[] = {
	BCR_ILCRA,
	BCR_ILCRB,
	BCR_ILCRC,
	BCR_ILCRD,
	BCR_ILCRE,
	BCR_ILCRF,
	BCR_ILCRG,
};

static struct ipr_desc ipr_irq_desc = {
	.ipr_offsets	= ipr_offsets,
	.nr_offsets	= ARRAY_SIZE(ipr_offsets),

	.ipr_data	= ipr_irq_table,
	.nr_irqs	= ARRAY_SIZE(ipr_irq_table),
	.chip = {
		.name	= "IPR-se770x",
	},
};

/*
 * Initialize IRQ setting
 */
void __init init_se_IRQ(void)
{
	/* Disable all interrupts */
	ctrl_outw(0, BCR_ILCRA);
	ctrl_outw(0, BCR_ILCRB);
	ctrl_outw(0, BCR_ILCRC);
	ctrl_outw(0, BCR_ILCRD);
	ctrl_outw(0, BCR_ILCRE);
	ctrl_outw(0, BCR_ILCRF);
	ctrl_outw(0, BCR_ILCRG);

	register_ipr_controller(&ipr_irq_desc);
}
