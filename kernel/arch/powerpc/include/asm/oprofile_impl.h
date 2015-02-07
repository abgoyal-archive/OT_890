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
 * Copyright (C) 2004 Anton Blanchard <anton@au.ibm.com>, IBM
 *
 * Based on alpha version.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef _ASM_POWERPC_OPROFILE_IMPL_H
#define _ASM_POWERPC_OPROFILE_IMPL_H
#ifdef __KERNEL__

#define OP_MAX_COUNTER 8

/* Per-counter configuration as set via oprofilefs.  */
struct op_counter_config {
	unsigned long enabled;
	unsigned long event;
	unsigned long count;
	/* Classic doesn't support per-counter user/kernel selection */
	unsigned long kernel;
	unsigned long user;
	unsigned long unit_mask;
};

/* System-wide configuration as set via oprofilefs.  */
struct op_system_config {
#ifdef CONFIG_PPC64
	unsigned long mmcr0;
	unsigned long mmcr1;
	unsigned long mmcra;
#ifdef CONFIG_OPROFILE_CELL
	/* Register for oprofile user tool to check cell kernel profiling
	 * suport.
	 */
	unsigned long cell_support;
#endif
#endif
	unsigned long enable_kernel;
	unsigned long enable_user;
};

/* Per-arch configuration */
struct op_powerpc_model {
	int (*reg_setup) (struct op_counter_config *,
			   struct op_system_config *,
			   int num_counters);
	int  (*cpu_setup) (struct op_counter_config *);
	int  (*start) (struct op_counter_config *);
	int  (*global_start) (struct op_counter_config *);
	void (*stop) (void);
	void (*global_stop) (void);
	int (*sync_start)(void);
	int (*sync_stop)(void);
	void (*handle_interrupt) (struct pt_regs *,
				  struct op_counter_config *);
	int num_counters;
};

extern struct op_powerpc_model op_model_fsl_emb;
extern struct op_powerpc_model op_model_rs64;
extern struct op_powerpc_model op_model_power4;
extern struct op_powerpc_model op_model_7450;
extern struct op_powerpc_model op_model_cell;
extern struct op_powerpc_model op_model_pa6t;


/* All the classic PPC parts use these */
static inline unsigned int classic_ctr_read(unsigned int i)
{
	switch(i) {
	case 0:
		return mfspr(SPRN_PMC1);
	case 1:
		return mfspr(SPRN_PMC2);
	case 2:
		return mfspr(SPRN_PMC3);
	case 3:
		return mfspr(SPRN_PMC4);
	case 4:
		return mfspr(SPRN_PMC5);
	case 5:
		return mfspr(SPRN_PMC6);

/* No PPC32 chip has more than 6 so far */
#ifdef CONFIG_PPC64
	case 6:
		return mfspr(SPRN_PMC7);
	case 7:
		return mfspr(SPRN_PMC8);
#endif
	default:
		return 0;
	}
}

static inline void classic_ctr_write(unsigned int i, unsigned int val)
{
	switch(i) {
	case 0:
		mtspr(SPRN_PMC1, val);
		break;
	case 1:
		mtspr(SPRN_PMC2, val);
		break;
	case 2:
		mtspr(SPRN_PMC3, val);
		break;
	case 3:
		mtspr(SPRN_PMC4, val);
		break;
	case 4:
		mtspr(SPRN_PMC5, val);
		break;
	case 5:
		mtspr(SPRN_PMC6, val);
		break;

/* No PPC32 chip has more than 6, yet */
#ifdef CONFIG_PPC64
	case 6:
		mtspr(SPRN_PMC7, val);
		break;
	case 7:
		mtspr(SPRN_PMC8, val);
		break;
#endif
	default:
		break;
	}
}


extern void op_powerpc_backtrace(struct pt_regs * const regs, unsigned int depth);

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_OPROFILE_IMPL_H */
