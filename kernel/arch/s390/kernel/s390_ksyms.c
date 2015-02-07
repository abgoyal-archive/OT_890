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
 *  arch/s390/kernel/s390_ksyms.c
 *
 *  S390 version
 */
#include <linux/highuid.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/syscalls.h>
#include <linux/interrupt.h>
#include <asm/checksum.h>
#include <asm/cpcmd.h>
#include <asm/delay.h>
#include <asm/pgalloc.h>
#include <asm/setup.h>
#include <asm/ftrace.h>
#ifdef CONFIG_IP_MULTICAST
#include <net/arp.h>
#endif

/*
 * memory management
 */
EXPORT_SYMBOL(_oi_bitmap);
EXPORT_SYMBOL(_ni_bitmap);
EXPORT_SYMBOL(_zb_findmap);
EXPORT_SYMBOL(_sb_findmap);

/*
 * binfmt_elf loader 
 */
extern int dump_fpu (struct pt_regs * regs, s390_fp_regs *fpregs);
EXPORT_SYMBOL(dump_fpu);
EXPORT_SYMBOL(empty_zero_page);

/*
 * misc.
 */
EXPORT_SYMBOL(machine_flags);
EXPORT_SYMBOL(__udelay);
EXPORT_SYMBOL(kernel_thread);
EXPORT_SYMBOL(csum_fold);
EXPORT_SYMBOL(console_mode);
EXPORT_SYMBOL(console_devno);
EXPORT_SYMBOL(console_irq);

#ifdef CONFIG_FUNCTION_TRACER
EXPORT_SYMBOL(_mcount);
#endif
