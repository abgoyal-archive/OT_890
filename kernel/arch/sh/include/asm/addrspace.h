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
 * Copyright (C) 1999 by Kaz Kojima
 *
 * Defitions for the address spaces of the SH CPUs.
 */
#ifndef __ASM_SH_ADDRSPACE_H
#define __ASM_SH_ADDRSPACE_H

#ifdef __KERNEL__

#include <cpu/addrspace.h>

/* If this CPU supports segmentation, hook up the helpers */
#ifdef P1SEG

/*
   [ P0/U0 (virtual) ]		0x00000000     <------ User space
   [ P1 (fixed)   cached ]	0x80000000     <------ Kernel space
   [ P2 (fixed)  non-cachable]	0xA0000000     <------ Physical access
   [ P3 (virtual) cached]	0xC0000000     <------ vmalloced area
   [ P4 control   ]		0xE0000000
 */

/* Returns the privileged segment base of a given address  */
#define PXSEG(a)	(((unsigned long)(a)) & 0xe0000000)

/* Returns the physical address of a PnSEG (n=1,2) address   */
#define PHYSADDR(a)	(((unsigned long)(a)) & 0x1fffffff)

#ifdef CONFIG_29BIT
/*
 * Map an address to a certain privileged segment
 */
#define P1SEGADDR(a)	\
	((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) | P1SEG))
#define P2SEGADDR(a)	\
	((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) | P2SEG))
#define P3SEGADDR(a)	\
	((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) | P3SEG))
#define P4SEGADDR(a)	\
	((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) | P4SEG))
#endif /* 29BIT */
#endif /* P1SEG */

/* Check if an address can be reached in 29 bits */
#define IS_29BIT(a)	(((unsigned long)(a)) < 0x20000000)

#ifdef CONFIG_SH_STORE_QUEUES
/*
 * This is a special case for the SH-4 store queues, as pages for this
 * space still need to be faulted in before it's possible to flush the
 * store queue cache for writeout to the remapped region.
 */
#define P3_ADDR_MAX		(P4SEG_STORE_QUE + 0x04000000)
#else
#define P3_ADDR_MAX		P4SEG
#endif

#endif /* __KERNEL__ */
#endif /* __ASM_SH_ADDRSPACE_H */
