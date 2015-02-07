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

unsigned int SingleCPDO(struct roundingData *roundData, const unsigned int opcode, FPREG * rFd);
unsigned int DoubleCPDO(struct roundingData *roundData, const unsigned int opcode, FPREG * rFd);
unsigned int ExtendedCPDO(struct roundingData *roundData, const unsigned int opcode, FPREG * rFd);

unsigned int EmulateCPDO(const unsigned int opcode)
{
	FPA11 *fpa11 = GET_FPA11();
	FPREG *rFd;
	unsigned int nType, nDest, nRc;
	struct roundingData roundData;

	/* Get the destination size.  If not valid let Linux perform
	   an invalid instruction trap. */
	nDest = getDestinationSize(opcode);
	if (typeNone == nDest)
		return 0;

	roundData.mode = SetRoundingMode(opcode);
	roundData.precision = SetRoundingPrecision(opcode);
	roundData.exception = 0;

	/* Compare the size of the operands in Fn and Fm.
	   Choose the largest size and perform operations in that size,
	   in order to make use of all the precision of the operands.
	   If Fm is a constant, we just grab a constant of a size
	   matching the size of the operand in Fn. */
	if (MONADIC_INSTRUCTION(opcode))
		nType = nDest;
	else
		nType = fpa11->fType[getFn(opcode)];

	if (!CONSTANT_FM(opcode)) {
		register unsigned int Fm = getFm(opcode);
		if (nType < fpa11->fType[Fm]) {
			nType = fpa11->fType[Fm];
		}
	}

	rFd = &fpa11->fpreg[getFd(opcode)];

	switch (nType) {
	case typeSingle:
		nRc = SingleCPDO(&roundData, opcode, rFd);
		break;
	case typeDouble:
		nRc = DoubleCPDO(&roundData, opcode, rFd);
		break;
#ifdef CONFIG_FPE_NWFPE_XP
	case typeExtended:
		nRc = ExtendedCPDO(&roundData, opcode, rFd);
		break;
#endif
	default:
		nRc = 0;
	}

	/* The CPDO functions used to always set the destination type
	   to be the same as their working size. */

	if (nRc != 0) {
		/* If the operation succeeded, check to see if the result in the
		   destination register is the correct size.  If not force it
		   to be. */

		fpa11->fType[getFd(opcode)] = nDest;

#ifdef CONFIG_FPE_NWFPE_XP
		if (nDest != nType) {
			switch (nDest) {
			case typeSingle:
				{
					if (typeDouble == nType)
						rFd->fSingle = float64_to_float32(&roundData, rFd->fDouble);
					else
						rFd->fSingle = floatx80_to_float32(&roundData, rFd->fExtended);
				}
				break;

			case typeDouble:
				{
					if (typeSingle == nType)
						rFd->fDouble = float32_to_float64(rFd->fSingle);
					else
						rFd->fDouble = floatx80_to_float64(&roundData, rFd->fExtended);
				}
				break;

			case typeExtended:
				{
					if (typeSingle == nType)
						rFd->fExtended = float32_to_floatx80(rFd->fSingle);
					else
						rFd->fExtended = float64_to_floatx80(rFd->fDouble);
				}
				break;
			}
		}
#else
		if (nDest != nType) {
			if (nDest == typeSingle)
				rFd->fSingle = float64_to_float32(&roundData, rFd->fDouble);
			else
				rFd->fDouble = float32_to_float64(rFd->fSingle);
		}
#endif
	}

	if (roundData.exception)
		float_raise(roundData.exception);

	return nRc;
}
