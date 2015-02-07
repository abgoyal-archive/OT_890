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
 * Export AVR32-specific functions for loadable modules.
 *
 * Copyright (C) 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/module.h>

#include <asm/checksum.h>
#include <asm/uaccess.h>

/*
 * GCC functions
 */
extern unsigned long long __avr32_lsl64(unsigned long long u, unsigned long b);
extern unsigned long long __avr32_lsr64(unsigned long long u, unsigned long b);
extern unsigned long long __avr32_asr64(unsigned long long u, unsigned long b);
EXPORT_SYMBOL(__avr32_lsl64);
EXPORT_SYMBOL(__avr32_lsr64);
EXPORT_SYMBOL(__avr32_asr64);

/*
 * String functions
 */
EXPORT_SYMBOL(memset);
EXPORT_SYMBOL(memcpy);

EXPORT_SYMBOL(clear_page);
EXPORT_SYMBOL(copy_page);

/*
 * Userspace access stuff.
 */
EXPORT_SYMBOL(copy_from_user);
EXPORT_SYMBOL(copy_to_user);
EXPORT_SYMBOL(__copy_user);
EXPORT_SYMBOL(strncpy_from_user);
EXPORT_SYMBOL(__strncpy_from_user);
EXPORT_SYMBOL(clear_user);
EXPORT_SYMBOL(__clear_user);
EXPORT_SYMBOL(strnlen_user);

EXPORT_SYMBOL(csum_partial);
EXPORT_SYMBOL(csum_partial_copy_generic);

/* Delay loops (lib/delay.S) */
EXPORT_SYMBOL(__ndelay);
EXPORT_SYMBOL(__udelay);
EXPORT_SYMBOL(__const_udelay);

/* Bit operations (lib/findbit.S) */
EXPORT_SYMBOL(find_first_zero_bit);
EXPORT_SYMBOL(find_next_zero_bit);
EXPORT_SYMBOL(find_first_bit);
EXPORT_SYMBOL(find_next_bit);
EXPORT_SYMBOL(generic_find_next_le_bit);
EXPORT_SYMBOL(generic_find_next_zero_le_bit);

/* I/O primitives (lib/io-*.S) */
EXPORT_SYMBOL(__raw_readsb);
EXPORT_SYMBOL(__raw_readsw);
EXPORT_SYMBOL(__raw_readsl);
EXPORT_SYMBOL(__raw_writesb);
EXPORT_SYMBOL(__raw_writesw);
EXPORT_SYMBOL(__raw_writesl);
