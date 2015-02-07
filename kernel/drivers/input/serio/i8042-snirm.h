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

#ifndef _I8042_SNIRM_H
#define _I8042_SNIRM_H

#include <asm/sni.h>

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

/*
 * Names.
 */

#define I8042_KBD_PHYS_DESC "onboard/serio0"
#define I8042_AUX_PHYS_DESC "onboard/serio1"
#define I8042_MUX_PHYS_DESC "onboard/serio%d"

/*
 * IRQs.
 */
static int i8042_kbd_irq;
static int i8042_aux_irq;
#define I8042_KBD_IRQ i8042_kbd_irq
#define I8042_AUX_IRQ i8042_aux_irq

static void __iomem *kbd_iobase;

#define I8042_COMMAND_REG	(kbd_iobase + 0x64UL)
#define I8042_DATA_REG		(kbd_iobase + 0x60UL)

static inline int i8042_read_data(void)
{
	return readb(kbd_iobase + 0x60UL);
}

static inline int i8042_read_status(void)
{
	return readb(kbd_iobase + 0x64UL);
}

static inline void i8042_write_data(int val)
{
	writeb(val, kbd_iobase + 0x60UL);
}

static inline void i8042_write_command(int val)
{
	writeb(val, kbd_iobase + 0x64UL);
}
static inline int i8042_platform_init(void)
{
	/* RM200 is strange ... */
	if (sni_brd_type == SNI_BRD_RM200) {
		kbd_iobase = ioremap(0x16000000, 4);
		i8042_kbd_irq = 33;
		i8042_aux_irq = 44;
	} else {
		kbd_iobase = ioremap(0x14000000, 4);
		i8042_kbd_irq = 1;
		i8042_aux_irq = 12;
	}
	if (!kbd_iobase)
		return -ENOMEM;

	return 0;
}

static inline void i8042_platform_exit(void)
{

}

#endif /* _I8042_SNIRM_H */
