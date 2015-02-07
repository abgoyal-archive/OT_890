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
 * SGI IP27 specific setup.
 *
 * Copyright (C) 1995 - 1997, 1999 Silcon Graphics, Inc.
 * Copyright (C) 1999 Ralf Baechle (ralf@gnu.org)
 */
#ifndef _ASM_SN_SN0_ARCH_H
#define _ASM_SN_SN0_ARCH_H


#ifndef SN0XXL  /* 128 cpu SMP max */
/*
 * This is the maximum number of nodes that can be part of a kernel.
 * Effectively, it's the maximum number of compact node ids (cnodeid_t).
 */
#define MAX_COMPACT_NODES       64

/*
 * MAXCPUS refers to the maximum number of CPUs in a single kernel.
 * This is not necessarily the same as MAXNODES * CPUS_PER_NODE
 */
#define MAXCPUS                 128

#else /* SN0XXL system */

#define MAX_COMPACT_NODES       128
#define MAXCPUS                 256

#endif /* SN0XXL */

/*
 * This is the maximum number of NASIDS that can be present in a system.
 * (Highest NASID plus one.)
 */
#define MAX_NASIDS		256

/*
 * MAX_REGIONS refers to the maximum number of hardware partitioned regions.
 */
#define	MAX_REGIONS		64
#define MAX_NONPREMIUM_REGIONS  16
#define MAX_PREMIUM_REGIONS     MAX_REGIONS

/*
 * MAX_PARITIONS refers to the maximum number of logically defined
 * partitions the system can support.
 */
#define MAX_PARTITIONS		MAX_REGIONS

#define NASID_MASK_BYTES	((MAX_NASIDS + 7) / 8)

/*
 * Slot constants for SN0
 */
#ifdef CONFIG_SGI_SN_N_MODE
#define MAX_MEM_SLOTS   16                      /* max slots per node */
#else /* !CONFIG_SGI_SN_N_MODE, assume CONFIG_SGI_SN_M_MODE */
#define MAX_MEM_SLOTS   32                      /* max slots per node */
#endif /* CONFIG_SGI_SN_M_MODE */

#define SLOT_SHIFT      	(27)
#define SLOT_MIN_MEM_SIZE	(32*1024*1024)

#define CPUS_PER_NODE		2	/* CPUs on a single hub */
#define CPUS_PER_NODE_SHFT	1	/* Bits to shift in the node number */
#define CPUS_PER_SUBNODE	2	/* CPUs on a single hub PI */

#endif /* _ASM_SN_SN0_ARCH_H */
