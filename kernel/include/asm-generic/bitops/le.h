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

#ifndef _ASM_GENERIC_BITOPS_LE_H_
#define _ASM_GENERIC_BITOPS_LE_H_

#include <asm/types.h>
#include <asm/byteorder.h>

#define BITOP_WORD(nr)		((nr) / BITS_PER_LONG)
#define BITOP_LE_SWIZZLE	((BITS_PER_LONG-1) & ~0x7)

#if defined(__LITTLE_ENDIAN)

#define generic_test_le_bit(nr, addr) test_bit(nr, addr)
#define generic___set_le_bit(nr, addr) __set_bit(nr, addr)
#define generic___clear_le_bit(nr, addr) __clear_bit(nr, addr)

#define generic_test_and_set_le_bit(nr, addr) test_and_set_bit(nr, addr)
#define generic_test_and_clear_le_bit(nr, addr) test_and_clear_bit(nr, addr)

#define generic___test_and_set_le_bit(nr, addr) __test_and_set_bit(nr, addr)
#define generic___test_and_clear_le_bit(nr, addr) __test_and_clear_bit(nr, addr)

#define generic_find_next_zero_le_bit(addr, size, offset) find_next_zero_bit(addr, size, offset)
#define generic_find_next_le_bit(addr, size, offset) \
			find_next_bit(addr, size, offset)

#elif defined(__BIG_ENDIAN)

#define generic_test_le_bit(nr, addr) \
	test_bit((nr) ^ BITOP_LE_SWIZZLE, (addr))
#define generic___set_le_bit(nr, addr) \
	__set_bit((nr) ^ BITOP_LE_SWIZZLE, (addr))
#define generic___clear_le_bit(nr, addr) \
	__clear_bit((nr) ^ BITOP_LE_SWIZZLE, (addr))

#define generic_test_and_set_le_bit(nr, addr) \
	test_and_set_bit((nr) ^ BITOP_LE_SWIZZLE, (addr))
#define generic_test_and_clear_le_bit(nr, addr) \
	test_and_clear_bit((nr) ^ BITOP_LE_SWIZZLE, (addr))

#define generic___test_and_set_le_bit(nr, addr) \
	__test_and_set_bit((nr) ^ BITOP_LE_SWIZZLE, (addr))
#define generic___test_and_clear_le_bit(nr, addr) \
	__test_and_clear_bit((nr) ^ BITOP_LE_SWIZZLE, (addr))

extern unsigned long generic_find_next_zero_le_bit(const unsigned long *addr,
		unsigned long size, unsigned long offset);
extern unsigned long generic_find_next_le_bit(const unsigned long *addr,
		unsigned long size, unsigned long offset);

#else
#error "Please fix <asm/byteorder.h>"
#endif

#define generic_find_first_zero_le_bit(addr, size) \
        generic_find_next_zero_le_bit((addr), (size), 0)

#endif /* _ASM_GENERIC_BITOPS_LE_H_ */
