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

#ifndef _ASM_IA64_FPSWA_H
#define _ASM_IA64_FPSWA_H

/*
 * Floating-point Software Assist
 *
 * Copyright (C) 1999 Intel Corporation.
 * Copyright (C) 1999 Asit Mallick <asit.k.mallick@intel.com>
 * Copyright (C) 1999 Goutham Rao <goutham.rao@intel.com>
 */

typedef struct {
	/* 4 * 128 bits */
	unsigned long fp_lp[4*2];
} fp_state_low_preserved_t;

typedef struct {
	/* 10 * 128 bits */
	unsigned long fp_lv[10 * 2];
} fp_state_low_volatile_t;

typedef	struct {
	/* 16 * 128 bits */
	unsigned long fp_hp[16 * 2];
} fp_state_high_preserved_t;

typedef struct {
	/* 96 * 128 bits */
	unsigned long fp_hv[96 * 2];
} fp_state_high_volatile_t;

/**
 * floating point state to be passed to the FP emulation library by
 * the trap/fault handler
 */
typedef struct {
	unsigned long			bitmask_low64;
	unsigned long			bitmask_high64;
	fp_state_low_preserved_t	*fp_state_low_preserved;
	fp_state_low_volatile_t		*fp_state_low_volatile;
	fp_state_high_preserved_t	*fp_state_high_preserved;
	fp_state_high_volatile_t	*fp_state_high_volatile;
} fp_state_t;

typedef struct {
	unsigned long status;
	unsigned long err0;
	unsigned long err1;
	unsigned long err2;
} fpswa_ret_t;

/**
 * function header for the Floating Point software assist
 * library. This function is invoked by the Floating point software
 * assist trap/fault handler.
 */
typedef fpswa_ret_t (*efi_fpswa_t) (unsigned long trap_type, void *bundle, unsigned long *ipsr,
				    unsigned long *fsr, unsigned long *isr, unsigned long *preds,
				    unsigned long *ifs, fp_state_t *fp_state);

/**
 * This is the FPSWA library interface as defined by EFI.  We need to pass a 
 * pointer to the interface itself on a call to the assist library
 */
typedef struct {
	unsigned int	 revision;
	unsigned int	 reserved;
	efi_fpswa_t	 fpswa;
} fpswa_interface_t;

extern fpswa_interface_t *fpswa_interface;

#endif /* _ASM_IA64_FPSWA_H */
