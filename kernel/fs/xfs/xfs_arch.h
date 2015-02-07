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
 * Copyright (c) 2000-2002,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef __XFS_ARCH_H__
#define __XFS_ARCH_H__

#ifndef XFS_BIG_INUMS
# error XFS_BIG_INUMS must be defined true or false
#endif

#ifdef __KERNEL__

#include <asm/byteorder.h>

#ifdef __BIG_ENDIAN
#define	XFS_NATIVE_HOST	1
#else
#undef XFS_NATIVE_HOST
#endif

#else /* __KERNEL__ */

#if __BYTE_ORDER == __BIG_ENDIAN
#define	XFS_NATIVE_HOST	1
#else
#undef XFS_NATIVE_HOST
#endif

#ifdef XFS_NATIVE_HOST
#define cpu_to_be16(val)	((__force __be16)(__u16)(val))
#define cpu_to_be32(val)	((__force __be32)(__u32)(val))
#define cpu_to_be64(val)	((__force __be64)(__u64)(val))
#define be16_to_cpu(val)	((__force __u16)(__be16)(val))
#define be32_to_cpu(val)	((__force __u32)(__be32)(val))
#define be64_to_cpu(val)	((__force __u64)(__be64)(val))
#else
#define cpu_to_be16(val)	((__force __be16)__swab16((__u16)(val)))
#define cpu_to_be32(val)	((__force __be32)__swab32((__u32)(val)))
#define cpu_to_be64(val)	((__force __be64)__swab64((__u64)(val)))
#define be16_to_cpu(val)	(__swab16((__force __u16)(__be16)(val)))
#define be32_to_cpu(val)	(__swab32((__force __u32)(__be32)(val)))
#define be64_to_cpu(val)	(__swab64((__force __u64)(__be64)(val)))
#endif

static inline void be16_add_cpu(__be16 *a, __s16 b)
{
	*a = cpu_to_be16(be16_to_cpu(*a) + b);
}

static inline void be32_add_cpu(__be32 *a, __s32 b)
{
	*a = cpu_to_be32(be32_to_cpu(*a) + b);
}

static inline void be64_add_cpu(__be64 *a, __s64 b)
{
	*a = cpu_to_be64(be64_to_cpu(*a) + b);
}

#endif	/* __KERNEL__ */

/* do we need conversion? */
#define ARCH_NOCONVERT 1
#ifdef XFS_NATIVE_HOST
# define ARCH_CONVERT	ARCH_NOCONVERT
#else
# define ARCH_CONVERT	0
#endif

/* generic swapping macros */

#ifndef HAVE_SWABMACROS
#define INT_SWAP16(type,var) ((typeof(type))(__swab16((__u16)(var))))
#define INT_SWAP32(type,var) ((typeof(type))(__swab32((__u32)(var))))
#define INT_SWAP64(type,var) ((typeof(type))(__swab64((__u64)(var))))
#endif

#define INT_SWAP(type, var) \
    ((sizeof(type) == 8) ? INT_SWAP64(type,var) : \
    ((sizeof(type) == 4) ? INT_SWAP32(type,var) : \
    ((sizeof(type) == 2) ? INT_SWAP16(type,var) : \
    (var))))

/*
 * get and set integers from potentially unaligned locations
 */

#define INT_GET_UNALIGNED_16_BE(pointer) \
   ((__u16)((((__u8*)(pointer))[0] << 8) | (((__u8*)(pointer))[1])))
#define INT_SET_UNALIGNED_16_BE(pointer,value) \
    { \
	((__u8*)(pointer))[0] = (((value) >> 8) & 0xff); \
	((__u8*)(pointer))[1] = (((value)     ) & 0xff); \
    }

/* does not return a value */
#define INT_SET(reference,arch,valueref) \
    (__builtin_constant_p(valueref) ? \
	(void)( (reference) = ( ((arch) != ARCH_NOCONVERT) ? (INT_SWAP((reference),(valueref))) : (valueref)) ) : \
	(void)( \
	    ((reference) = (valueref)), \
	    ( ((arch) != ARCH_NOCONVERT) ? (reference) = INT_SWAP((reference),(reference)) : 0 ) \
	) \
    )

/*
 * In directories inode numbers are stored as unaligned arrays of unsigned
 * 8bit integers on disk.
 *
 * For v1 directories or v2 directories that contain inode numbers that
 * do not fit into 32bit the array has eight members, but the first member
 * is always zero:
 *
 *  |unused|48-55|40-47|32-39|24-31|16-23| 8-15| 0- 7|
 *
 * For v2 directories that only contain entries with inode numbers that fit
 * into 32bits a four-member array is used:
 *
 *  |24-31|16-23| 8-15| 0- 7|
 */ 

#define XFS_GET_DIR_INO4(di) \
	(((__u32)(di).i[0] << 24) | ((di).i[1] << 16) | ((di).i[2] << 8) | ((di).i[3]))

#define XFS_PUT_DIR_INO4(from, di) \
do { \
	(di).i[0] = (((from) & 0xff000000ULL) >> 24); \
	(di).i[1] = (((from) & 0x00ff0000ULL) >> 16); \
	(di).i[2] = (((from) & 0x0000ff00ULL) >> 8); \
	(di).i[3] = ((from) & 0x000000ffULL); \
} while (0)

#define XFS_DI_HI(di) \
	(((__u32)(di).i[1] << 16) | ((di).i[2] << 8) | ((di).i[3]))
#define XFS_DI_LO(di) \
	(((__u32)(di).i[4] << 24) | ((di).i[5] << 16) | ((di).i[6] << 8) | ((di).i[7]))

#define XFS_GET_DIR_INO8(di)        \
	(((xfs_ino_t)XFS_DI_LO(di) & 0xffffffffULL) | \
	 ((xfs_ino_t)XFS_DI_HI(di) << 32))

#define XFS_PUT_DIR_INO8(from, di) \
do { \
	(di).i[0] = 0; \
	(di).i[1] = (((from) & 0x00ff000000000000ULL) >> 48); \
	(di).i[2] = (((from) & 0x0000ff0000000000ULL) >> 40); \
	(di).i[3] = (((from) & 0x000000ff00000000ULL) >> 32); \
	(di).i[4] = (((from) & 0x00000000ff000000ULL) >> 24); \
	(di).i[5] = (((from) & 0x0000000000ff0000ULL) >> 16); \
	(di).i[6] = (((from) & 0x000000000000ff00ULL) >> 8); \
	(di).i[7] = ((from) & 0x00000000000000ffULL); \
} while (0)
	
#endif	/* __XFS_ARCH_H__ */
