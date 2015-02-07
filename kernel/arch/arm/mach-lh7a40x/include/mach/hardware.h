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

/* arch/arm/mach-lh7a40x/include/mach/hardware.h
 *
 *  Copyright (C) 2004 Coastal Environmental Systems
 *
 *  [ Substantially cribbed from arch/arm/mach-pxa/include/mach/hardware.h ]
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.
 *
 */

#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

#include <asm/sizes.h>		/* Added for the sake of amba-clcd driver */

#define io_p2v(x) (0xf0000000 | (((x) & 0xfff00000) >> 4) | ((x) & 0x0000ffff))
#define io_v2p(x) (             (((x) & 0x0fff0000) << 4) | ((x) & 0x0000ffff))

#ifdef __ASSEMBLY__

# define __REG(x)	io_p2v(x)
# define __PREG(x)	io_v2p(x)

#else

# if 0
#  define __REG(x)	(*((volatile u32 *)io_p2v(x)))
# else
/*
 * This __REG() version gives the same results as the one above,  except
 * that we are fooling gcc somehow so it generates far better and smaller
 * assembly code for access to contigous registers.  It's a shame that gcc
 * doesn't guess this by itself.
 */
#include <asm/types.h>
typedef struct { volatile u32 offset[4096]; } __regbase;
# define __REGP(x)	((__regbase *)((x)&~4095))->offset[((x)&4095)>>2]
# define __REG(x)	__REGP(io_p2v(x))
typedef struct { volatile u16 offset[4096]; } __regbase16;
# define __REGP16(x)	((__regbase16 *)((x)&~4095))->offset[((x)&4095)>>1]
# define __REG16(x)	__REGP16(io_p2v(x))
typedef struct { volatile u8 offset[4096]; } __regbase8;
# define __REGP8(x)	((__regbase8 *)((x)&~4095))->offset[(x)&4095]
# define __REG8(x)	__REGP8(io_p2v(x))
#endif

/* Let's kick gcc's ass again... */
# define __REG2(x,y)	\
	( __builtin_constant_p(y) ? (__REG((x) + (y))) \
				  : (*(volatile u32 *)((u32)&__REG(x) + (y))) )

# define __PREG(x)	(io_v2p((u32)&(x)))

#endif

#define MASK_AND_SET(v,m,s)	(v) = ((v)&~(m))|(s)

#include "registers.h"

#endif  /* _ASM_ARCH_HARDWARE_H */
