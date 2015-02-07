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

/* -*- linux-c -*- ------------------------------------------------------- *
 *
 *   Copyright 2002-2007 H. Peter Anvin - All Rights Reserved
 *
 *   This file is part of the Linux kernel, and is made available under
 *   the terms of the GNU General Public License version 2 or (at your
 *   option) any later version; incorporated herein by reference.
 *
 * ----------------------------------------------------------------------- */

/*
 * mktables.c
 *
 * Make RAID-6 tables.  This is a host user space program to be run at
 * compile time.
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>

static uint8_t gfmul(uint8_t a, uint8_t b)
{
	uint8_t v = 0;

	while (b) {
		if (b & 1)
			v ^= a;
		a = (a << 1) ^ (a & 0x80 ? 0x1d : 0);
		b >>= 1;
	}

	return v;
}

static uint8_t gfpow(uint8_t a, int b)
{
	uint8_t v = 1;

	b %= 255;
	if (b < 0)
		b += 255;

	while (b) {
		if (b & 1)
			v = gfmul(v, a);
		a = gfmul(a, a);
		b >>= 1;
	}

	return v;
}

int main(int argc, char *argv[])
{
	int i, j, k;
	uint8_t v;
	uint8_t exptbl[256], invtbl[256];

	printf("#include \"raid6.h\"\n");

	/* Compute multiplication table */
	printf("\nconst u8  __attribute__((aligned(256)))\n"
		"raid6_gfmul[256][256] =\n"
		"{\n");
	for (i = 0; i < 256; i++) {
		printf("\t{\n");
		for (j = 0; j < 256; j += 8) {
			printf("\t\t");
			for (k = 0; k < 8; k++)
				printf("0x%02x,%c", gfmul(i, j + k),
				       (k == 7) ? '\n' : ' ');
		}
		printf("\t},\n");
	}
	printf("};\n");

	/* Compute power-of-2 table (exponent) */
	v = 1;
	printf("\nconst u8 __attribute__((aligned(256)))\n"
	       "raid6_gfexp[256] =\n" "{\n");
	for (i = 0; i < 256; i += 8) {
		printf("\t");
		for (j = 0; j < 8; j++) {
			exptbl[i + j] = v;
			printf("0x%02x,%c", v, (j == 7) ? '\n' : ' ');
			v = gfmul(v, 2);
			if (v == 1)
				v = 0;	/* For entry 255, not a real entry */
		}
	}
	printf("};\n");

	/* Compute inverse table x^-1 == x^254 */
	printf("\nconst u8 __attribute__((aligned(256)))\n"
	       "raid6_gfinv[256] =\n" "{\n");
	for (i = 0; i < 256; i += 8) {
		printf("\t");
		for (j = 0; j < 8; j++) {
			invtbl[i + j] = v = gfpow(i + j, 254);
			printf("0x%02x,%c", v, (j == 7) ? '\n' : ' ');
		}
	}
	printf("};\n");

	/* Compute inv(2^x + 1) (exponent-xor-inverse) table */
	printf("\nconst u8 __attribute__((aligned(256)))\n"
	       "raid6_gfexi[256] =\n" "{\n");
	for (i = 0; i < 256; i += 8) {
		printf("\t");
		for (j = 0; j < 8; j++)
			printf("0x%02x,%c", invtbl[exptbl[i + j] ^ 1],
			       (j == 7) ? '\n' : ' ');
	}
	printf("};\n");

	return 0;
}
