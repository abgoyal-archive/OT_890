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
 * Copyright (C) 2003, 04, 07 Ralf Baechle (ralf@linux-mips.org)
 */
#ifndef __ASM_MACH_SIBYTE_CPU_FEATURE_OVERRIDES_H
#define __ASM_MACH_SIBYTE_CPU_FEATURE_OVERRIDES_H

/*
 * Sibyte are MIPS64 processors wired to a specific configuration
 */
#define cpu_has_watch		1
#define cpu_has_mips16		0
#define cpu_has_divec		1
#define cpu_has_vce		0
#define cpu_has_cache_cdex_p	0
#define cpu_has_cache_cdex_s	0
#define cpu_has_prefetch	1
#define cpu_has_mcheck		1
#define cpu_has_ejtag		1

#define cpu_has_llsc		1
#define cpu_has_vtag_icache	1
#define cpu_has_dc_aliases	0
#define cpu_has_ic_fills_f_dc	0
#define cpu_has_dsp		0
#define cpu_has_mipsmt		0
#define cpu_has_userlocal	0
#define cpu_icache_snoops_remote_store	0

#define cpu_has_nofpuex		0
#define cpu_has_64bits		1

#define cpu_has_mips32r1	1
#define cpu_has_mips32r2	0
#define cpu_has_mips64r1	1
#define cpu_has_mips64r2	0

#define cpu_has_inclusive_pcaches	0

#define cpu_dcache_line_size()	32
#define cpu_icache_line_size()	32
#define cpu_scache_line_size()	32

#endif /* __ASM_MACH_SIBYTE_CPU_FEATURE_OVERRIDES_H */
