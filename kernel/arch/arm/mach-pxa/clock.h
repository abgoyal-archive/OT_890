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

#include <asm/clkdev.h>

struct clkops {
	void			(*enable)(struct clk *);
	void			(*disable)(struct clk *);
	unsigned long		(*getrate)(struct clk *);
};

struct clk {
	const struct clkops	*ops;
	unsigned long		rate;
	unsigned int		cken;
	unsigned int		delay;
	unsigned int		enabled;
	struct clk		*other;
};

#define INIT_CLKREG(_clk,_devname,_conname)		\
	{						\
		.clk		= _clk,			\
		.dev_id		= _devname,		\
		.con_id		= _conname,		\
	}

#define DEFINE_CKEN(_name, _cken, _rate, _delay)	\
struct clk clk_##_name = {				\
		.ops	= &clk_cken_ops,		\
		.rate	= _rate,			\
		.cken	= CKEN_##_cken,			\
		.delay	= _delay,			\
	}

#define DEFINE_CK(_name, _cken, _ops)			\
struct clk clk_##_name = {				\
		.ops	= _ops,				\
		.cken	= CKEN_##_cken,			\
	}

#define DEFINE_CLK(_name, _ops, _rate, _delay)		\
struct clk clk_##_name = {				\
		.ops	= _ops, 			\
		.rate	= _rate,			\
		.delay	= _delay,			\
	}

extern const struct clkops clk_cken_ops;

void clk_cken_enable(struct clk *clk);
void clk_cken_disable(struct clk *clk);

#ifdef CONFIG_PXA3xx
#define DEFINE_PXA3_CKEN(_name, _cken, _rate, _delay)	\
struct clk clk_##_name = {				\
		.ops	= &clk_pxa3xx_cken_ops,		\
		.rate	= _rate,			\
		.cken	= CKEN_##_cken,			\
		.delay	= _delay,			\
	}

#define DEFINE_PXA3_CK(_name, _cken, _ops)		\
struct clk clk_##_name = {				\
		.ops	= _ops,				\
		.cken	= CKEN_##_cken,			\
	}

extern const struct clkops clk_pxa3xx_cken_ops;
extern void clk_pxa3xx_cken_enable(struct clk *);
extern void clk_pxa3xx_cken_disable(struct clk *);
#endif

void clks_register(struct clk_lookup *clks, size_t num);
int clk_add_alias(char *alias, struct device *alias_dev, char *id,
	struct device *dev);

