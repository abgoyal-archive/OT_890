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

#ifndef __ASM_CRISv10_ARCH_BUG_H
#define __ASM_CRISv10_ARCH_BUG_H

#include <linux/stringify.h>

#ifdef CONFIG_BUG
#ifdef CONFIG_DEBUG_BUGVERBOSE
/* The BUG() macro is used for marking obviously incorrect code paths.
 * It will cause a message with the file name and line number to be printed,
 * and then cause an oops.  The message is actually printed by handle_BUG()
 * in arch/cris/kernel/traps.c, and the reason we use this method of storing
 * the file name and line number is that we do not want to affect the registers
 * by calling printk() before causing the oops.
 */

#define BUG_PREFIX 0x0D7F
#define BUG_MAGIC  0x00001234

struct bug_frame {
	unsigned short prefix;
	unsigned int magic;
	unsigned short clear;
	unsigned short movu;
	unsigned short line;
	unsigned short jump;
	unsigned char *filename;
};

#if 0
/* Unfortunately this version of the macro does not work due to a problem
 * with the compiler (aka a bug) when compiling with -O2, which sometimes
 * erroneously causes the second input to be stored in a register...
 */
#define BUG()								\
	__asm__ __volatile__ ("clear.d [" __stringify(BUG_MAGIC) "]\n\t"\
				"movu.w %0,$r0\n\t"			\
				"jump %1\n\t"				\
				: : "i" (__LINE__), "i" (__FILE__))
#else
/* This version will have to do for now, until the compiler is fixed.
 * The drawbacks of this version are that the file name will appear multiple
 * times in the .rodata section, and that __LINE__ and __FILE__ can probably
 * not be used like this with newer versions of gcc.
 */
#define BUG()								\
	__asm__ __volatile__ ("clear.d [" __stringify(BUG_MAGIC) "]\n\t"\
			      "movu.w " __stringify(__LINE__) ",$r0\n\t"\
			      "jump 0f\n\t"				\
			      ".section .rodata\n"			\
			      "0:\t.string \"" __FILE__ "\"\n\t"	\
			      ".previous")
#endif

#else

/* This just causes an oops. */
#define BUG() (*(int *)0 = 0)

#endif

#define HAVE_ARCH_BUG
#endif

#include <asm-generic/bug.h>

#endif
