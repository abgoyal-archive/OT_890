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

#include <linux/init.h>
#include <linux/types.h>
#include <linux/audit.h>
#include <asm/unistd.h>

static unsigned dir_class[] = {
#include <asm-generic/audit_dir_write.h>
~0U
};

static unsigned read_class[] = {
#include <asm-generic/audit_read.h>
~0U
};

static unsigned write_class[] = {
#include <asm-generic/audit_write.h>
~0U
};

static unsigned chattr_class[] = {
#include <asm-generic/audit_change_attr.h>
~0U
};

static unsigned signal_class[] = {
#include <asm-generic/audit_signal.h>
~0U
};

int audit_classify_arch(int arch)
{
#ifdef CONFIG_IA32_EMULATION
	if (arch == AUDIT_ARCH_I386)
		return 1;
#endif
	return 0;
}

int audit_classify_syscall(int abi, unsigned syscall)
{
#ifdef CONFIG_IA32_EMULATION
	extern int ia32_classify_syscall(unsigned);
	if (abi == AUDIT_ARCH_I386)
		return ia32_classify_syscall(syscall);
#endif
	switch(syscall) {
	case __NR_open:
		return 2;
	case __NR_openat:
		return 3;
	case __NR_execve:
		return 5;
	default:
		return 0;
	}
}

static int __init audit_classes_init(void)
{
#ifdef CONFIG_IA32_EMULATION
	extern __u32 ia32_dir_class[];
	extern __u32 ia32_write_class[];
	extern __u32 ia32_read_class[];
	extern __u32 ia32_chattr_class[];
	extern __u32 ia32_signal_class[];
	audit_register_class(AUDIT_CLASS_WRITE_32, ia32_write_class);
	audit_register_class(AUDIT_CLASS_READ_32, ia32_read_class);
	audit_register_class(AUDIT_CLASS_DIR_WRITE_32, ia32_dir_class);
	audit_register_class(AUDIT_CLASS_CHATTR_32, ia32_chattr_class);
	audit_register_class(AUDIT_CLASS_SIGNAL_32, ia32_signal_class);
#endif
	audit_register_class(AUDIT_CLASS_WRITE, write_class);
	audit_register_class(AUDIT_CLASS_READ, read_class);
	audit_register_class(AUDIT_CLASS_DIR_WRITE, dir_class);
	audit_register_class(AUDIT_CLASS_CHATTR, chattr_class);
	audit_register_class(AUDIT_CLASS_SIGNAL, signal_class);
	return 0;
}

__initcall(audit_classes_init);
