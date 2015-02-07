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
 *	linux/arch/alpha/kernel/es1888.c
 *
 * Init the built-in ES1888 sound chip (SB16 compatible)
 */

#include <linux/init.h>
#include <asm/io.h>
#include "proto.h"

void __init
es1888_init(void)
{
	/* Sequence of IO reads to init the audio controller */
	inb(0x0229);
	inb(0x0229);
	inb(0x0229);
	inb(0x022b);
	inb(0x0229);
	inb(0x022b);
	inb(0x0229);
	inb(0x0229);
	inb(0x022b);
	inb(0x0229);
	inb(0x0220); /* This sets the base address to 0x220 */

	/* Sequence to set DMA channels */
	outb(0x01, 0x0226);		/* reset */
	inb(0x0226);			/* pause */
	outb(0x00, 0x0226);		/* release reset */
	while (!(inb(0x022e) & 0x80))	/* wait for bit 7 to assert*/
		continue;
	inb(0x022a);			/* pause */
	outb(0xc6, 0x022c);		/* enable extended mode */
	inb(0x022a);			/* pause, also forces the write */
	while (inb(0x022c) & 0x80)	/* wait for bit 7 to deassert */
		continue;
	outb(0xb1, 0x022c);		/* setup for write to Interrupt CR */
	while (inb(0x022c) & 0x80)	/* wait for bit 7 to deassert */
		continue;
	outb(0x14, 0x022c);		/* set IRQ 5 */
	while (inb(0x022c) & 0x80)	/* wait for bit 7 to deassert */
		continue;
	outb(0xb2, 0x022c);		/* setup for write to DMA CR */
	while (inb(0x022c) & 0x80)	/* wait for bit 7 to deassert */
		continue;
	outb(0x18, 0x022c);		/* set DMA channel 1 */
	inb(0x022c);			/* force the write */
}
