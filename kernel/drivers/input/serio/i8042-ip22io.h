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

#ifndef _I8042_IP22_H
#define _I8042_IP22_H

#include <asm/sgi/ioc.h>
#include <asm/sgi/ip22.h>

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

/*
 * Names.
 */

#define I8042_KBD_PHYS_DESC "hpc3ps2/serio0"
#define I8042_AUX_PHYS_DESC "hpc3ps2/serio1"
#define I8042_MUX_PHYS_DESC "hpc3ps2/serio%d"

/*
 * IRQs.
 */

#define I8042_KBD_IRQ SGI_KEYBD_IRQ
#define I8042_AUX_IRQ SGI_KEYBD_IRQ

/*
 * Register numbers.
 */

#define I8042_COMMAND_REG	((unsigned long)&sgioc->kbdmouse.command)
#define I8042_STATUS_REG	((unsigned long)&sgioc->kbdmouse.command)
#define I8042_DATA_REG		((unsigned long)&sgioc->kbdmouse.data)

static inline int i8042_read_data(void)
{
	return sgioc->kbdmouse.data;
}

static inline int i8042_read_status(void)
{
	return sgioc->kbdmouse.command;
}

static inline void i8042_write_data(int val)
{
	sgioc->kbdmouse.data = val;
}

static inline void i8042_write_command(int val)
{
	sgioc->kbdmouse.command = val;
}

static inline int i8042_platform_init(void)
{
#if 0
	/* XXX sgi_kh is a virtual address */
	if (!request_mem_region(sgi_kh, sizeof(struct hpc_keyb), "i8042"))
		return -EBUSY;
#endif

	i8042_reset = 1;

	return 0;
}

static inline void i8042_platform_exit(void)
{
#if 0
	release_mem_region(JAZZ_KEYBOARD_ADDRESS, sizeof(struct hpc_keyb));
#endif
}

#endif /* _I8042_IP22_H */
