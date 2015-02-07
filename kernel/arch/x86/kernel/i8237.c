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
 * 8237A DMA controller suspend functions.
 *
 * Written by Pierre Ossman, 2005.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */

#include <linux/init.h>
#include <linux/sysdev.h>

#include <asm/dma.h>

/*
 * This module just handles suspend/resume issues with the
 * 8237A DMA controller (used for ISA and LPC).
 * Allocation is handled in kernel/dma.c and normal usage is
 * in asm/dma.h.
 */

static int i8237A_resume(struct sys_device *dev)
{
	unsigned long flags;
	int i;

	flags = claim_dma_lock();

	dma_outb(0, DMA1_RESET_REG);
	dma_outb(0, DMA2_RESET_REG);

	for (i = 0; i < 8; i++) {
		set_dma_addr(i, 0x000000);
		/* DMA count is a bit weird so this is not 0 */
		set_dma_count(i, 1);
	}

	/* Enable cascade DMA or channel 0-3 won't work */
	enable_dma(4);

	release_dma_lock(flags);

	return 0;
}

static int i8237A_suspend(struct sys_device *dev, pm_message_t state)
{
	return 0;
}

static struct sysdev_class i8237_sysdev_class = {
	.name		= "i8237",
	.suspend	= i8237A_suspend,
	.resume		= i8237A_resume,
};

static struct sys_device device_i8237A = {
	.id		= 0,
	.cls		= &i8237_sysdev_class,
};

static int __init i8237A_init_sysfs(void)
{
	int error = sysdev_class_register(&i8237_sysdev_class);
	if (!error)
		error = sysdev_register(&device_i8237A);
	return error;
}
device_initcall(i8237A_init_sysfs);
