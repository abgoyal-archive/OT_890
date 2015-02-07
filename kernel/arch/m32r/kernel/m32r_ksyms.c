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

#include <linux/module.h>
#include <linux/smp.h>
#include <linux/user.h>
#include <linux/elfcore.h>
#include <linux/sched.h>
#include <linux/in6.h>
#include <linux/interrupt.h>
#include <linux/string.h>

#include <asm/processor.h>
#include <asm/uaccess.h>
#include <asm/checksum.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <asm/irq.h>
#include <asm/tlbflush.h>
#include <asm/pgtable.h>

/* platform dependent support */
EXPORT_SYMBOL(boot_cpu_data);
EXPORT_SYMBOL(dump_fpu);
EXPORT_SYMBOL(__ioremap);
EXPORT_SYMBOL(iounmap);
EXPORT_SYMBOL(kernel_thread);

/* Networking helper routines. */
/* Delay loops */
EXPORT_SYMBOL(__udelay);
EXPORT_SYMBOL(__delay);
EXPORT_SYMBOL(__const_udelay);

EXPORT_SYMBOL(strncpy_from_user);
EXPORT_SYMBOL(__strncpy_from_user);
EXPORT_SYMBOL(clear_user);
EXPORT_SYMBOL(__clear_user);
EXPORT_SYMBOL(__generic_copy_from_user);
EXPORT_SYMBOL(__generic_copy_to_user);
EXPORT_SYMBOL(strnlen_user);

#ifdef CONFIG_SMP
#ifdef CONFIG_CHIP_M32700_TS1
extern void *dcache_dummy;
EXPORT_SYMBOL(dcache_dummy);
#endif
EXPORT_SYMBOL(cpu_data);

/* TLB flushing */
EXPORT_SYMBOL(smp_flush_tlb_page);
#endif

/* compiler generated symbol */
extern void __ashldi3(void);
extern void __ashrdi3(void);
extern void __lshldi3(void);
extern void __lshrdi3(void);
extern void __muldi3(void);
EXPORT_SYMBOL(__ashldi3);
EXPORT_SYMBOL(__ashrdi3);
EXPORT_SYMBOL(__lshldi3);
EXPORT_SYMBOL(__lshrdi3);
EXPORT_SYMBOL(__muldi3);

/* memory and string operations */
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memset);
EXPORT_SYMBOL(copy_page);
EXPORT_SYMBOL(clear_page);
EXPORT_SYMBOL(strlen);
EXPORT_SYMBOL(empty_zero_page);

EXPORT_SYMBOL(_inb);
EXPORT_SYMBOL(_inw);
EXPORT_SYMBOL(_inl);
EXPORT_SYMBOL(_outb);
EXPORT_SYMBOL(_outw);
EXPORT_SYMBOL(_outl);
EXPORT_SYMBOL(_inb_p);
EXPORT_SYMBOL(_inw_p);
EXPORT_SYMBOL(_inl_p);
EXPORT_SYMBOL(_outb_p);
EXPORT_SYMBOL(_outw_p);
EXPORT_SYMBOL(_outl_p);
EXPORT_SYMBOL(_insb);
EXPORT_SYMBOL(_insw);
EXPORT_SYMBOL(_insl);
EXPORT_SYMBOL(_outsb);
EXPORT_SYMBOL(_outsw);
EXPORT_SYMBOL(_outsl);
EXPORT_SYMBOL(_readb);
EXPORT_SYMBOL(_readw);
EXPORT_SYMBOL(_readl);
EXPORT_SYMBOL(_writeb);
EXPORT_SYMBOL(_writew);
EXPORT_SYMBOL(_writel);

