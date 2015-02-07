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

/* MN103E010 Processor initialisation
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#include <linux/kernel.h>
#include <asm/rtc.h>

/*
 * initialise the on-silicon processor peripherals
 */
asmlinkage void __init processor_init(void)
{
	int loop;

	/* set up the exception table first */
	for (loop = 0x000; loop < 0x400; loop += 8)
		__set_intr_stub(loop, __common_exception);

	__set_intr_stub(EXCEP_ITLBMISS,		itlb_miss);
	__set_intr_stub(EXCEP_DTLBMISS,		dtlb_miss);
	__set_intr_stub(EXCEP_IAERROR,		itlb_aerror);
	__set_intr_stub(EXCEP_DAERROR,		dtlb_aerror);
	__set_intr_stub(EXCEP_BUSERROR,		raw_bus_error);
	__set_intr_stub(EXCEP_DOUBLE_FAULT,	double_fault);
	__set_intr_stub(EXCEP_SYSCALL0,		system_call);

	__set_intr_stub(EXCEP_NMI,		nmi_handler);
	__set_intr_stub(EXCEP_WDT,		nmi_handler);
	__set_intr_stub(EXCEP_IRQ_LEVEL0,	irq_handler);
	__set_intr_stub(EXCEP_IRQ_LEVEL1,	irq_handler);
	__set_intr_stub(EXCEP_IRQ_LEVEL2,	irq_handler);
	__set_intr_stub(EXCEP_IRQ_LEVEL3,	irq_handler);
	__set_intr_stub(EXCEP_IRQ_LEVEL4,	irq_handler);
	__set_intr_stub(EXCEP_IRQ_LEVEL5,	irq_handler);
	__set_intr_stub(EXCEP_IRQ_LEVEL6,	irq_handler);

	IVAR0 = EXCEP_IRQ_LEVEL0;
	IVAR1 = EXCEP_IRQ_LEVEL1;
	IVAR2 = EXCEP_IRQ_LEVEL2;
	IVAR3 = EXCEP_IRQ_LEVEL3;
	IVAR4 = EXCEP_IRQ_LEVEL4;
	IVAR5 = EXCEP_IRQ_LEVEL5;
	IVAR6 = EXCEP_IRQ_LEVEL6;

	mn10300_dcache_flush_inv();
	mn10300_icache_inv();

	/* disable all interrupts and set to priority 6 (lowest) */
	for (loop = 0; loop < NR_IRQS; loop++)
		GxICR(loop) = GxICR_LEVEL_6 | GxICR_DETECT;

	/* clear the timers */
	TM0MD	= 0;
	TM1MD	= 0;
	TM2MD	= 0;
	TM3MD	= 0;
	TM4MD	= 0;
	TM5MD	= 0;
	TM6MD	= 0;
	TM6MDA	= 0;
	TM6MDB	= 0;
	TM7MD	= 0;
	TM8MD	= 0;
	TM9MD	= 0;
	TM10MD	= 0;
	TM11MD	= 0;

	calibrate_clock();
}
