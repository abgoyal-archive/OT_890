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
 * include/asm-sh/cpu-sh4/cache.h
 *
 * Copyright (C) 1999 Niibe Yutaka
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef __ASM_CPU_SH4_CACHE_H
#define __ASM_CPU_SH4_CACHE_H

#define L1_CACHE_SHIFT	5

#define SH_CACHE_VALID		1
#define SH_CACHE_UPDATED	2
#define SH_CACHE_COMBINED	4
#define SH_CACHE_ASSOC		8

#define CCR		0xff00001c	/* Address of Cache Control Register */
#define CCR_CACHE_OCE	0x0001	/* Operand Cache Enable */
#define CCR_CACHE_WT	0x0002	/* Write-Through (for P0,U0,P3) (else writeback)*/
#define CCR_CACHE_CB	0x0004	/* Copy-Back (for P1) (else writethrough) */
#define CCR_CACHE_OCI	0x0008	/* OC Invalidate */
#define CCR_CACHE_ORA	0x0020	/* OC RAM Mode */
#define CCR_CACHE_OIX	0x0080	/* OC Index Enable */
#define CCR_CACHE_ICE	0x0100	/* Instruction Cache Enable */
#define CCR_CACHE_ICI	0x0800	/* IC Invalidate */
#define CCR_CACHE_IIX	0x8000	/* IC Index Enable */
#ifndef CONFIG_CPU_SH4A
#define CCR_CACHE_EMODE	0x80000000	/* EMODE Enable */
#endif

/* Default CCR setup: 8k+16k-byte cache,P1-wb,enable */
#define CCR_CACHE_ENABLE	(CCR_CACHE_OCE|CCR_CACHE_ICE)
#define CCR_CACHE_INVALIDATE	(CCR_CACHE_OCI|CCR_CACHE_ICI)

#define CACHE_IC_ADDRESS_ARRAY	0xf0000000
#define CACHE_OC_ADDRESS_ARRAY	0xf4000000

#endif /* __ASM_CPU_SH4_CACHE_H */

