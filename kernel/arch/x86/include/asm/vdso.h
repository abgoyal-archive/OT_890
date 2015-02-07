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

#ifndef _ASM_X86_VDSO_H
#define _ASM_X86_VDSO_H

#ifdef CONFIG_X86_64
extern const char VDSO64_PRELINK[];

/*
 * Given a pointer to the vDSO image, find the pointer to VDSO64_name
 * as that symbol is defined in the vDSO sources or linker script.
 */
#define VDSO64_SYMBOL(base, name)					\
({									\
	extern const char VDSO64_##name[];				\
	(void *)(VDSO64_##name - VDSO64_PRELINK + (unsigned long)(base)); \
})
#endif

#if defined CONFIG_X86_32 || defined CONFIG_COMPAT
extern const char VDSO32_PRELINK[];

/*
 * Given a pointer to the vDSO image, find the pointer to VDSO32_name
 * as that symbol is defined in the vDSO sources or linker script.
 */
#define VDSO32_SYMBOL(base, name)					\
({									\
	extern const char VDSO32_##name[];				\
	(void *)(VDSO32_##name - VDSO32_PRELINK + (unsigned long)(base)); \
})
#endif

/*
 * These symbols are defined with the addresses in the vsyscall page.
 * See vsyscall-sigreturn.S.
 */
extern void __user __kernel_sigreturn;
extern void __user __kernel_rt_sigreturn;

/*
 * These symbols are defined by vdso32.S to mark the bounds
 * of the ELF DSO images included therein.
 */
extern const char vdso32_int80_start, vdso32_int80_end;
extern const char vdso32_syscall_start, vdso32_syscall_end;
extern const char vdso32_sysenter_start, vdso32_sysenter_end;

#endif /* _ASM_X86_VDSO_H */
