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
 * Generate definitions needed by assembly language modules.
 * This code generates raw asm output which is post-processed to extract
 * and format the required data.
 */

#include <linux/types.h>
#include <linux/stddef.h>
#include <linux/sched.h>
#include <linux/ptrace.h>
#include <linux/kbuild.h>
#include <asm/io.h>

void foo(void)
{
	DEFINE(TI_TASK, offsetof(struct thread_info, task));
	DEFINE(TI_FLAGS, offsetof(struct thread_info, flags));
	DEFINE(TI_CPU, offsetof(struct thread_info, cpu));
	BLANK();

        DEFINE(TASK_BLOCKED, offsetof(struct task_struct, blocked));
        DEFINE(TASK_CRED, offsetof(struct task_struct, cred));
        DEFINE(TASK_REAL_PARENT, offsetof(struct task_struct, real_parent));
        DEFINE(TASK_GROUP_LEADER, offsetof(struct task_struct, group_leader));
        DEFINE(TASK_TGID, offsetof(struct task_struct, tgid));
        BLANK();

        DEFINE(CRED_UID,  offsetof(struct cred, uid));
        DEFINE(CRED_EUID, offsetof(struct cred, euid));
        DEFINE(CRED_GID,  offsetof(struct cred, gid));
        DEFINE(CRED_EGID, offsetof(struct cred, egid));
        BLANK();

	DEFINE(SIZEOF_PT_REGS, sizeof(struct pt_regs));
	DEFINE(PT_PTRACED, PT_PTRACED);
	DEFINE(CLONE_VM, CLONE_VM);
	DEFINE(CLONE_UNTRACED, CLONE_UNTRACED);
	DEFINE(SIGCHLD, SIGCHLD);
	BLANK();

	DEFINE(HAE_CACHE, offsetof(struct alpha_machine_vector, hae_cache));
	DEFINE(HAE_REG, offsetof(struct alpha_machine_vector, hae_register));
}
