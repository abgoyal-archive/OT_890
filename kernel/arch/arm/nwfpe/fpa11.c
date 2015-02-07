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
    NetWinder Floating Point Emulator
    (c) Rebel.COM, 1998,1999
    (c) Philip Blundell, 2001

    Direct questions, comments to Scott Bambrough <scottb@netwinder.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "fpa11.h"
#include "fpopcode.h"

#include "fpmodule.h"
#include "fpmodule.inl"

#include <linux/compiler.h>
#include <linux/string.h>
#include <asm/system.h>

/* Reset the FPA11 chip.  Called to initialize and reset the emulator. */
static void resetFPA11(void)
{
	int i;
	FPA11 *fpa11 = GET_FPA11();

	/* initialize the register type array */
	for (i = 0; i <= 7; i++) {
		fpa11->fType[i] = typeNone;
	}

	/* FPSR: set system id to FP_EMULATOR, set AC, clear all other bits */
	fpa11->fpsr = FP_EMULATOR | BIT_AC;
}

int8 SetRoundingMode(const unsigned int opcode)
{
	switch (opcode & MASK_ROUNDING_MODE) {
	default:
	case ROUND_TO_NEAREST:
		return float_round_nearest_even;

	case ROUND_TO_PLUS_INFINITY:
		return float_round_up;

	case ROUND_TO_MINUS_INFINITY:
		return float_round_down;

	case ROUND_TO_ZERO:
		return float_round_to_zero;
	}
}

int8 SetRoundingPrecision(const unsigned int opcode)
{
#ifdef CONFIG_FPE_NWFPE_XP
	switch (opcode & MASK_ROUNDING_PRECISION) {
	case ROUND_SINGLE:
		return 32;

	case ROUND_DOUBLE:
		return 64;

	case ROUND_EXTENDED:
		return 80;

	default:
		return 80;
	}
#endif
	return 80;
}

void nwfpe_init_fpa(union fp_state *fp)
{
	FPA11 *fpa11 = (FPA11 *)fp;
#ifdef NWFPE_DEBUG
	printk("NWFPE: setting up state.\n");
#endif
 	memset(fpa11, 0, sizeof(FPA11));
	resetFPA11();
	fpa11->initflag = 1;
}

/* Emulate the instruction in the opcode. */
unsigned int EmulateAll(unsigned int opcode)
{
	unsigned int code;

#ifdef NWFPE_DEBUG
	printk("NWFPE: emulating opcode %08x\n", opcode);
#endif
	code = opcode & 0x00000f00;
	if (code == 0x00000100 || code == 0x00000200) {
		/* For coprocessor 1 or 2 (FPA11) */
		code = opcode & 0x0e000000;
		if (code == 0x0e000000) {
			if (opcode & 0x00000010) {
				/* Emulate conversion opcodes. */
				/* Emulate register transfer opcodes. */
				/* Emulate comparison opcodes. */
				return EmulateCPRT(opcode);
			} else {
				/* Emulate monadic arithmetic opcodes. */
				/* Emulate dyadic arithmetic opcodes. */
				return EmulateCPDO(opcode);
			}
		} else if (code == 0x0c000000) {
			/* Emulate load/store opcodes. */
			/* Emulate load/store multiple opcodes. */
			return EmulateCPDT(opcode);
		}
	}

	/* Invalid instruction detected.  Return FALSE. */
	return 0;
}
