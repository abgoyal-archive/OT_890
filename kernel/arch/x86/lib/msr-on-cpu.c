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
#include <linux/preempt.h>
#include <linux/smp.h>
#include <asm/msr.h>

struct msr_info {
	u32 msr_no;
	u32 l, h;
	int err;
};

static void __rdmsr_on_cpu(void *info)
{
	struct msr_info *rv = info;

	rdmsr(rv->msr_no, rv->l, rv->h);
}

static void __wrmsr_on_cpu(void *info)
{
	struct msr_info *rv = info;

	wrmsr(rv->msr_no, rv->l, rv->h);
}

int rdmsr_on_cpu(unsigned int cpu, u32 msr_no, u32 *l, u32 *h)
{
	int err;
	struct msr_info rv;

	rv.msr_no = msr_no;
	err = smp_call_function_single(cpu, __rdmsr_on_cpu, &rv, 1);
	*l = rv.l;
	*h = rv.h;

	return err;
}

int wrmsr_on_cpu(unsigned int cpu, u32 msr_no, u32 l, u32 h)
{
	int err;
	struct msr_info rv;

	rv.msr_no = msr_no;
	rv.l = l;
	rv.h = h;
	err = smp_call_function_single(cpu, __wrmsr_on_cpu, &rv, 1);

	return err;
}

/* These "safe" variants are slower and should be used when the target MSR
   may not actually exist. */
static void __rdmsr_safe_on_cpu(void *info)
{
	struct msr_info *rv = info;

	rv->err = rdmsr_safe(rv->msr_no, &rv->l, &rv->h);
}

static void __wrmsr_safe_on_cpu(void *info)
{
	struct msr_info *rv = info;

	rv->err = wrmsr_safe(rv->msr_no, rv->l, rv->h);
}

int rdmsr_safe_on_cpu(unsigned int cpu, u32 msr_no, u32 *l, u32 *h)
{
	int err;
	struct msr_info rv;

	rv.msr_no = msr_no;
	err = smp_call_function_single(cpu, __rdmsr_safe_on_cpu, &rv, 1);
	*l = rv.l;
	*h = rv.h;

	return err ? err : rv.err;
}

int wrmsr_safe_on_cpu(unsigned int cpu, u32 msr_no, u32 l, u32 h)
{
	int err;
	struct msr_info rv;

	rv.msr_no = msr_no;
	rv.l = l;
	rv.h = h;
	err = smp_call_function_single(cpu, __wrmsr_safe_on_cpu, &rv, 1);

	return err ? err : rv.err;
}

EXPORT_SYMBOL(rdmsr_on_cpu);
EXPORT_SYMBOL(wrmsr_on_cpu);
EXPORT_SYMBOL(rdmsr_safe_on_cpu);
EXPORT_SYMBOL(wrmsr_safe_on_cpu);
