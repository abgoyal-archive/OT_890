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
 *  linux/fs/hfsplus/options.c
 *
 * Copyright (C) 2001
 * Brad Boyer (flar@allandria.com)
 * (C) 2003 Ardis Technologies <roman@ardistech.com>
 *
 * Option parsing
 */

#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/parser.h>
#include <linux/nls.h>
#include <linux/mount.h>
#include <linux/seq_file.h>
#include "hfsplus_fs.h"

enum {
	opt_creator, opt_type,
	opt_umask, opt_uid, opt_gid,
	opt_part, opt_session, opt_nls,
	opt_nodecompose, opt_decompose,
	opt_force, opt_err
};

static const match_table_t tokens = {
	{ opt_creator, "creator=%s" },
	{ opt_type, "type=%s" },
	{ opt_umask, "umask=%o" },
	{ opt_uid, "uid=%u" },
	{ opt_gid, "gid=%u" },
	{ opt_part, "part=%u" },
	{ opt_session, "session=%u" },
	{ opt_nls, "nls=%s" },
	{ opt_decompose, "decompose" },
	{ opt_nodecompose, "nodecompose" },
	{ opt_force, "force" },
	{ opt_err, NULL }
};

/* Initialize an options object to reasonable defaults */
void hfsplus_fill_defaults(struct hfsplus_sb_info *opts)
{
	if (!opts)
		return;

	opts->creator = HFSPLUS_DEF_CR_TYPE;
	opts->type = HFSPLUS_DEF_CR_TYPE;
	opts->umask = current->fs->umask;
	opts->uid = current_uid();
	opts->gid = current_gid();
	opts->part = -1;
	opts->session = -1;
}

/* convert a "four byte character" to a 32 bit int with error checks */
static inline int match_fourchar(substring_t *arg, u32 *result)
{
	if (arg->to - arg->from != 4)
		return -EINVAL;
	memcpy(result, arg->from, 4);
	return 0;
}

/* Parse options from mount. Returns 0 on failure */
/* input is the options passed to mount() as a string */
int hfsplus_parse_options(char *input, struct hfsplus_sb_info *sbi)
{
	char *p;
	substring_t args[MAX_OPT_ARGS];
	int tmp, token;

	if (!input)
		goto done;

	while ((p = strsep(&input, ",")) != NULL) {
		if (!*p)
			continue;

		token = match_token(p, tokens, args);
		switch (token) {
		case opt_creator:
			if (match_fourchar(&args[0], &sbi->creator)) {
				printk(KERN_ERR "hfs: creator requires a 4 character value\n");
				return 0;
			}
			break;
		case opt_type:
			if (match_fourchar(&args[0], &sbi->type)) {
				printk(KERN_ERR "hfs: type requires a 4 character value\n");
				return 0;
			}
			break;
		case opt_umask:
			if (match_octal(&args[0], &tmp)) {
				printk(KERN_ERR "hfs: umask requires a value\n");
				return 0;
			}
			sbi->umask = (umode_t)tmp;
			break;
		case opt_uid:
			if (match_int(&args[0], &tmp)) {
				printk(KERN_ERR "hfs: uid requires an argument\n");
				return 0;
			}
			sbi->uid = (uid_t)tmp;
			break;
		case opt_gid:
			if (match_int(&args[0], &tmp)) {
				printk(KERN_ERR "hfs: gid requires an argument\n");
				return 0;
			}
			sbi->gid = (gid_t)tmp;
			break;
		case opt_part:
			if (match_int(&args[0], &sbi->part)) {
				printk(KERN_ERR "hfs: part requires an argument\n");
				return 0;
			}
			break;
		case opt_session:
			if (match_int(&args[0], &sbi->session)) {
				printk(KERN_ERR "hfs: session requires an argument\n");
				return 0;
			}
			break;
		case opt_nls:
			if (sbi->nls) {
				printk(KERN_ERR "hfs: unable to change nls mapping\n");
				return 0;
			}
			p = match_strdup(&args[0]);
			if (p)
				sbi->nls = load_nls(p);
			if (!sbi->nls) {
				printk(KERN_ERR "hfs: unable to load nls mapping \"%s\"\n", p);
				kfree(p);
				return 0;
			}
			kfree(p);
			break;
		case opt_decompose:
			sbi->flags &= ~HFSPLUS_SB_NODECOMPOSE;
			break;
		case opt_nodecompose:
			sbi->flags |= HFSPLUS_SB_NODECOMPOSE;
			break;
		case opt_force:
			sbi->flags |= HFSPLUS_SB_FORCE;
			break;
		default:
			return 0;
		}
	}

done:
	if (!sbi->nls) {
		/* try utf8 first, as this is the old default behaviour */
		sbi->nls = load_nls("utf8");
		if (!sbi->nls)
			sbi->nls = load_nls_default();
		if (!sbi->nls)
			return 0;
	}

	return 1;
}

int hfsplus_show_options(struct seq_file *seq, struct vfsmount *mnt)
{
	struct hfsplus_sb_info *sbi = &HFSPLUS_SB(mnt->mnt_sb);

	if (sbi->creator != HFSPLUS_DEF_CR_TYPE)
		seq_printf(seq, ",creator=%.4s", (char *)&sbi->creator);
	if (sbi->type != HFSPLUS_DEF_CR_TYPE)
		seq_printf(seq, ",type=%.4s", (char *)&sbi->type);
	seq_printf(seq, ",umask=%o,uid=%u,gid=%u", sbi->umask, sbi->uid, sbi->gid);
	if (sbi->part >= 0)
		seq_printf(seq, ",part=%u", sbi->part);
	if (sbi->session >= 0)
		seq_printf(seq, ",session=%u", sbi->session);
	if (sbi->nls)
		seq_printf(seq, ",nls=%s", sbi->nls->charset);
	if (sbi->flags & HFSPLUS_SB_NODECOMPOSE)
		seq_printf(seq, ",nodecompose");
	return 0;
}
