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
 *  arch/s390/kernel/processor.c
 *
 *  Copyright IBM Corp. 2008
 *  Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com)
 */

#define KMSG_COMPONENT "cpu"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/seq_file.h>
#include <linux/delay.h>

#include <asm/elf.h>
#include <asm/lowcore.h>
#include <asm/param.h>

void __cpuinit print_cpu_info(struct cpuinfo_S390 *cpuinfo)
{
	pr_info("Processor %d started, address %d, identification %06X\n",
		cpuinfo->cpu_nr, cpuinfo->cpu_addr, cpuinfo->cpu_id.ident);
}

/*
 * show_cpuinfo - Get information on one CPU for use by procfs.
 */

static int show_cpuinfo(struct seq_file *m, void *v)
{
	static const char *hwcap_str[8] = {
		"esan3", "zarch", "stfle", "msa", "ldisp", "eimm", "dfp",
		"edat"
	};
       struct cpuinfo_S390 *cpuinfo;
       unsigned long n = (unsigned long) v - 1;
       int i;

       s390_adjust_jiffies();
       preempt_disable();
       if (!n) {
	       seq_printf(m, "vendor_id       : IBM/S390\n"
			  "# processors    : %i\n"
			  "bogomips per cpu: %lu.%02lu\n",
			  num_online_cpus(), loops_per_jiffy/(500000/HZ),
			  (loops_per_jiffy/(5000/HZ))%100);
	       seq_puts(m, "features\t: ");
	       for (i = 0; i < 8; i++)
		       if (hwcap_str[i] && (elf_hwcap & (1UL << i)))
			       seq_printf(m, "%s ", hwcap_str[i]);
	       seq_puts(m, "\n");
       }

       if (cpu_online(n)) {
#ifdef CONFIG_SMP
	       if (smp_processor_id() == n)
		       cpuinfo = &S390_lowcore.cpu_data;
	       else
		       cpuinfo = &lowcore_ptr[n]->cpu_data;
#else
	       cpuinfo = &S390_lowcore.cpu_data;
#endif
	       seq_printf(m, "processor %li: "
			  "version = %02X,  "
			  "identification = %06X,  "
			  "machine = %04X\n",
			  n, cpuinfo->cpu_id.version,
			  cpuinfo->cpu_id.ident,
			  cpuinfo->cpu_id.machine);
       }
       preempt_enable();
       return 0;
}

static void *c_start(struct seq_file *m, loff_t *pos)
{
	return *pos < NR_CPUS ? (void *)((unsigned long) *pos + 1) : NULL;
}

static void *c_next(struct seq_file *m, void *v, loff_t *pos)
{
	++*pos;
	return c_start(m, pos);
}

static void c_stop(struct seq_file *m, void *v)
{
}

const struct seq_operations cpuinfo_op = {
	.start	= c_start,
	.next	= c_next,
	.stop	= c_stop,
	.show	= show_cpuinfo,
};

