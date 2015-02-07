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

#ifndef _LINUX_UNALIGNED_PACKED_STRUCT_H
#define _LINUX_UNALIGNED_PACKED_STRUCT_H

#include <linux/kernel.h>

struct __una_u16 { u16 x __attribute__((packed)); };
struct __una_u32 { u32 x __attribute__((packed)); };
struct __una_u64 { u64 x __attribute__((packed)); };

static inline u16 __get_unaligned_cpu16(const void *p)
{
	const struct __una_u16 *ptr = (const struct __una_u16 *)p;
	return ptr->x;
}

static inline u32 __get_unaligned_cpu32(const void *p)
{
	const struct __una_u32 *ptr = (const struct __una_u32 *)p;
	return ptr->x;
}

static inline u64 __get_unaligned_cpu64(const void *p)
{
	const struct __una_u64 *ptr = (const struct __una_u64 *)p;
	return ptr->x;
}

static inline void __put_unaligned_cpu16(u16 val, void *p)
{
	struct __una_u16 *ptr = (struct __una_u16 *)p;
	ptr->x = val;
}

static inline void __put_unaligned_cpu32(u32 val, void *p)
{
	struct __una_u32 *ptr = (struct __una_u32 *)p;
	ptr->x = val;
}

static inline void __put_unaligned_cpu64(u64 val, void *p)
{
	struct __una_u64 *ptr = (struct __una_u64 *)p;
	ptr->x = val;
}

#endif /* _LINUX_UNALIGNED_PACKED_STRUCT_H */
