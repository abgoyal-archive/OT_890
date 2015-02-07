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
 * @file arch/alpha/oprofile/op_impl.h
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Richard Henderson <rth@twiddle.net>
 */

#ifndef OP_IMPL_H
#define OP_IMPL_H 1

/* Per-counter configuration as set via oprofilefs.  */
struct op_counter_config {
	unsigned long enabled;
	unsigned long event;
	unsigned long count;
	/* Dummies because I am too lazy to hack the userspace tools.  */
	unsigned long kernel;
	unsigned long user;
	unsigned long unit_mask;
};

/* System-wide configuration as set via oprofilefs.  */
struct op_system_config {
	unsigned long enable_pal;
	unsigned long enable_kernel;
	unsigned long enable_user;
};

/* Cached values for the various performance monitoring registers.  */
struct op_register_config {
	unsigned long enable;
	unsigned long mux_select;
	unsigned long proc_mode;
	unsigned long freq;
	unsigned long reset_values;
	unsigned long need_reset;
};

/* Per-architecture configuration and hooks.  */
struct op_axp_model {
	void (*reg_setup) (struct op_register_config *,
			   struct op_counter_config *,
			   struct op_system_config *);
	void (*cpu_setup) (void *);
	void (*reset_ctr) (struct op_register_config *, unsigned long);
	void (*handle_interrupt) (unsigned long, struct pt_regs *,
				  struct op_counter_config *);
	char *cpu_type;
	unsigned char num_counters;
	unsigned char can_set_proc_mode;
};

#endif
