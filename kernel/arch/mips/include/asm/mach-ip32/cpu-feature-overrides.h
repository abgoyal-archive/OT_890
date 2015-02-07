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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2005 Ilya A. Volynets-Evenbakh
 * Copyright (C) 2005, 07 Ralf Baechle (ralf@linux-mips.org)
 */
#ifndef __ASM_MACH_IP32_CPU_FEATURE_OVERRIDES_H
#define __ASM_MACH_IP32_CPU_FEATURE_OVERRIDES_H


/*
 * R5000 has an interesting "restriction":  ll(d)/sc(d)
 * instructions to XKPHYS region simply do uncached bus
 * requests. This breaks all the atomic bitops functions.
 * so, for 64bit IP32 kernel we just don't use ll/sc.
 * This does not affect luserland.
 */
#if (defined(CONFIG_CPU_R5000) || defined(CONFIG_CPU_NEVADA)) && defined(CONFIG_64BIT)
#define cpu_has_llsc		0
#else
#define cpu_has_llsc		1
#endif

/* Settings which are common for all ip32 CPUs */
#define cpu_has_tlb		1
#define cpu_has_4kex		1
#define cpu_has_fpu		1
#define cpu_has_32fpr		1
#define cpu_has_counter		1
#define cpu_has_mips16		0
#define cpu_has_vce		0
#define cpu_has_cache_cdex_s	0
#define cpu_has_mcheck		0
#define cpu_has_ejtag		0
#define cpu_has_vtag_icache	0
#define cpu_has_ic_fills_f_dc	0
#define cpu_has_dsp		0
#define cpu_has_4k_cache	1
#define cpu_has_mipsmt		0
#define cpu_has_userlocal	0


#define cpu_has_mips32r1	0
#define cpu_has_mips32r2	0
#define cpu_has_mips64r1	0
#define cpu_has_mips64r2	0

#endif /* __ASM_MACH_IP32_CPU_FEATURE_OVERRIDES_H */
