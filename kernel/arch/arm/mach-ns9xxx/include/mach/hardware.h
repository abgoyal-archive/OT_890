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
 * arch/arm/mach-ns9xxx/include/mach/hardware.h
 *
 * Copyright (C) 2006 by Digi International Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */
#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

/*
 * NetSilicon NS9xxx internal mapping:
 *
 * physical                <--> virtual
 * 0x90000000 - 0x906fffff <--> 0xf9000000 - 0xf96fffff
 * 0xa0100000 - 0xa0afffff <--> 0xfa100000 - 0xfaafffff
 */
#define io_p2v(x)	(0xf0000000 \
			 + (((x) & 0xf0000000) >> 4) \
			 + ((x) & 0x00ffffff))

#define io_v2p(x)	((((x) & 0x0f000000) << 4) \
			 + ((x) & 0x00ffffff))

#define __REGSHIFT(mask)	((mask) & (-(mask)))

#define __REGBIT(bit)		((u32)1 << (bit))
#define __REGBITS(hbit, lbit)	((((u32)1 << ((hbit) - (lbit) + 1)) - 1) << (lbit))
#define __REGVAL(mask, value)	(((value) * __REGSHIFT(mask)) & (mask))

#ifndef __ASSEMBLY__

#  define __REG(x)	((void __iomem __force *)io_p2v((x)))
#  define __REG2(x, y)	((void __iomem __force *)(io_p2v((x)) + 4 * (y)))

#  define __REGSET(var, field, value)					\
	((var) = (((var) & ~((field) & ~(value))) | (value)))

#  define REGSET(var, reg, field, value)				\
	__REGSET(var, reg ## _ ## field, reg ## _ ## field ## _ ## value)

#  define REGSET_IDX(var, reg, field, idx, value)			\
	__REGSET(var, reg ## _ ## field((idx)), reg ## _ ## field ## _ ## value((idx)))

#  define REGSETIM(var, reg, field, value)				\
	__REGSET(var, reg ## _ ## field, __REGVAL(reg ## _ ## field, (value)))

#  define REGSETIM_IDX(var, reg, field, idx, value)			\
	__REGSET(var, reg ## _ ## field((idx)), __REGVAL(reg ## _ ## field((idx)), (value)))

#  define __REGGET(var, field)						\
	(((var) & (field)))

#  define REGGET(var, reg, field)					\
	 __REGGET(var, reg ## _ ## field)

#  define REGGET_IDX(var, reg, field, idx)				\
	 __REGGET(var, reg ## _ ## field((idx)))

#  define REGGETIM(var, reg, field)					\
	 __REGGET(var, reg ## _ ## field) / __REGSHIFT(reg ## _ ## field)

#  define REGGETIM_IDX(var, reg, field, idx)				\
	 __REGGET(var, reg ## _ ## field((idx))) /			\
	 __REGSHIFT(reg ## _ ## field((idx)))

#else

#  define __REG(x)	io_p2v(x)
#  define __REG2(x, y)	io_p2v((x) + 4 * (y))

#endif

#endif /* ifndef __ASM_ARCH_HARDWARE_H */
