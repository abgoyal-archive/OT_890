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
 * endian.h - Defines for endianness handling in NTFS Linux kernel driver.
 *	      Part of the Linux-NTFS project.
 *
 * Copyright (c) 2001-2004 Anton Altaparmakov
 *
 * This program/include file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program/include file is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the main directory of the Linux-NTFS
 * distribution in the file COPYING); if not, write to the Free Software
 * Foundation,Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _LINUX_NTFS_ENDIAN_H
#define _LINUX_NTFS_ENDIAN_H

#include <asm/byteorder.h>
#include "types.h"

/*
 * Signed endianness conversion functions.
 */

static inline s16 sle16_to_cpu(sle16 x)
{
	return le16_to_cpu((__force le16)x);
}

static inline s32 sle32_to_cpu(sle32 x)
{
	return le32_to_cpu((__force le32)x);
}

static inline s64 sle64_to_cpu(sle64 x)
{
	return le64_to_cpu((__force le64)x);
}

static inline s16 sle16_to_cpup(sle16 *x)
{
	return le16_to_cpu(*(__force le16*)x);
}

static inline s32 sle32_to_cpup(sle32 *x)
{
	return le32_to_cpu(*(__force le32*)x);
}

static inline s64 sle64_to_cpup(sle64 *x)
{
	return le64_to_cpu(*(__force le64*)x);
}

static inline sle16 cpu_to_sle16(s16 x)
{
	return (__force sle16)cpu_to_le16(x);
}

static inline sle32 cpu_to_sle32(s32 x)
{
	return (__force sle32)cpu_to_le32(x);
}

static inline sle64 cpu_to_sle64(s64 x)
{
	return (__force sle64)cpu_to_le64(x);
}

static inline sle16 cpu_to_sle16p(s16 *x)
{
	return (__force sle16)cpu_to_le16(*x);
}

static inline sle32 cpu_to_sle32p(s32 *x)
{
	return (__force sle32)cpu_to_le32(*x);
}

static inline sle64 cpu_to_sle64p(s64 *x)
{
	return (__force sle64)cpu_to_le64(*x);
}

#endif /* _LINUX_NTFS_ENDIAN_H */
