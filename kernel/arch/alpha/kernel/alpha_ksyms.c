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
 * linux/arch/alpha/kernel/alpha_ksyms.c
 *
 * Export the alpha-specific functions that are needed for loadable
 * modules.
 */

#include <linux/module.h>
#include <asm/console.h>
#include <asm/uaccess.h>
#include <asm/checksum.h>
#include <asm/fpu.h>
#include <asm/machvec.h>

#include <linux/syscalls.h>

/* these are C runtime functions with special calling conventions: */
extern void __divl (void);
extern void __reml (void);
extern void __divq (void);
extern void __remq (void);
extern void __divlu (void);
extern void __remlu (void);
extern void __divqu (void);
extern void __remqu (void);

EXPORT_SYMBOL(alpha_mv);
EXPORT_SYMBOL(callback_getenv);
EXPORT_SYMBOL(callback_setenv);
EXPORT_SYMBOL(callback_save_env);

/* platform dependent support */
EXPORT_SYMBOL(strcat);
EXPORT_SYMBOL(strcpy);
EXPORT_SYMBOL(strlen);
EXPORT_SYMBOL(strncpy);
EXPORT_SYMBOL(strncat);
EXPORT_SYMBOL(strchr);
EXPORT_SYMBOL(strrchr);
EXPORT_SYMBOL(memmove);
EXPORT_SYMBOL(__memcpy);
EXPORT_SYMBOL(__memset);
EXPORT_SYMBOL(__memsetw);
EXPORT_SYMBOL(__constant_c_memset);
EXPORT_SYMBOL(copy_page);
EXPORT_SYMBOL(clear_page);

EXPORT_SYMBOL(alpha_read_fp_reg);
EXPORT_SYMBOL(alpha_read_fp_reg_s);
EXPORT_SYMBOL(alpha_write_fp_reg);
EXPORT_SYMBOL(alpha_write_fp_reg_s);

/* entry.S */
EXPORT_SYMBOL(kernel_thread);
EXPORT_SYMBOL(kernel_execve);

/* Networking helper routines. */
EXPORT_SYMBOL(csum_tcpudp_magic);
EXPORT_SYMBOL(ip_compute_csum);
EXPORT_SYMBOL(ip_fast_csum);
EXPORT_SYMBOL(csum_partial_copy_nocheck);
EXPORT_SYMBOL(csum_partial_copy_from_user);
EXPORT_SYMBOL(csum_ipv6_magic);

#ifdef CONFIG_MATHEMU_MODULE
extern long (*alpha_fp_emul_imprecise)(struct pt_regs *, unsigned long);
extern long (*alpha_fp_emul) (unsigned long pc);
EXPORT_SYMBOL(alpha_fp_emul_imprecise);
EXPORT_SYMBOL(alpha_fp_emul);
#endif

/*
 * The following are specially called from the uaccess assembly stubs.
 */
EXPORT_SYMBOL(__copy_user);
EXPORT_SYMBOL(__do_clear_user);
EXPORT_SYMBOL(__strncpy_from_user);
EXPORT_SYMBOL(__strnlen_user);

/* 
 * SMP-specific symbols.
 */

#ifdef CONFIG_SMP
EXPORT_SYMBOL(_atomic_dec_and_lock);
#endif /* CONFIG_SMP */

/*
 * The following are special because they're not called
 * explicitly (the C compiler or assembler generates them in
 * response to division operations).  Fortunately, their
 * interface isn't gonna change any time soon now, so it's OK
 * to leave it out of version control.
 */
# undef memcpy
# undef memset
EXPORT_SYMBOL(__divl);
EXPORT_SYMBOL(__divlu);
EXPORT_SYMBOL(__divq);
EXPORT_SYMBOL(__divqu);
EXPORT_SYMBOL(__reml);
EXPORT_SYMBOL(__remlu);
EXPORT_SYMBOL(__remq);
EXPORT_SYMBOL(__remqu);
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memset);
EXPORT_SYMBOL(memchr);
