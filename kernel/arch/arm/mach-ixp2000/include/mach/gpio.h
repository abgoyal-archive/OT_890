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
 * arch/arm/mach-ixp2000/include/mach/gpio.h
 *
 * Copyright (C) 2002 Intel Corporation.
 *
 * This program is free software, you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/*
 * IXP2000 GPIO in/out, edge/level detection for IRQs:
 * IRQs are generated on Falling-edge, Rising-Edge, Level-low, Level-High
 * or both Falling-edge and Rising-edge.  
 * This must be called *before* the corresponding IRQ is registerd.
 * Use this instead of directly setting the GPIO registers.
 * GPIOs may also be used as GPIOs (e.g. for emulating i2c/smb)
 */
#ifndef __ASM_ARCH_GPIO_H
#define __ASM_ARCH_GPIO_H

#ifndef __ASSEMBLY__

#define GPIO_IN				0
#define GPIO_OUT			1

#define IXP2000_GPIO_LOW		0
#define IXP2000_GPIO_HIGH		1

extern void gpio_line_config(int line, int direction);

static inline int gpio_line_get(int line)
{
	return (((*IXP2000_GPIO_PLR) >> line) & 1);
}

static inline void gpio_line_set(int line, int value)
{
	if (value == IXP2000_GPIO_HIGH) {
		ixp2000_reg_write(IXP2000_GPIO_POSR, 1 << line);
	} else if (value == IXP2000_GPIO_LOW) {
		ixp2000_reg_write(IXP2000_GPIO_POCR, 1 << line);
	}
}

#endif /* !__ASSEMBLY__ */

#endif /* ASM_ARCH_IXP2000_GPIO_H_ */
