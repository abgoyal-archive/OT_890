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
 * getopt.c
 */

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/net.h>

#include "getopt.h"

/**
 *	smb_getopt - option parser
 *	@caller: name of the caller, for error messages
 *	@options: the options string
 *	@opts: an array of &struct option entries controlling parser operations
 *	@optopt: output; will contain the current option
 *	@optarg: output; will contain the value (if one exists)
 *	@flag: output; may be NULL; should point to a long for or'ing flags
 *	@value: output; may be NULL; will be overwritten with the integer value
 *		of the current argument.
 *
 *	Helper to parse options on the format used by mount ("a=b,c=d,e,f").
 *	Returns opts->val if a matching entry in the 'opts' array is found,
 *	0 when no more tokens are found, -1 if an error is encountered.
 */
int smb_getopt(char *caller, char **options, struct option *opts,
	       char **optopt, char **optarg, unsigned long *flag,
	       unsigned long *value)
{
	char *token;
	char *val;
	int i;

	do {
		if ((token = strsep(options, ",")) == NULL)
			return 0;
	} while (*token == '\0');
	*optopt = token;

	*optarg = NULL;
	if ((val = strchr (token, '=')) != NULL) {
		*val++ = 0;
		if (value)
			*value = simple_strtoul(val, NULL, 0);
		*optarg = val;
	}

	for (i = 0; opts[i].name != NULL; i++) {
		if (!strcmp(opts[i].name, token)) {
			if (!opts[i].flag && (!val || !*val)) {
				printk("%s: the %s option requires an argument\n",
				       caller, token);
				return -1;
			}

			if (flag && opts[i].flag)
				*flag |= opts[i].flag;

			return opts[i].val;
		}
	}
	printk("%s: Unrecognized mount option %s\n", caller, token);
	return -1;
}
