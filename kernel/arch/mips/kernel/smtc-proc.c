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
 * /proc hooks for SMTC kernel
 * Copyright (C) 2005 Mips Technologies, Inc
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/cpumask.h>
#include <linux/interrupt.h>

#include <asm/cpu.h>
#include <asm/processor.h>
#include <asm/atomic.h>
#include <asm/system.h>
#include <asm/hardirq.h>
#include <asm/mmu_context.h>
#include <asm/mipsregs.h>
#include <asm/cacheflush.h>
#include <linux/proc_fs.h>

#include <asm/smtc_proc.h>

/*
 * /proc diagnostic and statistics hooks
 */

/*
 * Statistics gathered
 */
unsigned long selfipis[NR_CPUS];

struct smtc_cpu_proc smtc_cpu_stats[NR_CPUS];

static struct proc_dir_entry *smtc_stats;

atomic_t smtc_fpu_recoveries;

static int proc_read_smtc(char *page, char **start, off_t off,
                          int count, int *eof, void *data)
{
	int totalen = 0;
	int len;
	int i;
	extern unsigned long ebase;

	len = sprintf(page, "SMTC Status Word: 0x%08x\n", smtc_status);
	totalen += len;
	page += len;
	len = sprintf(page, "Config7: 0x%08x\n", read_c0_config7());
	totalen += len;
	page += len;
	len = sprintf(page, "EBASE: 0x%08lx\n", ebase);
	totalen += len;
	page += len;
	len = sprintf(page, "Counter Interrupts taken per CPU (TC)\n");
	totalen += len;
	page += len;
	for (i=0; i < NR_CPUS; i++) {
		len = sprintf(page, "%d: %ld\n", i, smtc_cpu_stats[i].timerints);
		totalen += len;
		page += len;
	}
	len = sprintf(page, "Self-IPIs by CPU:\n");
	totalen += len;
	page += len;
	for(i = 0; i < NR_CPUS; i++) {
		len = sprintf(page, "%d: %ld\n", i, smtc_cpu_stats[i].selfipis);
		totalen += len;
		page += len;
	}
	len = sprintf(page, "%d Recoveries of \"stolen\" FPU\n",
	              atomic_read(&smtc_fpu_recoveries));
	totalen += len;
	page += len;

	return totalen;
}

void init_smtc_stats(void)
{
	int i;

	for (i=0; i<NR_CPUS; i++) {
		smtc_cpu_stats[i].timerints = 0;
		smtc_cpu_stats[i].selfipis = 0;
	}

	atomic_set(&smtc_fpu_recoveries, 0);

	smtc_stats = create_proc_read_entry("smtc", 0444, NULL,
	                                    proc_read_smtc, NULL);
}
