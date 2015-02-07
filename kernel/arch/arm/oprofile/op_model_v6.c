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

/**
 * @file op_model_v6.c
 * ARM11 Performance Monitor Driver
 *
 * Based on op_model_xscale.c
 *
 * @remark Copyright 2000-2004 Deepak Saxena <dsaxena@mvista.com>
 * @remark Copyright 2000-2004 MontaVista Software Inc
 * @remark Copyright 2004 Dave Jiang <dave.jiang@intel.com>
 * @remark Copyright 2004 Intel Corporation
 * @remark Copyright 2004 Zwane Mwaikambo <zwane@arm.linux.org.uk>
 * @remark Copyright 2004 OProfile Authors
 *
 * @remark Read the file COPYING
 *
 * @author Tony Lindgren <tony@atomide.com>
 */

/* #define DEBUG */
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/oprofile.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <asm/system.h>

#include "op_counter.h"
#include "op_arm_model.h"
#include "op_model_arm11_core.h"

static int irqs[] = {
#ifdef CONFIG_ARCH_OMAP2
	3,
#endif
};

static void armv6_pmu_stop(void)
{
	arm11_stop_pmu();
	arm11_release_interrupts(irqs, ARRAY_SIZE(irqs));
}

static int armv6_pmu_start(void)
{
	int ret;

	ret = arm11_request_interrupts(irqs, ARRAY_SIZE(irqs));
	if (ret >= 0)
		ret = arm11_start_pmu();

	return ret;
}

static int armv6_detect_pmu(void)
{
	return 0;
}

struct op_arm_model_spec op_armv6_spec = {
	.init		= armv6_detect_pmu,
	.num_counters	= 3,
	.setup_ctrs	= arm11_setup_pmu,
	.start		= armv6_pmu_start,
	.stop		= armv6_pmu_stop,
	.name		= "arm/armv6",
};
