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

#include <linux/types.h>
#include <linux/errno.h>
#include <asm/uaccess.h>

#include <asm/sfp-machine.h>
#include <math-emu/soft-fp.h>

int
mtfsf(unsigned int FM, u32 *frB)
{
	u32 mask;
	u32 fpscr;

	if (FM == 0)
		return 0;

	if (FM == 0xff)
		mask = 0x9fffffff;
	else {
		mask = 0;
		if (FM & (1 << 0))
			mask |= 0x90000000;
		if (FM & (1 << 1))
			mask |= 0x0f000000;
		if (FM & (1 << 2))
			mask |= 0x00f00000;
		if (FM & (1 << 3))
			mask |= 0x000f0000;
		if (FM & (1 << 4))
			mask |= 0x0000f000;
		if (FM & (1 << 5))
			mask |= 0x00000f00;
		if (FM & (1 << 6))
			mask |= 0x000000f0;
		if (FM & (1 << 7))
			mask |= 0x0000000f;
	}

	__FPU_FPSCR &= ~(mask);
	__FPU_FPSCR |= (frB[1] & mask);

	__FPU_FPSCR &= ~(FPSCR_VX);
	if (__FPU_FPSCR & (FPSCR_VXSNAN | FPSCR_VXISI | FPSCR_VXIDI |
		     FPSCR_VXZDZ | FPSCR_VXIMZ | FPSCR_VXVC |
		     FPSCR_VXSOFT | FPSCR_VXSQRT | FPSCR_VXCVI))
		__FPU_FPSCR |= FPSCR_VX;

	fpscr = __FPU_FPSCR;
	fpscr &= ~(FPSCR_FEX);
	if (((fpscr & FPSCR_VX) && (fpscr & FPSCR_VE)) ||
	    ((fpscr & FPSCR_OX) && (fpscr & FPSCR_OE)) ||
	    ((fpscr & FPSCR_UX) && (fpscr & FPSCR_UE)) ||
	    ((fpscr & FPSCR_ZX) && (fpscr & FPSCR_ZE)) ||
	    ((fpscr & FPSCR_XX) && (fpscr & FPSCR_XE)))
		fpscr |= FPSCR_FEX;
	__FPU_FPSCR = fpscr;

#ifdef DEBUG
	printk("%s: %02x %p: %08lx\n", __func__, FM, frB, __FPU_FPSCR);
#endif

	return 0;
}
