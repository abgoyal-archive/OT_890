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
 * lib/reed_solomon/encode_rs.c
 *
 * Overview:
 *   Generic Reed Solomon encoder / decoder library
 *
 * Copyright 2002, Phil Karn, KA9Q
 * May be used under the terms of the GNU General Public License (GPL)
 *
 * Adaption to the kernel by Thomas Gleixner (tglx@linutronix.de)
 *
 * $Id$
 *
 */

/* Generic data width independent code which is included by the
 * wrappers.
 * int encode_rsX (struct rs_control *rs, uintX_t *data, int len, uintY_t *par)
 */
{
	int i, j, pad;
	int nn = rs->nn;
	int nroots = rs->nroots;
	uint16_t *alpha_to = rs->alpha_to;
	uint16_t *index_of = rs->index_of;
	uint16_t *genpoly = rs->genpoly;
	uint16_t fb;
	uint16_t msk = (uint16_t) rs->nn;

	/* Check length parameter for validity */
	pad = nn - nroots - len;
	if (pad < 0 || pad >= nn)
		return -ERANGE;

	for (i = 0; i < len; i++) {
		fb = index_of[((((uint16_t) data[i])^invmsk) & msk) ^ par[0]];
		/* feedback term is non-zero */
		if (fb != nn) {
			for (j = 1; j < nroots; j++) {
				par[j] ^= alpha_to[rs_modnn(rs, fb +
							 genpoly[nroots - j])];
			}
		}
		/* Shift */
		memmove(&par[0], &par[1], sizeof(uint16_t) * (nroots - 1));
		if (fb != nn) {
			par[nroots - 1] = alpha_to[rs_modnn(rs,
							    fb + genpoly[0])];
		} else {
			par[nroots - 1] = 0;
		}
	}
	return 0;
}
