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
 * linux/fs/nfsd/auth.c
 *
 * Copyright (C) 1995, 1996 Olaf Kirch <okir@monad.swb.de>
 */

#include <linux/types.h>
#include <linux/sched.h>
#include <linux/sunrpc/svc.h>
#include <linux/sunrpc/svcauth.h>
#include <linux/nfsd/nfsd.h>
#include <linux/nfsd/export.h>
#include "auth.h"

int nfsexp_flags(struct svc_rqst *rqstp, struct svc_export *exp)
{
	struct exp_flavor_info *f;
	struct exp_flavor_info *end = exp->ex_flavors + exp->ex_nflavors;

	for (f = exp->ex_flavors; f < end; f++) {
		if (f->pseudoflavor == rqstp->rq_flavor)
			return f->flags;
	}
	return exp->ex_flags;

}

int nfsd_setuser(struct svc_rqst *rqstp, struct svc_export *exp)
{
	struct group_info *rqgi;
	struct group_info *gi;
	struct cred *new;
	int i;
	int flags = nfsexp_flags(rqstp, exp);
	int ret;

	/* discard any old override before preparing the new set */
	revert_creds(get_cred(current->real_cred));
	new = prepare_creds();
	if (!new)
		return -ENOMEM;

	new->fsuid = rqstp->rq_cred.cr_uid;
	new->fsgid = rqstp->rq_cred.cr_gid;

	rqgi = rqstp->rq_cred.cr_group_info;

	if (flags & NFSEXP_ALLSQUASH) {
		new->fsuid = exp->ex_anon_uid;
		new->fsgid = exp->ex_anon_gid;
		gi = groups_alloc(0);
		if (!gi)
			goto oom;
	} else if (flags & NFSEXP_ROOTSQUASH) {
		if (!new->fsuid)
			new->fsuid = exp->ex_anon_uid;
		if (!new->fsgid)
			new->fsgid = exp->ex_anon_gid;

		gi = groups_alloc(rqgi->ngroups);
		if (!gi)
			goto oom;

		for (i = 0; i < rqgi->ngroups; i++) {
			if (!GROUP_AT(rqgi, i))
				GROUP_AT(gi, i) = exp->ex_anon_gid;
			else
				GROUP_AT(gi, i) = GROUP_AT(rqgi, i);
		}
	} else {
		gi = get_group_info(rqgi);
	}

	if (new->fsuid == (uid_t) -1)
		new->fsuid = exp->ex_anon_uid;
	if (new->fsgid == (gid_t) -1)
		new->fsgid = exp->ex_anon_gid;

	ret = set_groups(new, gi);
	put_group_info(gi);
	if (ret < 0)
		goto error;

	if (new->fsuid)
		new->cap_effective = cap_drop_nfsd_set(new->cap_effective);
	else
		new->cap_effective = cap_raise_nfsd_set(new->cap_effective,
							new->cap_permitted);
	put_cred(override_creds(new));
	put_cred(new);
	return 0;

oom:
	ret = -ENOMEM;
error:
	abort_creds(new);
	return ret;
}

