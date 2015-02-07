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
 * Copyright (C) 2008 Red Hat, Inc., Jason Baron <jbaron@redhat.com>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DYNAMIC_DEBUG_HASH_BITS 6

static const char *program;

static void usage(void)
{
	printf("Usage: %s <djb2|r5> <modname>\n", program);
	exit(1);
}

/* djb2 hashing algorithm by Dan Bernstein. From:
 * http://www.cse.yorku.ca/~oz/hash.html
 */

unsigned int djb2_hash(char *str)
{
	unsigned long hash = 5381;
	int c;

	c = *str;
	while (c) {
		hash = ((hash << 5) + hash) + c;
		c = *++str;
	}
	return (unsigned int)(hash & ((1 << DYNAMIC_DEBUG_HASH_BITS) - 1));
}

unsigned int r5_hash(char *str)
{
	unsigned long hash = 0;
	int c;

	c = *str;
	while (c) {
		hash = (hash + (c << 4) + (c >> 4)) * 11;
		c = *++str;
	}
	return (unsigned int)(hash & ((1 << DYNAMIC_DEBUG_HASH_BITS) - 1));
}

int main(int argc, char *argv[])
{
	program = argv[0];

	if (argc != 3)
		usage();
	if (!strcmp(argv[1], "djb2"))
		printf("%d\n", djb2_hash(argv[2]));
	else if (!strcmp(argv[1], "r5"))
		printf("%d\n", r5_hash(argv[2]));
	else
		usage();
	exit(0);
}

