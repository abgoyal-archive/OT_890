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

#ifndef __ASM_SH_CLOCK_H
#define __ASM_SH_CLOCK_H

#include <linux/kref.h>
#include <linux/list.h>
#include <linux/seq_file.h>
#include <linux/clk.h>
#include <linux/err.h>

struct clk;

struct clk_ops {
	void (*init)(struct clk *clk);
	void (*enable)(struct clk *clk);
	void (*disable)(struct clk *clk);
	void (*recalc)(struct clk *clk);
	int (*set_rate)(struct clk *clk, unsigned long rate, int algo_id);
	long (*round_rate)(struct clk *clk, unsigned long rate);
};

struct clk {
	struct list_head	node;
	const char		*name;
	int			id;
	struct module		*owner;

	struct clk		*parent;
	struct clk_ops		*ops;

	struct kref		kref;

	unsigned long		rate;
	unsigned long		flags;
	unsigned long		arch_flags;
};

#define CLK_ALWAYS_ENABLED	(1 << 0)
#define CLK_RATE_PROPAGATES	(1 << 1)

/* Should be defined by processor-specific code */
void arch_init_clk_ops(struct clk_ops **, int type);
int __init arch_clk_init(void);

/* arch/sh/kernel/cpu/clock.c */
int clk_init(void);

void clk_recalc_rate(struct clk *);

int clk_register(struct clk *);
void clk_unregister(struct clk *);

static inline int clk_always_enable(const char *id)
{
	struct clk *clk;
	int ret;

	clk = clk_get(NULL, id);
	if (IS_ERR(clk))
		return PTR_ERR(clk);

	ret = clk_enable(clk);
	if (ret)
		clk_put(clk);

	return ret;
}

/* the exported API, in addition to clk_set_rate */
/**
 * clk_set_rate_ex - set the clock rate for a clock source, with additional parameter
 * @clk: clock source
 * @rate: desired clock rate in Hz
 * @algo_id: algorithm id to be passed down to ops->set_rate
 *
 * Returns success (0) or negative errno.
 */
int clk_set_rate_ex(struct clk *clk, unsigned long rate, int algo_id);

enum clk_sh_algo_id {
	NO_CHANGE = 0,

	IUS_N1_N1,
	IUS_322,
	IUS_522,
	IUS_N11,

	SB_N1,

	SB3_N1,
	SB3_32,
	SB3_43,
	SB3_54,

	BP_N1,

	IP_N1,
};
#endif /* __ASM_SH_CLOCK_H */
