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

#include <stdio.h>
#include "crc32defs.h"
#include <inttypes.h>

#define ENTRIES_PER_LINE 4

#define LE_TABLE_SIZE (1 << CRC_LE_BITS)
#define BE_TABLE_SIZE (1 << CRC_BE_BITS)

static uint32_t crc32table_le[LE_TABLE_SIZE];
static uint32_t crc32table_be[BE_TABLE_SIZE];

/**
 * crc32init_le() - allocate and initialize LE table data
 *
 * crc is the crc of the byte i; other entries are filled in based on the
 * fact that crctable[i^j] = crctable[i] ^ crctable[j].
 *
 */
static void crc32init_le(void)
{
	unsigned i, j;
	uint32_t crc = 1;

	crc32table_le[0] = 0;

	for (i = 1 << (CRC_LE_BITS - 1); i; i >>= 1) {
		crc = (crc >> 1) ^ ((crc & 1) ? CRCPOLY_LE : 0);
		for (j = 0; j < LE_TABLE_SIZE; j += 2 * i)
			crc32table_le[i + j] = crc ^ crc32table_le[j];
	}
}

/**
 * crc32init_be() - allocate and initialize BE table data
 */
static void crc32init_be(void)
{
	unsigned i, j;
	uint32_t crc = 0x80000000;

	crc32table_be[0] = 0;

	for (i = 1; i < BE_TABLE_SIZE; i <<= 1) {
		crc = (crc << 1) ^ ((crc & 0x80000000) ? CRCPOLY_BE : 0);
		for (j = 0; j < i; j++)
			crc32table_be[i + j] = crc ^ crc32table_be[j];
	}
}

static void output_table(uint32_t table[], int len, char *trans)
{
	int i;

	for (i = 0; i < len - 1; i++) {
		if (i % ENTRIES_PER_LINE == 0)
			printf("\n");
		printf("%s(0x%8.8xL), ", trans, table[i]);
	}
	printf("%s(0x%8.8xL)\n", trans, table[len - 1]);
}

int main(int argc, char** argv)
{
	printf("/* this file is generated - do not edit */\n\n");

	if (CRC_LE_BITS > 1) {
		crc32init_le();
		printf("static const u32 crc32table_le[] = {");
		output_table(crc32table_le, LE_TABLE_SIZE, "tole");
		printf("};\n");
	}

	if (CRC_BE_BITS > 1) {
		crc32init_be();
		printf("static const u32 crc32table_be[] = {");
		output_table(crc32table_be, BE_TABLE_SIZE, "tobe");
		printf("};\n");
	}

	return 0;
}
