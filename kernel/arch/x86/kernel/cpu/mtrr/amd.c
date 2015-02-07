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

#include <linux/init.h>
#include <linux/mm.h>
#include <asm/mtrr.h>
#include <asm/msr.h>

#include "mtrr.h"

static void
amd_get_mtrr(unsigned int reg, unsigned long *base,
	     unsigned long *size, mtrr_type * type)
{
	unsigned long low, high;

	rdmsr(MSR_K6_UWCCR, low, high);
	/*  Upper dword is region 1, lower is region 0  */
	if (reg == 1)
		low = high;
	/*  The base masks off on the right alignment  */
	*base = (low & 0xFFFE0000) >> PAGE_SHIFT;
	*type = 0;
	if (low & 1)
		*type = MTRR_TYPE_UNCACHABLE;
	if (low & 2)
		*type = MTRR_TYPE_WRCOMB;
	if (!(low & 3)) {
		*size = 0;
		return;
	}
	/*
	 *  This needs a little explaining. The size is stored as an
	 *  inverted mask of bits of 128K granularity 15 bits long offset
	 *  2 bits
	 *
	 *  So to get a size we do invert the mask and add 1 to the lowest
	 *  mask bit (4 as its 2 bits in). This gives us a size we then shift
	 *  to turn into 128K blocks
	 *
	 *  eg              111 1111 1111 1100      is 512K
	 *
	 *  invert          000 0000 0000 0011
	 *  +1              000 0000 0000 0100
	 *  *128K   ...
	 */
	low = (~low) & 0x1FFFC;
	*size = (low + 4) << (15 - PAGE_SHIFT);
	return;
}

static void amd_set_mtrr(unsigned int reg, unsigned long base,
			 unsigned long size, mtrr_type type)
/*  [SUMMARY] Set variable MTRR register on the local CPU.
    <reg> The register to set.
    <base> The base address of the region.
    <size> The size of the region. If this is 0 the region is disabled.
    <type> The type of the region.
    [RETURNS] Nothing.
*/
{
	u32 regs[2];

	/*
	 *  Low is MTRR0 , High MTRR 1
	 */
	rdmsr(MSR_K6_UWCCR, regs[0], regs[1]);
	/*
	 *  Blank to disable
	 */
	if (size == 0)
		regs[reg] = 0;
	else
		/* Set the register to the base, the type (off by one) and an
		   inverted bitmask of the size The size is the only odd
		   bit. We are fed say 512K We invert this and we get 111 1111
		   1111 1011 but if you subtract one and invert you get the   
		   desired 111 1111 1111 1100 mask

		   But ~(x - 1) == ~x + 1 == -x. Two's complement rocks!  */
		regs[reg] = (-size >> (15 - PAGE_SHIFT) & 0x0001FFFC)
		    | (base << PAGE_SHIFT) | (type + 1);

	/*
	 *  The writeback rule is quite specific. See the manual. Its
	 *  disable local interrupts, write back the cache, set the mtrr
	 */
	wbinvd();
	wrmsr(MSR_K6_UWCCR, regs[0], regs[1]);
}

static int amd_validate_add_page(unsigned long base, unsigned long size, unsigned int type)
{
	/* Apply the K6 block alignment and size rules
	   In order
	   o Uncached or gathering only
	   o 128K or bigger block
	   o Power of 2 block
	   o base suitably aligned to the power
	*/
	if (type > MTRR_TYPE_WRCOMB || size < (1 << (17 - PAGE_SHIFT))
	    || (size & ~(size - 1)) - size || (base & (size - 1)))
		return -EINVAL;
	return 0;
}

static struct mtrr_ops amd_mtrr_ops = {
	.vendor            = X86_VENDOR_AMD,
	.set               = amd_set_mtrr,
	.get               = amd_get_mtrr,
	.get_free_region   = generic_get_free_region,
	.validate_add_page = amd_validate_add_page,
	.have_wrcomb       = positive_have_wrcomb,
};

int __init amd_init_mtrr(void)
{
	set_mtrr_ops(&amd_mtrr_ops);
	return 0;
}

//arch_initcall(amd_mtrr_init);
