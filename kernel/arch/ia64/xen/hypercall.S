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
 * Support routines for Xen hypercalls
 *
 * Copyright (C) 2005 Dan Magenheimer <dan.magenheimer@hp.com>
 * Copyright (C) 2008 Yaozu (Eddie) Dong <eddie.dong@intel.com>
 */

#include <asm/asmmacro.h>
#include <asm/intrinsics.h>
#include <asm/xen/privop.h>

/*
 * Hypercalls without parameter.
 */
#define __HCALL0(name,hcall)		\
	GLOBAL_ENTRY(name);		\
	break	hcall;			\
	br.ret.sptk.many rp;		\
	END(name)

/*
 * Hypercalls with 1 parameter.
 */
#define __HCALL1(name,hcall)		\
	GLOBAL_ENTRY(name);		\
	mov r8=r32;			\
	break	hcall;			\
	br.ret.sptk.many rp;		\
	END(name)

/*
 * Hypercalls with 2 parameters.
 */
#define __HCALL2(name,hcall)		\
	GLOBAL_ENTRY(name);		\
	mov r8=r32;			\
	mov r9=r33;			\
	break	hcall;			\
	br.ret.sptk.many rp;		\
	END(name)

__HCALL0(xen_get_psr, HYPERPRIVOP_GET_PSR)
__HCALL0(xen_get_ivr, HYPERPRIVOP_GET_IVR)
__HCALL0(xen_get_tpr, HYPERPRIVOP_GET_TPR)
__HCALL0(xen_hyper_ssm_i, HYPERPRIVOP_SSM_I)

__HCALL1(xen_set_tpr, HYPERPRIVOP_SET_TPR)
__HCALL1(xen_eoi, HYPERPRIVOP_EOI)
__HCALL1(xen_thash, HYPERPRIVOP_THASH)
__HCALL1(xen_set_itm, HYPERPRIVOP_SET_ITM)
__HCALL1(xen_get_rr, HYPERPRIVOP_GET_RR)
__HCALL1(xen_fc, HYPERPRIVOP_FC)
__HCALL1(xen_get_cpuid, HYPERPRIVOP_GET_CPUID)
__HCALL1(xen_get_pmd, HYPERPRIVOP_GET_PMD)

__HCALL2(xen_ptcga, HYPERPRIVOP_PTC_GA)
__HCALL2(xen_set_rr, HYPERPRIVOP_SET_RR)
__HCALL2(xen_set_kr, HYPERPRIVOP_SET_KR)

#ifdef CONFIG_IA32_SUPPORT
__HCALL0(xen_get_eflag, HYPERPRIVOP_GET_EFLAG)
__HCALL1(xen_set_eflag, HYPERPRIVOP_SET_EFLAG)	// refer SDM vol1 3.1.8
#endif /* CONFIG_IA32_SUPPORT */

GLOBAL_ENTRY(xen_set_rr0_to_rr4)
	mov r8=r32
	mov r9=r33
	mov r10=r34
	mov r11=r35
	mov r14=r36
	XEN_HYPER_SET_RR0_TO_RR4
	br.ret.sptk.many rp
	;;
END(xen_set_rr0_to_rr4)

GLOBAL_ENTRY(xen_send_ipi)
	mov r14=r32
	mov r15=r33
	mov r2=0x400
	break 0x1000
	;;
	br.ret.sptk.many rp
	;;
END(xen_send_ipi)

GLOBAL_ENTRY(__hypercall)
	mov r2=r37
	break 0x1000
	br.ret.sptk.many b0
	;;
END(__hypercall)
