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
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file "COPYING" in the main directory of this
 * archive for more details.
 *
 * Copyright (C) 2000 - 2001 by Kanoj Sarcar (kanoj@sgi.com)
 * Copyright (C) 2000 - 2001 by Silicon Graphics, Inc.
 * Copyright (C) 2000, 2001, 2002 Ralf Baechle
 * Copyright (C) 2000, 2001 Broadcom Corporation
 */
#ifndef __ASM_SMP_OPS_H
#define __ASM_SMP_OPS_H

#ifdef CONFIG_SMP

#include <linux/cpumask.h>

struct plat_smp_ops {
	void (*send_ipi_single)(int cpu, unsigned int action);
	void (*send_ipi_mask)(cpumask_t mask, unsigned int action);
	void (*init_secondary)(void);
	void (*smp_finish)(void);
	void (*cpus_done)(void);
	void (*boot_secondary)(int cpu, struct task_struct *idle);
	void (*smp_setup)(void);
	void (*prepare_cpus)(unsigned int max_cpus);
};

extern void register_smp_ops(struct plat_smp_ops *ops);

static inline void plat_smp_setup(void)
{
	extern struct plat_smp_ops *mp_ops;	/* private */

	mp_ops->smp_setup();
}

#else /* !CONFIG_SMP */

struct plat_smp_ops;

static inline void plat_smp_setup(void)
{
	/* UP, nothing to do ...  */
}

static inline void register_smp_ops(struct plat_smp_ops *ops)
{
}

#endif /* !CONFIG_SMP */

extern struct plat_smp_ops up_smp_ops;
extern struct plat_smp_ops cmp_smp_ops;
extern struct plat_smp_ops vsmp_smp_ops;

#endif /* __ASM_SMP_OPS_H */
