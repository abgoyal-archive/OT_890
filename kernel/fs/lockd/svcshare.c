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
 * linux/fs/lockd/svcshare.c
 *
 * Management of DOS shares.
 *
 * Copyright (C) 1996 Olaf Kirch <okir@monad.swb.de>
 */

#include <linux/time.h>
#include <linux/unistd.h>
#include <linux/string.h>
#include <linux/slab.h>

#include <linux/sunrpc/clnt.h>
#include <linux/sunrpc/svc.h>
#include <linux/lockd/lockd.h>
#include <linux/lockd/share.h>

static inline int
nlm_cmp_owner(struct nlm_share *share, struct xdr_netobj *oh)
{
	return share->s_owner.len == oh->len
	    && !memcmp(share->s_owner.data, oh->data, oh->len);
}

__be32
nlmsvc_share_file(struct nlm_host *host, struct nlm_file *file,
			struct nlm_args *argp)
{
	struct nlm_share	*share;
	struct xdr_netobj	*oh = &argp->lock.oh;
	u8			*ohdata;

	for (share = file->f_shares; share; share = share->s_next) {
		if (share->s_host == host && nlm_cmp_owner(share, oh))
			goto update;
		if ((argp->fsm_access & share->s_mode)
		 || (argp->fsm_mode   & share->s_access ))
			return nlm_lck_denied;
	}

	share = kmalloc(sizeof(*share) + oh->len,
						GFP_KERNEL);
	if (share == NULL)
		return nlm_lck_denied_nolocks;

	/* Copy owner handle */
	ohdata = (u8 *) (share + 1);
	memcpy(ohdata, oh->data, oh->len);

	share->s_file	    = file;
	share->s_host       = host;
	share->s_owner.data = ohdata;
	share->s_owner.len  = oh->len;
	share->s_next       = file->f_shares;
	file->f_shares      = share;

update:
	share->s_access = argp->fsm_access;
	share->s_mode   = argp->fsm_mode;
	return nlm_granted;
}

/*
 * Delete a share.
 */
__be32
nlmsvc_unshare_file(struct nlm_host *host, struct nlm_file *file,
			struct nlm_args *argp)
{
	struct nlm_share	*share, **shpp;
	struct xdr_netobj	*oh = &argp->lock.oh;

	for (shpp = &file->f_shares; (share = *shpp) != NULL;
					shpp = &share->s_next) {
		if (share->s_host == host && nlm_cmp_owner(share, oh)) {
			*shpp = share->s_next;
			kfree(share);
			return nlm_granted;
		}
	}

	/* X/Open spec says return success even if there was no
	 * corresponding share. */
	return nlm_granted;
}

/*
 * Traverse all shares for a given file, and delete
 * those owned by the given (type of) host
 */
void nlmsvc_traverse_shares(struct nlm_host *host, struct nlm_file *file,
		nlm_host_match_fn_t match)
{
	struct nlm_share	*share, **shpp;

	shpp = &file->f_shares;
	while ((share = *shpp) !=  NULL) {
		if (match(share->s_host, host)) {
			*shpp = share->s_next;
			kfree(share);
			continue;
		}
		shpp = &share->s_next;
	}
}
