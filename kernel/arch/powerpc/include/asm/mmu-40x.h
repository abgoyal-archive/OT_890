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

#ifndef _ASM_POWERPC_MMU_40X_H_
#define _ASM_POWERPC_MMU_40X_H_

/*
 * PPC40x support
 */

#define PPC40X_TLB_SIZE 64

/*
 * TLB entries are defined by a "high" tag portion and a "low" data
 * portion.  On all architectures, the data portion is 32-bits.
 *
 * TLB entries are managed entirely under software control by reading,
 * writing, and searchoing using the 4xx-specific tlbre, tlbwr, and tlbsx
 * instructions.
 */

#define	TLB_LO          1
#define	TLB_HI          0

#define	TLB_DATA        TLB_LO
#define	TLB_TAG         TLB_HI

/* Tag portion */

#define TLB_EPN_MASK    0xFFFFFC00      /* Effective Page Number */
#define TLB_PAGESZ_MASK 0x00000380
#define TLB_PAGESZ(x)   (((x) & 0x7) << 7)
#define   PAGESZ_1K		0
#define   PAGESZ_4K             1
#define   PAGESZ_16K            2
#define   PAGESZ_64K            3
#define   PAGESZ_256K           4
#define   PAGESZ_1M             5
#define   PAGESZ_4M             6
#define   PAGESZ_16M            7
#define TLB_VALID       0x00000040      /* Entry is valid */

/* Data portion */

#define TLB_RPN_MASK    0xFFFFFC00      /* Real Page Number */
#define TLB_PERM_MASK   0x00000300
#define TLB_EX          0x00000200      /* Instruction execution allowed */
#define TLB_WR          0x00000100      /* Writes permitted */
#define TLB_ZSEL_MASK   0x000000F0
#define TLB_ZSEL(x)     (((x) & 0xF) << 4)
#define TLB_ATTR_MASK   0x0000000F
#define TLB_W           0x00000008      /* Caching is write-through */
#define TLB_I           0x00000004      /* Caching is inhibited */
#define TLB_M           0x00000002      /* Memory is coherent */
#define TLB_G           0x00000001      /* Memory is guarded from prefetch */

#ifndef __ASSEMBLY__

typedef struct {
	unsigned int	id;
	unsigned int	active;
	unsigned long	vdso_base;
} mm_context_t;

#endif /* !__ASSEMBLY__ */

#endif /* _ASM_POWERPC_MMU_40X_H_ */
