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

#ifndef _ASM_IA64_SYNC_BITOPS_H
#define _ASM_IA64_SYNC_BITOPS_H

/*
 * Copyright (C) 2008 Isaku Yamahata <yamahata at valinux co jp>
 *
 * Based on synch_bitops.h which Dan Magenhaimer wrote.
 *
 * bit operations which provide guaranteed strong synchronisation
 * when communicating with Xen or other guest OSes running on other CPUs.
 */

static inline void sync_set_bit(int nr, volatile void *addr)
{
	set_bit(nr, addr);
}

static inline void sync_clear_bit(int nr, volatile void *addr)
{
	clear_bit(nr, addr);
}

static inline void sync_change_bit(int nr, volatile void *addr)
{
	change_bit(nr, addr);
}

static inline int sync_test_and_set_bit(int nr, volatile void *addr)
{
	return test_and_set_bit(nr, addr);
}

static inline int sync_test_and_clear_bit(int nr, volatile void *addr)
{
	return test_and_clear_bit(nr, addr);
}

static inline int sync_test_and_change_bit(int nr, volatile void *addr)
{
	return test_and_change_bit(nr, addr);
}

static inline int sync_test_bit(int nr, const volatile void *addr)
{
	return test_bit(nr, addr);
}

#define sync_cmpxchg(ptr, old, new)				\
	((__typeof__(*(ptr)))cmpxchg_acq((ptr), (old), (new)))

#endif /* _ASM_IA64_SYNC_BITOPS_H */
