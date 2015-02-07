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
 *  linux/fs/hpfs/dentry.c
 *
 *  Mikulas Patocka (mikulas@artax.karlin.mff.cuni.cz), 1998-1999
 *
 *  dcache operations
 */

#include "hpfs_fn.h"

/*
 * Note: the dentry argument is the parent dentry.
 */

static int hpfs_hash_dentry(struct dentry *dentry, struct qstr *qstr)
{
	unsigned long	 hash;
	int		 i;
	unsigned l = qstr->len;

	if (l == 1) if (qstr->name[0]=='.') goto x;
	if (l == 2) if (qstr->name[0]=='.' || qstr->name[1]=='.') goto x;
	hpfs_adjust_length((char *)qstr->name, &l);
	/*if (hpfs_chk_name((char *)qstr->name,&l))*/
		/*return -ENAMETOOLONG;*/
		/*return -ENOENT;*/
	x:

	hash = init_name_hash();
	for (i = 0; i < l; i++)
		hash = partial_name_hash(hpfs_upcase(hpfs_sb(dentry->d_sb)->sb_cp_table,qstr->name[i]), hash);
	qstr->hash = end_name_hash(hash);

	return 0;
}

static int hpfs_compare_dentry(struct dentry *dentry, struct qstr *a, struct qstr *b)
{
	unsigned al=a->len;
	unsigned bl=b->len;
	hpfs_adjust_length((char *)a->name, &al);
	/*hpfs_adjust_length((char *)b->name, &bl);*/
	/* 'a' is the qstr of an already existing dentry, so the name
	 * must be valid. 'b' must be validated first.
	 */

	if (hpfs_chk_name((char *)b->name, &bl)) return 1;
	if (hpfs_compare_names(dentry->d_sb, (char *)a->name, al, (char *)b->name, bl, 0)) return 1;
	return 0;
}

static struct dentry_operations hpfs_dentry_operations = {
	.d_hash		= hpfs_hash_dentry,
	.d_compare	= hpfs_compare_dentry,
};

void hpfs_set_dentry_operations(struct dentry *dentry)
{
	dentry->d_op = &hpfs_dentry_operations;
}
