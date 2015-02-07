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

#ifndef _SPARC_VAC_OPS_H
#define _SPARC_VAC_OPS_H

/* vac-ops.h: Inline assembly routines to do operations on the Sparc
 *            VAC (virtual address cache) for the sun4c.
 *
 * Copyright (C) 1994, David S. Miller (davem@caip.rutgers.edu)
 */

#include <asm/sysen.h>
#include <asm/contregs.h>
#include <asm/asi.h>

/* The SUN4C models have a virtually addressed write-through
 * cache.
 *
 * The cache tags are directly accessible through an ASI and
 * each have the form:
 *
 * ------------------------------------------------------------
 * | MBZ | CONTEXT | WRITE | PRIV | VALID | MBZ | TagID | MBZ |
 * ------------------------------------------------------------
 *  31 25  24   22     21     20     19    18 16  15   2  1  0
 *
 * MBZ: These bits are either unused and/or reserved and should
 *      be written as zeroes.
 *
 * CONTEXT: Records the context to which this cache line belongs.
 *
 * WRITE: A copy of the writable bit from the mmu pte access bits.
 *
 * PRIV: A copy of the privileged bit from the pte access bits.
 *
 * VALID: If set, this line is valid, else invalid.
 *
 * TagID: Fourteen bits of tag ID.
 *
 * Every virtual address is seen by the cache like this:
 *
 * ----------------------------------------
 * |  RESV  | TagID | LINE | BYTE-in-LINE |
 * ----------------------------------------
 *  31    30 29   16 15   4 3            0
 *
 * RESV: Unused/reserved.
 *
 * TagID: Used to match the Tag-ID in that vac tags.
 *
 * LINE: Which line within the cache
 *
 * BYTE-in-LINE: Which byte within the cache line.
 */

/* Sun4c VAC Tags */
#define S4CVACTAG_CID      0x01c00000
#define S4CVACTAG_W        0x00200000
#define S4CVACTAG_P        0x00100000
#define S4CVACTAG_V        0x00080000
#define S4CVACTAG_TID      0x0000fffc

/* Sun4c VAC Virtual Address */
/* These aren't used, why bother? (Anton) */
#if 0
#define S4CVACVA_TID       0x3fff0000
#define S4CVACVA_LINE      0x0000fff0
#define S4CVACVA_BIL       0x0000000f
#endif

/* The indexing of cache lines creates a problem.  Because the line
 * field of a virtual address extends past the page offset within
 * the virtual address it is possible to have what are called
 * 'bad aliases' which will create inconsistencies.  So we must make
 * sure that within a context that if a physical page is mapped
 * more than once, that 'extra' line bits are the same.  If this is
 * not the case, and thus is a 'bad alias' we must turn off the
 * cacheable bit in the pte's of all such pages.
 */

#define S4CVAC_BADBITS    0x0000f000

/* The following is true if vaddr1 and vaddr2 would cause
 * a 'bad alias'.
 */
#define S4CVAC_BADALIAS(vaddr1, vaddr2) \
        ((((unsigned long) (vaddr1)) ^ ((unsigned long) (vaddr2))) & \
	 (S4CVAC_BADBITS))

/* The following structure describes the characteristics of a sun4c
 * VAC as probed from the prom during boot time.
 */
struct sun4c_vac_props {
	unsigned int num_bytes;     /* Size of the cache */
	unsigned int do_hwflushes;  /* Hardware flushing available? */
	unsigned int linesize;      /* Size of each line in bytes */
	unsigned int log2lsize;     /* log2(linesize) */
	unsigned int on;            /* VAC is enabled */
};

extern struct sun4c_vac_props sun4c_vacinfo;

/* sun4c_enable_vac() enables the sun4c virtual address cache. */
static inline void sun4c_enable_vac(void)
{
	__asm__ __volatile__("lduba [%0] %1, %%g1\n\t"
			     "or    %%g1, %2, %%g1\n\t"
			     "stba  %%g1, [%0] %1\n\t"
			     : /* no outputs */
			     : "r" ((unsigned int) AC_SENABLE),
			     "i" (ASI_CONTROL), "i" (SENABLE_CACHE)
			     : "g1", "memory");
	sun4c_vacinfo.on = 1;
}

/* sun4c_disable_vac() disables the virtual address cache. */
static inline void sun4c_disable_vac(void)
{
	__asm__ __volatile__("lduba [%0] %1, %%g1\n\t"
			     "andn  %%g1, %2, %%g1\n\t"
			     "stba  %%g1, [%0] %1\n\t"
			     : /* no outputs */
			     : "r" ((unsigned int) AC_SENABLE),
			     "i" (ASI_CONTROL), "i" (SENABLE_CACHE)
			     : "g1", "memory");
	sun4c_vacinfo.on = 0;
}

#endif /* !(_SPARC_VAC_OPS_H) */
