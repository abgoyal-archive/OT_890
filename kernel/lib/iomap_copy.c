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
 * Copyright 2006 PathScale, Inc.  All Rights Reserved.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <linux/module.h>
#include <linux/io.h>

/**
 * __iowrite32_copy - copy data to MMIO space, in 32-bit units
 * @to: destination, in MMIO space (must be 32-bit aligned)
 * @from: source (must be 32-bit aligned)
 * @count: number of 32-bit quantities to copy
 *
 * Copy data from kernel space to MMIO space, in units of 32 bits at a
 * time.  Order of access is not guaranteed, nor is a memory barrier
 * performed afterwards.
 */
void __attribute__((weak)) __iowrite32_copy(void __iomem *to,
					    const void *from,
					    size_t count)
{
	u32 __iomem *dst = to;
	const u32 *src = from;
	const u32 *end = src + count;

	while (src < end)
		__raw_writel(*src++, dst++);
}
EXPORT_SYMBOL_GPL(__iowrite32_copy);

/**
 * __iowrite64_copy - copy data to MMIO space, in 64-bit or 32-bit units
 * @to: destination, in MMIO space (must be 64-bit aligned)
 * @from: source (must be 64-bit aligned)
 * @count: number of 64-bit quantities to copy
 *
 * Copy data from kernel space to MMIO space, in units of 32 or 64 bits at a
 * time.  Order of access is not guaranteed, nor is a memory barrier
 * performed afterwards.
 */
void __attribute__((weak)) __iowrite64_copy(void __iomem *to,
					    const void *from,
					    size_t count)
{
#ifdef CONFIG_64BIT
	u64 __iomem *dst = to;
	const u64 *src = from;
	const u64 *end = src + count;

	while (src < end)
		__raw_writeq(*src++, dst++);
#else
	__iowrite32_copy(to, from, count * 2);
#endif
}

EXPORT_SYMBOL_GPL(__iowrite64_copy);
