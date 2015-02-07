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
 * @file op_model_mpcore.c
 * MPCORE Event Monitor Driver
 * @remark Copyright 2004 ARM SMP Development Team
 * @remark Copyright 2000-2004 Deepak Saxena <dsaxena@mvista.com>
 * @remark Copyright 2000-2004 MontaVista Software Inc
 * @remark Copyright 2004 Dave Jiang <dave.jiang@intel.com>
 * @remark Copyright 2004 Intel Corporation
 * @remark Copyright 2004 Zwane Mwaikambo <zwane@arm.linux.org.uk>
 * @remark Copyright 2004 Oprofile Authors
 *
 * @remark Read the file COPYING
 *
 * @author Zwane Mwaikambo
 */
#ifndef OP_MODEL_MPCORE_H
#define OP_MODEL_MPCORE_H

struct eventmonitor {
	unsigned long PMCR;
	unsigned char MCEB[8];
	unsigned long MC[8];
};

/*
 * List of userspace counter numbers: note that the structure is important.
 * The code relies on CPUn's counters being CPU0's counters + 3n
 * and on CPU0's counters starting at 0
 */

#define COUNTER_CPU0_PMN0 0
#define COUNTER_CPU0_PMN1 1
#define COUNTER_CPU0_CCNT 2

#define COUNTER_CPU1_PMN0 3
#define COUNTER_CPU1_PMN1 4
#define COUNTER_CPU1_CCNT 5

#define COUNTER_CPU2_PMN0 6
#define COUNTER_CPU2_PMN1 7
#define COUNTER_CPU2_CCNT 8

#define COUNTER_CPU3_PMN0 9
#define COUNTER_CPU3_PMN1 10
#define COUNTER_CPU3_CCNT 11

#define COUNTER_SCU_MN0 12
#define COUNTER_SCU_MN1 13
#define COUNTER_SCU_MN2 14
#define COUNTER_SCU_MN3 15
#define COUNTER_SCU_MN4 16
#define COUNTER_SCU_MN5 17
#define COUNTER_SCU_MN6 18
#define COUNTER_SCU_MN7 19
#define NUM_SCU_COUNTERS 8

#define SCU_COUNTER(number)	((number) + COUNTER_SCU_MN0)

#define MPCORE_NUM_COUNTERS	SCU_COUNTER(NUM_SCU_COUNTERS)

#endif
