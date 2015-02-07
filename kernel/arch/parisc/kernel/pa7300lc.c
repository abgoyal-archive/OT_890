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
 *   linux/arch/parisc/kernel/pa7300lc.c
 *	- PA7300LC-specific functions	
 *
 *   Copyright (C) 2000 Philipp Rumpf */

#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <asm/ptrace.h>
#include <asm/machdep.h>

/* CPU register indices */

#define MIOC_STATUS	0xf040
#define MIOC_CONTROL	0xf080
#define MDERRADD	0xf0e0
#define DMAERR		0xf0e8
#define DIOERR		0xf0ec
#define HIDMAMEM	0xf0f4

/* this returns the HPA of the CPU it was called on */
static u32 cpu_hpa(void)
{
	return 0xfffb0000;
}

static void pa7300lc_lpmc(int code, struct pt_regs *regs)
{
	u32 hpa;
	printk(KERN_WARNING "LPMC on CPU %d\n", smp_processor_id());

	show_regs(regs);

	hpa = cpu_hpa();
	printk(KERN_WARNING
		"MIOC_CONTROL %08x\n" "MIOC_STATUS  %08x\n"
		"MDERRADD     %08x\n" "DMAERR       %08x\n"
		"DIOERR       %08x\n" "HIDMAMEM     %08x\n",
		gsc_readl(hpa+MIOC_CONTROL), gsc_readl(hpa+MIOC_STATUS),
		gsc_readl(hpa+MDERRADD), gsc_readl(hpa+DMAERR),
		gsc_readl(hpa+DIOERR), gsc_readl(hpa+HIDMAMEM));
}

void pa7300lc_init(void)
{
	cpu_lpmc = pa7300lc_lpmc;
}
