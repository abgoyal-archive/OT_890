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
 * Helpers for formatting and printing strings
 *
 * Copyright 31 August 2008 James Bottomley
 */
#include <linux/kernel.h>
#include <linux/math64.h>
#include <linux/module.h>
#include <linux/string_helpers.h>

/**
 * string_get_size - get the size in the specified units
 * @size:	The size to be converted
 * @units:	units to use (powers of 1000 or 1024)
 * @buf:	buffer to format to
 * @len:	length of buffer
 *
 * This function returns a string formatted to 3 significant figures
 * giving the size in the required units.  Returns 0 on success or
 * error on failure.  @buf is always zero terminated.
 *
 */
int string_get_size(u64 size, const enum string_size_units units,
		    char *buf, int len)
{
	const char *units_10[] = { "B", "kB", "MB", "GB", "TB", "PB",
				   "EB", "ZB", "YB", NULL};
	const char *units_2[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB",
				 "EiB", "ZiB", "YiB", NULL };
	const char **units_str[] = {
		[STRING_UNITS_10] =  units_10,
		[STRING_UNITS_2] = units_2,
	};
	const unsigned int divisor[] = {
		[STRING_UNITS_10] = 1000,
		[STRING_UNITS_2] = 1024,
	};
	int i, j;
	u64 remainder = 0, sf_cap;
	char tmp[8];

	tmp[0] = '\0';
	i = 0;
	if (size >= divisor[units]) {
		while (size >= divisor[units] && units_str[units][i]) {
			remainder = do_div(size, divisor[units]);
			i++;
		}

		sf_cap = size;
		for (j = 0; sf_cap*10 < 1000; j++)
			sf_cap *= 10;

		if (j) {
			remainder *= 1000;
			do_div(remainder, divisor[units]);
			snprintf(tmp, sizeof(tmp), ".%03lld",
				 (unsigned long long)remainder);
			tmp[j+1] = '\0';
		}
	}

	snprintf(buf, len, "%lld%s %s", (unsigned long long)size,
		 tmp, units_str[units][i]);

	return 0;
}
EXPORT_SYMBOL(string_get_size);
