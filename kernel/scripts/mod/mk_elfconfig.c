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
#include <stdlib.h>
#include <string.h>
#include <elf.h>

int
main(int argc, char **argv)
{
	unsigned char ei[EI_NIDENT];
	union { short s; char c[2]; } endian_test;

	if (argc != 2) {
		fprintf(stderr, "Error: no arch\n");
	}
	if (fread(ei, 1, EI_NIDENT, stdin) != EI_NIDENT) {
		fprintf(stderr, "Error: input truncated\n");
		return 1;
	}
	if (memcmp(ei, ELFMAG, SELFMAG) != 0) {
		fprintf(stderr, "Error: not ELF\n");
		return 1;
	}
	switch (ei[EI_CLASS]) {
	case ELFCLASS32:
		printf("#define KERNEL_ELFCLASS ELFCLASS32\n");
		break;
	case ELFCLASS64:
		printf("#define KERNEL_ELFCLASS ELFCLASS64\n");
		break;
	default:
		exit(1);
	}
	switch (ei[EI_DATA]) {
	case ELFDATA2LSB:
		printf("#define KERNEL_ELFDATA ELFDATA2LSB\n");
		break;
	case ELFDATA2MSB:
		printf("#define KERNEL_ELFDATA ELFDATA2MSB\n");
		break;
	default:
		exit(1);
	}

	if (sizeof(unsigned long) == 4) {
		printf("#define HOST_ELFCLASS ELFCLASS32\n");
	} else if (sizeof(unsigned long) == 8) {
		printf("#define HOST_ELFCLASS ELFCLASS64\n");
	}

	endian_test.s = 0x0102;
	if (memcmp(endian_test.c, "\x01\x02", 2) == 0)
		printf("#define HOST_ELFDATA ELFDATA2MSB\n");
	else if (memcmp(endian_test.c, "\x02\x01", 2) == 0)
		printf("#define HOST_ELFDATA ELFDATA2LSB\n");
	else
		exit(1);

	if ((strcmp(argv[1], "h8300") == 0)
	    || (strcmp(argv[1], "blackfin") == 0))
		printf("#define MODULE_SYMBOL_PREFIX \"_\"\n");
	else
		printf("#define MODULE_SYMBOL_PREFIX \"\"\n");

	return 0;
}

