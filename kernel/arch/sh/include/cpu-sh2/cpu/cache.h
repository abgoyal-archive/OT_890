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
 * include/asm-sh/cpu-sh2/cache.h
 *
 * Copyright (C) 2003 Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef __ASM_CPU_SH2_CACHE_H
#define __ASM_CPU_SH2_CACHE_H

#define L1_CACHE_SHIFT	4

#define SH_CACHE_VALID		1
#define SH_CACHE_UPDATED	2
#define SH_CACHE_COMBINED	4
#define SH_CACHE_ASSOC		8

#if defined(CONFIG_CPU_SUBTYPE_SH7619)
#define CCR		0xffffffec

#define CCR_CACHE_CE	0x01	/* Cache enable */
#define CCR_CACHE_WT	0x02    /* CCR[bit1=1,bit2=1] */
				/* 0x00000000-0x7fffffff: Write-through  */
				/* 0x80000000-0x9fffffff: Write-back     */
                                /* 0xc0000000-0xdfffffff: Write-through  */
#define CCR_CACHE_CB	0x04    /* CCR[bit1=0,bit2=0] */
				/* 0x00000000-0x7fffffff: Write-back     */
				/* 0x80000000-0x9fffffff: Write-through  */
                                /* 0xc0000000-0xdfffffff: Write-back     */
#define CCR_CACHE_CF	0x08	/* Cache invalidate */

#define CACHE_OC_ADDRESS_ARRAY	0xf0000000
#define CACHE_OC_DATA_ARRAY	0xf1000000

#define CCR_CACHE_ENABLE	CCR_CACHE_CE
#define CCR_CACHE_INVALIDATE	CCR_CACHE_CF
#define CACHE_PHYSADDR_MASK	0x1ffffc00

#endif

#endif /* __ASM_CPU_SH2_CACHE_H */
