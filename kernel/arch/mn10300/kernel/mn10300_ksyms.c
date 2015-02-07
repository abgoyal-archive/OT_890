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

/* MN10300 Miscellaneous and library kernel exports
 *
 * Copyright (C) 2007 Matsushita Electric Industrial Co., Ltd.
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#include <linux/module.h>
#include <asm/uaccess.h>
#include <asm/pgtable.h>


EXPORT_SYMBOL(empty_zero_page);

EXPORT_SYMBOL(change_bit);
EXPORT_SYMBOL(test_and_change_bit);

EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memmove);
EXPORT_SYMBOL(memset);

EXPORT_SYMBOL(strncpy_from_user);
EXPORT_SYMBOL(__strncpy_from_user);
EXPORT_SYMBOL(clear_user);
EXPORT_SYMBOL(__clear_user);
EXPORT_SYMBOL(__generic_copy_from_user);
EXPORT_SYMBOL(__generic_copy_to_user);
EXPORT_SYMBOL(strnlen_user);

extern u64 __ashrdi3(u64, unsigned);
extern u64 __ashldi3(u64, unsigned);
extern u64 __lshrdi3(u64, unsigned);
extern s64 __negdi2(s64);
extern int __ucmpdi2(u64, u64);
EXPORT_SYMBOL(__ashrdi3);
EXPORT_SYMBOL(__ashldi3);
EXPORT_SYMBOL(__lshrdi3);
EXPORT_SYMBOL(__negdi2);
EXPORT_SYMBOL(__ucmpdi2);
