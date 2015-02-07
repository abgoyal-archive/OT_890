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

/*---------------------------------------------------------------------------+
 |  poly_atan.c                                                              |
 |                                                                           |
 | Compute the arctan of a FPU_REG, using a polynomial approximation.        |
 |                                                                           |
 | Copyright (C) 1992,1993,1994,1997                                         |
 |                  W. Metzenthen, 22 Parker St, Ormond, Vic 3163, Australia |
 |                  E-mail   billm@suburbia.net                              |
 |                                                                           |
 |                                                                           |
 +---------------------------------------------------------------------------*/

#include "exception.h"
#include "reg_constant.h"
#include "fpu_emu.h"
#include "fpu_system.h"
#include "status_w.h"
#include "control_w.h"
#include "poly.h"

#define	HIPOWERon	6	/* odd poly, negative terms */
static const unsigned long long oddnegterms[HIPOWERon] = {
	0x0000000000000000LL,	/* Dummy (not for - 1.0) */
	0x015328437f756467LL,
	0x0005dda27b73dec6LL,
	0x0000226bf2bfb91aLL,
	0x000000ccc439c5f7LL,
	0x0000000355438407LL
};

#define	HIPOWERop	6	/* odd poly, positive terms */
static const unsigned long long oddplterms[HIPOWERop] = {
/*  0xaaaaaaaaaaaaaaabLL,  transferred to fixedpterm[] */
	0x0db55a71875c9ac2LL,
	0x0029fce2d67880b0LL,
	0x0000dfd3908b4596LL,
	0x00000550fd61dab4LL,
	0x0000001c9422b3f9LL,
	0x000000003e3301e1LL
};

static const unsigned long long denomterm = 0xebd9b842c5c53a0eLL;

static const Xsig fixedpterm = MK_XSIG(0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa);

static const Xsig pi_signif = MK_XSIG(0xc90fdaa2, 0x2168c234, 0xc4c6628b);

/*--- poly_atan() -----------------------------------------------------------+
 |                                                                           |
 +---------------------------------------------------------------------------*/
