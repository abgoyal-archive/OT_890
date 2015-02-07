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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994 - 1999, 2000, 03, 04 Ralf Baechle
 * Copyright (C) 2000, 2002  Maciej W. Rozycki
 * Copyright (C) 1990, 1999, 2000 Silicon Graphics, Inc.
 */
#ifndef _ASM_MACH_GENERIC_SPACES_H
#define _ASM_MACH_GENERIC_SPACES_H

#include <linux/const.h>

/*
 * This gives the physical RAM offset.
 */
#ifndef PHYS_OFFSET
#define PHYS_OFFSET		_AC(0, UL)
#endif

#ifdef CONFIG_32BIT

#define CAC_BASE		_AC(0x80000000, UL)
#define IO_BASE			_AC(0xa0000000, UL)
#define UNCAC_BASE		_AC(0xa0000000, UL)

#ifndef MAP_BASE
#define MAP_BASE		_AC(0xc0000000, UL)
#endif

/*
 * Memory above this physical address will be considered highmem.
 */
#ifndef HIGHMEM_START
#define HIGHMEM_START		_AC(0x20000000, UL)
#endif

#endif /* CONFIG_32BIT */

#ifdef CONFIG_64BIT

#ifndef CAC_BASE
#ifdef CONFIG_DMA_NONCOHERENT
#define CAC_BASE		_AC(0x9800000000000000, UL)
#else
#define CAC_BASE		_AC(0xa800000000000000, UL)
#endif
#endif

#ifndef IO_BASE
#define IO_BASE			_AC(0x9000000000000000, UL)
#endif

#ifndef UNCAC_BASE
#define UNCAC_BASE		_AC(0x9000000000000000, UL)
#endif

#ifndef MAP_BASE
#define MAP_BASE		_AC(0xc000000000000000, UL)
#endif

/*
 * Memory above this physical address will be considered highmem.
 * Fixme: 59 bits is a fictive number and makes assumptions about processors
 * in the distant future.  Nobody will care for a few years :-)
 */
#ifndef HIGHMEM_START
#define HIGHMEM_START		(_AC(1, UL) << _AC(59, UL))
#endif

#define TO_PHYS(x)		(             ((x) & TO_PHYS_MASK))
#define TO_CAC(x)		(CAC_BASE   | ((x) & TO_PHYS_MASK))
#define TO_UNCAC(x)		(UNCAC_BASE | ((x) & TO_PHYS_MASK))

#endif /* CONFIG_64BIT */

/*
 * This handles the memory map.
 */
#ifndef PAGE_OFFSET
#define PAGE_OFFSET		(CAC_BASE + PHYS_OFFSET)
#endif

#endif /* __ASM_MACH_GENERIC_SPACES_H */
