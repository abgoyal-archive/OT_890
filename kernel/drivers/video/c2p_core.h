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
 *  Fast C2P (Chunky-to-Planar) Conversion
 *
 *  Copyright (C) 2003-2008 Geert Uytterhoeven
 *
 *  NOTES:
 *    - This code was inspired by Scout's C2P tutorial
 *    - It assumes to run on a big endian system
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License. See the file COPYING in the main directory of this archive
 *  for more details.
 */


    /*
     *  Basic transpose step
     */

static inline void _transp(u32 d[], unsigned int i1, unsigned int i2,
			   unsigned int shift, u32 mask)
{
	u32 t = (d[i1] ^ (d[i2] >> shift)) & mask;

	d[i1] ^= t;
	d[i2] ^= t << shift;
}


extern void c2p_unsupported(void);

static inline u32 get_mask(unsigned int n)
{
	switch (n) {
	case 1:
		return 0x55555555;

	case 2:
		return 0x33333333;

	case 4:
		return 0x0f0f0f0f;

	case 8:
		return 0x00ff00ff;

	case 16:
		return 0x0000ffff;
	}

	c2p_unsupported();
	return 0;
}


    /*
     *  Transpose operations on 8 32-bit words
     */

static inline void transp8(u32 d[], unsigned int n, unsigned int m)
{
	u32 mask = get_mask(n);

	switch (m) {
	case 1:
		/* First n x 1 block */
		_transp(d, 0, 1, n, mask);
		/* Second n x 1 block */
		_transp(d, 2, 3, n, mask);
		/* Third n x 1 block */
		_transp(d, 4, 5, n, mask);
		/* Fourth n x 1 block */
		_transp(d, 6, 7, n, mask);
		return;

	case 2:
		/* First n x 2 block */
		_transp(d, 0, 2, n, mask);
		_transp(d, 1, 3, n, mask);
		/* Second n x 2 block */
		_transp(d, 4, 6, n, mask);
		_transp(d, 5, 7, n, mask);
		return;

	case 4:
		/* Single n x 4 block */
		_transp(d, 0, 4, n, mask);
		_transp(d, 1, 5, n, mask);
		_transp(d, 2, 6, n, mask);
		_transp(d, 3, 7, n, mask);
		return;
	}

	c2p_unsupported();
}


    /*
     *  Transpose operations on 4 32-bit words
     */

static inline void transp4(u32 d[], unsigned int n, unsigned int m)
{
	u32 mask = get_mask(n);

	switch (m) {
	case 1:
		/* First n x 1 block */
		_transp(d, 0, 1, n, mask);
		/* Second n x 1 block */
		_transp(d, 2, 3, n, mask);
		return;

	case 2:
		/* Single n x 2 block */
		_transp(d, 0, 2, n, mask);
		_transp(d, 1, 3, n, mask);
		return;
	}

	c2p_unsupported();
}


    /*
     *  Transpose operations on 4 32-bit words (reverse order)
     */

static inline void transp4x(u32 d[], unsigned int n, unsigned int m)
{
	u32 mask = get_mask(n);

	switch (m) {
	case 2:
		/* Single n x 2 block */
		_transp(d, 2, 0, n, mask);
		_transp(d, 3, 1, n, mask);
		return;
	}

	c2p_unsupported();
}


    /*
     *  Compose two values, using a bitmask as decision value
     *  This is equivalent to (a & mask) | (b & ~mask)
     */

static inline u32 comp(u32 a, u32 b, u32 mask)
{
	return ((a ^ b) & mask) ^ b;
}
