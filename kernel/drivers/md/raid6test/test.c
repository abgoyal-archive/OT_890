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
 * raid6test.c
 *
 * Test RAID-6 recovery with various algorithms
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "raid6.h"

#define NDISKS		16	/* Including P and Q */

const char raid6_empty_zero_page[PAGE_SIZE] __attribute__((aligned(256)));
struct raid6_calls raid6_call;

char *dataptrs[NDISKS];
char data[NDISKS][PAGE_SIZE];
char recovi[PAGE_SIZE], recovj[PAGE_SIZE];

static void makedata(void)
{
	int i, j;

	for (i = 0; i < NDISKS; i++) {
		for (j = 0; j < PAGE_SIZE; j++)
			data[i][j] = rand();

		dataptrs[i] = data[i];
	}
}

static char disk_type(int d)
{
	switch (d) {
	case NDISKS-2:
		return 'P';
	case NDISKS-1:
		return 'Q';
	default:
		return 'D';
	}
}

static int test_disks(int i, int j)
{
	int erra, errb;

	memset(recovi, 0xf0, PAGE_SIZE);
	memset(recovj, 0xba, PAGE_SIZE);

	dataptrs[i] = recovi;
	dataptrs[j] = recovj;

	raid6_dual_recov(NDISKS, PAGE_SIZE, i, j, (void **)&dataptrs);

	erra = memcmp(data[i], recovi, PAGE_SIZE);
	errb = memcmp(data[j], recovj, PAGE_SIZE);

	if (i < NDISKS-2 && j == NDISKS-1) {
		/* We don't implement the DQ failure scenario, since it's
		   equivalent to a RAID-5 failure (XOR, then recompute Q) */
		erra = errb = 0;
	} else {
		printf("algo=%-8s  faila=%3d(%c)  failb=%3d(%c)  %s\n",
		       raid6_call.name,
		       i, disk_type(i),
		       j, disk_type(j),
		       (!erra && !errb) ? "OK" :
		       !erra ? "ERRB" :
		       !errb ? "ERRA" : "ERRAB");
	}

	dataptrs[i] = data[i];
	dataptrs[j] = data[j];

	return erra || errb;
}

int main(int argc, char *argv[])
{
	const struct raid6_calls *const *algo;
	int i, j;
	int err = 0;

	makedata();

	for (algo = raid6_algos; *algo; algo++) {
		if (!(*algo)->valid || (*algo)->valid()) {
			raid6_call = **algo;

			/* Nuke syndromes */
			memset(data[NDISKS-2], 0xee, 2*PAGE_SIZE);

			/* Generate assumed good syndrome */
			raid6_call.gen_syndrome(NDISKS, PAGE_SIZE,
						(void **)&dataptrs);

			for (i = 0; i < NDISKS-1; i++)
				for (j = i+1; j < NDISKS; j++)
					err += test_disks(i, j);
		}
		printf("\n");
	}

	printf("\n");
	/* Pick the best algorithm test */
	raid6_select_algo();

	if (err)
		printf("\n*** ERRORS FOUND ***\n");

	return err;
}
