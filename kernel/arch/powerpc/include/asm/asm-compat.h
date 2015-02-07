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

#ifndef _ASM_POWERPC_ASM_COMPAT_H
#define _ASM_POWERPC_ASM_COMPAT_H

#include <asm/types.h>

#ifdef __ASSEMBLY__
#  define stringify_in_c(...)	__VA_ARGS__
#  define ASM_CONST(x)		x
#else
/* This version of stringify will deal with commas... */
#  define __stringify_in_c(...)	#__VA_ARGS__
#  define stringify_in_c(...)	__stringify_in_c(__VA_ARGS__) " "
#  define __ASM_CONST(x)	x##UL
#  define ASM_CONST(x)		__ASM_CONST(x)
#endif


#ifdef __powerpc64__

/* operations for longs and pointers */
#define PPC_LL		stringify_in_c(ld)
#define PPC_STL		stringify_in_c(std)
#define PPC_LCMPI	stringify_in_c(cmpdi)
#define PPC_LONG	stringify_in_c(.llong)
#define PPC_LONG_ALIGN	stringify_in_c(.balign 8)
#define PPC_TLNEI	stringify_in_c(tdnei)
#define PPC_LLARX	stringify_in_c(ldarx)
#define PPC_STLCX	stringify_in_c(stdcx.)
#define PPC_CNTLZL	stringify_in_c(cntlzd)

/* Move to CR, single-entry optimized version. Only available
 * on POWER4 and later.
 */
#ifdef CONFIG_POWER4_ONLY
#define PPC_MTOCRF	stringify_in_c(mtocrf)
#else
#define PPC_MTOCRF	stringify_in_c(mtcrf)
#endif

#else /* 32-bit */

/* operations for longs and pointers */
#define PPC_LL		stringify_in_c(lwz)
#define PPC_STL		stringify_in_c(stw)
#define PPC_LCMPI	stringify_in_c(cmpwi)
#define PPC_LONG	stringify_in_c(.long)
#define PPC_LONG_ALIGN	stringify_in_c(.balign 4)
#define PPC_TLNEI	stringify_in_c(twnei)
#define PPC_LLARX	stringify_in_c(lwarx)
#define PPC_STLCX	stringify_in_c(stwcx.)
#define PPC_CNTLZL	stringify_in_c(cntlzw)
#define PPC_MTOCRF	stringify_in_c(mtcrf)

#endif

#ifdef __KERNEL__
#ifdef CONFIG_IBM405_ERR77
/* Erratum #77 on the 405 means we need a sync or dcbt before every
 * stwcx.  The old ATOMIC_SYNC_FIX covered some but not all of this.
 */
#define PPC405_ERR77(ra,rb)	stringify_in_c(dcbt	ra, rb;)
#define	PPC405_ERR77_SYNC	stringify_in_c(sync;)
#else
#define PPC405_ERR77(ra,rb)
#define PPC405_ERR77_SYNC
#endif
#endif

#endif /* _ASM_POWERPC_ASM_COMPAT_H */
