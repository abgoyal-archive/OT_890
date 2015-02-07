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

#ifndef _LINUX_ELFCORE_COMPAT_H
#define _LINUX_ELFCORE_COMPAT_H

#include <linux/elf.h>
#include <linux/elfcore.h>
#include <linux/compat.h>

/*
 * Make sure these layouts match the linux/elfcore.h native definitions.
 */

struct compat_elf_siginfo
{
	compat_int_t			si_signo;
	compat_int_t			si_code;
	compat_int_t			si_errno;
};

struct compat_elf_prstatus
{
	struct compat_elf_siginfo	pr_info;
	short				pr_cursig;
	compat_ulong_t			pr_sigpend;
	compat_ulong_t			pr_sighold;
	compat_pid_t			pr_pid;
	compat_pid_t			pr_ppid;
	compat_pid_t			pr_pgrp;
	compat_pid_t			pr_sid;
	struct compat_timeval		pr_utime;
	struct compat_timeval		pr_stime;
	struct compat_timeval		pr_cutime;
	struct compat_timeval		pr_cstime;
	compat_elf_gregset_t		pr_reg;
#ifdef CONFIG_BINFMT_ELF_FDPIC
	compat_ulong_t			pr_exec_fdpic_loadmap;
	compat_ulong_t			pr_interp_fdpic_loadmap;
#endif
	compat_int_t			pr_fpvalid;
};

struct compat_elf_prpsinfo
{
	char				pr_state;
	char				pr_sname;
	char				pr_zomb;
	char				pr_nice;
	compat_ulong_t			pr_flag;
	__compat_uid_t			pr_uid;
	__compat_gid_t			pr_gid;
	compat_pid_t			pr_pid, pr_ppid, pr_pgrp, pr_sid;
	char				pr_fname[16];
	char				pr_psargs[ELF_PRARGSZ];
};

#endif /* _LINUX_ELFCORE_COMPAT_H */