void poly_atan(FPU_REG *st0_ptr, u_char st0_tag,
	       FPU_REG *st1_ptr, u_char st1_tag)
{
	u_char transformed, inverted, sign1, sign2;
	int exponent;
	long int dummy_exp;
	Xsig accumulator, Numer, Denom, accumulatore, argSignif, argSq, argSqSq;
	u_char tag;

	sign1 = getsign(st0_ptr);
	sign2 = getsign(st1_ptr);
	if (st0_tag == TAG_Valid) {
		exponent = exponent(st0_ptr);
	} else {
		/* This gives non-compatible stack contents... */
		FPU_to_exp16(st0_ptr, st0_ptr);
		exponent = exponent16(st0_ptr);
	}
	if (st1_tag == TAG_Valid) {
		exponent -= exponent(st1_ptr);
	} else {
		/* This gives non-compatible stack contents... */
		FPU_to_exp16(st1_ptr, st1_ptr);
		exponent -= exponent16(st1_ptr);
	}

	if ((exponent < 0) || ((exponent == 0) &&
			       ((st0_ptr->sigh < st1_ptr->sigh) ||
				((st0_ptr->sigh == st1_ptr->sigh) &&
				 (st0_ptr->sigl < st1_ptr->sigl))))) {
		inverted = 1;
		Numer.lsw = Denom.lsw = 0;
		XSIG_LL(Numer) = significand(st0_ptr);
		XSIG_LL(Denom) = significand(st1_ptr);
	} else {
		inverted = 0;
		exponent = -exponent;
		Numer.lsw = Denom.lsw = 0;
		XSIG_LL(Numer) = significand(st1_ptr);
		XSIG_LL(Denom) = significand(st0_ptr);
	}
	div_Xsig(&Numer, &Denom, &argSignif);
	exponent += norm_Xsig(&argSignif);

	if ((exponent >= -1)
	    || ((exponent == -2) && (argSignif.msw > 0xd413ccd0))) {
		/* The argument is greater than sqrt(2)-1 (=0.414213562...) */
		/* Convert the argument by an identity for atan */
		transformed = 1;

		if (exponent >= 0) {
#ifdef PARANOID
			if (!((exponent == 0) &&
			      (argSignif.lsw == 0) && (argSignif.midw == 0) &&
			      (argSignif.msw == 0x80000000))) {
				EXCEPTION(EX_INTERNAL | 0x104);	/* There must be a logic error */
				return;
			}
#endif /* PARANOID */
			argSignif.msw = 0;	/* Make the transformed arg -> 0.0 */
		} else {
			Numer.lsw = Denom.lsw = argSignif.lsw;
			XSIG_LL(Numer) = XSIG_LL(Denom) = XSIG_LL(argSignif);

			if (exponent < -1)
				shr_Xsig(&Numer, -1 - exponent);
			negate_Xsig(&Numer);

			shr_Xsig(&Denom, -exponent);
			Denom.msw |= 0x80000000;

			div_Xsig(&Numer, &Denom, &argSignif);

			exponent = -1 + norm_Xsig(&argSignif);
		}
	} else {
		transformed = 0;
	}

	argSq.lsw = argSignif.lsw;
	argSq.midw = argSignif.midw;
	argSq.msw = argSignif.msw;
	mul_Xsig_Xsig(&argSq, &argSq);

	argSqSq.lsw = argSq.lsw;
	argSqSq.midw = argSq.midw;
	argSqSq.msw = argSq.msw;
	mul_Xsig_Xsig(&argSqSq, &argSqSq);

	accumulatore.lsw = argSq.lsw;
	XSIG_LL(accumulatore) = XSIG_LL(argSq);

	shr_Xsig(&argSq, 2 * (-1 - exponent - 1));
	shr_Xsig(&argSqSq, 4 * (-1 - exponent - 1));

	/* Now have argSq etc with binary point at the left
	   .1xxxxxxxx */

	/* Do the basic fixed point polynomial evaluation */
	accumulator.msw = accumulator.midw = accumulator.lsw = 0;
	polynomial_Xsig(&accumulator, &XSIG_LL(argSqSq),
			oddplterms, HIPOWERop - 1);
	mul64_Xsig(&accumulator, &XSIG_LL(argSq));
	negate_Xsig(&accumulator);
	polynomial_Xsig(&accumulator, &XSIG_LL(argSqSq), oddnegterms,
			HIPOWERon - 1);
	negate_Xsig(&accumulator);
	add_two_Xsig(&accumulator, &fixedpterm, &dummy_exp);

	mul64_Xsig(&accumulatore, &denomterm);
	shr_Xsig(&accumulatore, 1 + 2 * (-1 - exponent));
	accumulatore.msw |= 0x80000000;

	div_Xsig(&accumulator, &accumulatore, &accumulator);

	mul_Xsig_Xsig(&accumulator, &argSignif);
	mul_Xsig_Xsig(&accumulator, &argSq);

	shr_Xsig(&accumulator, 3);
	negate_Xsig(&accumulator);
	add_Xsig_Xsig(&accumulator, &argSignif);

	if (transformed) {
		/* compute pi/4 - accumulator */
		shr_Xsig(&accumulator, -1 - exponent);
		negate_Xsig(&accumulator);
		add_Xsig_Xsig(&accumulator, &pi_signif);
		exponent = -1;
	}

	if (inverted) {
		/* compute pi/2 - accumulator */
		shr_Xsig(&accumulator, -exponent);
		negate_Xsig(&accumulator);
		add_Xsig_Xsig(&accumulator, &pi_signif);
		exponent = 0;
	}

	if (sign1) {
		/* compute pi - accumulator */
		shr_Xsig(&accumulator, 1 - exponent);
		negate_Xsig(&accumulator);
		add_Xsig_Xsig(&accumulator, &pi_signif);
		exponent = 1;
	}

	exponent += round_Xsig(&accumulator);

	significand(st1_ptr) = XSIG_LL(accumulator);
	setexponent16(st1_ptr, exponent);

	tag = FPU_round(st1_ptr, 1, 0, FULL_PRECISION, sign2);
	FPU_settagi(1, tag);

	set_precision_flag_up();	/* We do not really know if up or down,
					   use this as the default. */

}
