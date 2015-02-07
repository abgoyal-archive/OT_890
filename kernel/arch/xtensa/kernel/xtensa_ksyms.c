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
 * arch/xtensa/kernel/xtensa_ksyms.c
 *
 * Export Xtensa-specific functions for loadable modules.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2005  Tensilica Inc.
 *
 * Joe Taylor <joe@tensilica.com>
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <linux/in6.h>

#include <asm/uaccess.h>
#include <asm/checksum.h>
#include <asm/dma.h>
#include <asm/io.h>
#include <asm/page.h>
#include <asm/pgalloc.h>
#ifdef CONFIG_BLK_DEV_FD
#include <asm/floppy.h>
#endif
#ifdef CONFIG_NET
#include <net/checksum.h>
#endif /* CONFIG_NET */


/*
 * String functions
 */
EXPORT_SYMBOL(memset);
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memmove);

EXPORT_SYMBOL(kernel_thread);

/*
 * gcc internal math functions
 */
extern long long __ashrdi3(long long, int);
extern long long __ashldi3(long long, int);
extern long long __lshrdi3(long long, int);
extern int __divsi3(int, int);
extern int __modsi3(int, int);
extern long long __muldi3(long long, long long);
extern int __mulsi3(int, int);
extern unsigned int __udivsi3(unsigned int, unsigned int);
extern unsigned int __umodsi3(unsigned int, unsigned int);
extern unsigned long long __umoddi3(unsigned long long, unsigned long long);
extern unsigned long long __udivdi3(unsigned long long, unsigned long long);

EXPORT_SYMBOL(__ashldi3);
EXPORT_SYMBOL(__ashrdi3);
EXPORT_SYMBOL(__lshrdi3);
EXPORT_SYMBOL(__divsi3);
EXPORT_SYMBOL(__modsi3);
EXPORT_SYMBOL(__muldi3);
EXPORT_SYMBOL(__mulsi3);
EXPORT_SYMBOL(__udivsi3);
EXPORT_SYMBOL(__umodsi3);
EXPORT_SYMBOL(__udivdi3);
EXPORT_SYMBOL(__umoddi3);

#ifdef CONFIG_NET
/*
 * Networking support
 */
EXPORT_SYMBOL(csum_partial_copy_generic);
#endif /* CONFIG_NET */

/*
 * Architecture-specific symbols
 */
EXPORT_SYMBOL(__xtensa_copy_user);

/*
 * Kernel hacking ...
 */

#if defined(CONFIG_VGA_CONSOLE) || defined(CONFIG_DUMMY_CONSOLE)
// FIXME EXPORT_SYMBOL(screen_info);
#endif

EXPORT_SYMBOL(outsb);
EXPORT_SYMBOL(outsw);
EXPORT_SYMBOL(outsl);
EXPORT_SYMBOL(insb);
EXPORT_SYMBOL(insw);
EXPORT_SYMBOL(insl);
