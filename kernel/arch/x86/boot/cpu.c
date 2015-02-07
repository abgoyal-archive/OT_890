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
 *   Copyright (C) 1991, 1992 Linus Torvalds
 *   Copyright 2007-2008 rPath, Inc. - All Rights Reserved
 *
 *   This file is part of the Linux kernel, and is made available under
 *   the terms of the GNU General Public License version 2.
 *
 * ----------------------------------------------------------------------- */

/*
 * arch/x86/boot/cpu.c
 *
 * Check for obligatory CPU features and abort if the features are not
 * present.
 */

#include "boot.h"
#include "cpustr.h"

static char *cpu_name(int level)
{
	static char buf[6];

	if (level == 64) {
		return "x86-64";
	} else {
		if (level == 15)
			level = 6;
		sprintf(buf, "i%d86", level);
		return buf;
	}
}

int validate_cpu(void)
{
	u32 *err_flags;
	int cpu_level, req_level;
	const unsigned char *msg_strs;

	check_cpu(&cpu_level, &req_level, &err_flags);

	if (cpu_level < req_level) {
		printf("This kernel requires an %s CPU, ",
		       cpu_name(req_level));
		printf("but only detected an %s CPU.\n",
		       cpu_name(cpu_level));
		return -1;
	}

	if (err_flags) {
		int i, j;
		puts("This kernel requires the following features "
		     "not present on the CPU:\n");

		msg_strs = (const unsigned char *)x86_cap_strs;

		for (i = 0; i < NCAPINTS; i++) {
			u32 e = err_flags[i];

			for (j = 0; j < 32; j++) {
				if (msg_strs[0] < i ||
				    (msg_strs[0] == i && msg_strs[1] < j)) {
					/* Skip to the next string */
					msg_strs += 2;
					while (*msg_strs++)
						;
				}
				if (e & 1) {
					if (msg_strs[0] == i &&
					    msg_strs[1] == j &&
					    msg_strs[2])
						printf("%s ", msg_strs+2);
					else
						printf("%d:%d ", i, j);
				}
				e >>= 1;
			}
		}
		putchar('\n');
		return -1;
	} else {
		return 0;
	}
}
