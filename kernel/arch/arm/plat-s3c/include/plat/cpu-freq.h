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

/* arch/arm/plat-s3c/include/plat/cpu-freq.h
 *
 * Copyright (c) 2006,2007 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * S3C CPU frequency scaling support - driver and board
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/cpufreq.h>

struct s3c_cpufreq_info;
struct s3c_cpufreq_board;
struct s3c_iotimings;

struct s3c_freq {
	unsigned long	fclk;
	unsigned long	armclk;
	unsigned long	hclk_tns;	/* in 10ths of ns */
	unsigned long	hclk;
	unsigned long	pclk;
};

/* wrapper 'struct cpufreq_freqs' so that any drivers receiving the
 * notification can use this information that is not provided by just
 * having the core frequency alone.
 */

struct s3c_cpufreq_freqs {
	struct cpufreq_freqs	freqs;
	struct s3c_freq		old;
	struct s3c_freq		new;
};

#define to_s3c_cpufreq(_cf) container_of(_cf, struct s3c_cpufreq_freqs, freqs)

struct s3c_clkdivs {
	int		p_divisor;	/* fclk / pclk */
	int		h_divisor;	/* fclk / hclk */
	int		arm_divisor;	/* not all cpus have this. */
	unsigned char	dvs;		/* using dvs mode to arm. */
};

#define PLLVAL(_m, _p, _s) (((_m) << 12) | ((_p) << 4) | (_s))

struct s3c_pllval {
	unsigned long		freq;
	unsigned long		pll_reg;
};

struct s3c_cpufreq_config {
	struct s3c_freq		freq;
	struct s3c_pllval	pll;
	struct s3c_clkdivs	divs;
	struct s3c_cpufreq_info *info;	/* for core, not drivers */
	struct s3c_cpufreq_board *board;
};

/* s3c_cpufreq_board
 *
 * per-board configuraton information, such as memory refresh and
 * how to initialise IO timings.
 */
struct s3c_cpufreq_board {
	unsigned int	refresh;	/* refresh period in ns */
	unsigned int	auto_io:1;	/* automatically init io timings. */
	unsigned int	need_io:1;	/* set if needs io timing support. */

	/* any non-zero field in here is taken as an upper limit. */
	struct s3c_freq	max;	/* frequency limits */
};

/* Things depending on frequency scaling. */
#ifdef CONFIG_CPU_FREQ_S3C
#define __init_or_cpufreq
#else
#define __init_or_cpufreq __init
#endif

/* Board functions */

#ifdef CONFIG_CPU_FREQ_S3C
extern int s3c_cpufreq_setboard(struct s3c_cpufreq_board *board);
#else

static inline int s3c_cpufreq_setboard(struct s3c_cpufreq_board *board)
{
	return 0;
}
#endif  /* CONFIG_CPU_FREQ_S3C */
