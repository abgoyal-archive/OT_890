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
 * linux/fs/nfs/callback_proc.c
 *
 * Copyright (C) 2004 Trond Myklebust
 *
 * NFSv4 callback procedures
 */
#include <linux/nfs4.h>
#include <linux/nfs_fs.h>
#include "nfs4_fs.h"
#include "callback.h"
#include "delegation.h"
#include "internal.h"

#ifdef NFS_DEBUG
#define NFSDBG_FACILITY NFSDBG_CALLBACK
#endif
 
__be32 nfs4_callback_getattr(struct cb_getattrargs *args, struct cb_getattrres *res)
{
	struct nfs_client *clp;
	struct nfs_delegation *delegation;
	struct nfs_inode *nfsi;
	struct inode *inode;

	res->bitmap[0] = res->bitmap[1] = 0;
	res->status = htonl(NFS4ERR_BADHANDLE);
	clp = nfs_find_client(args->addr, 4);
	if (clp == NULL)
		goto out;

	dprintk("NFS: GETATTR callback request from %s\n",
		rpc_peeraddr2str(clp->cl_rpcclient, RPC_DISPLAY_ADDR));

	inode = nfs_delegation_find_inode(clp, &args->fh);
	if (inode == NULL)
		goto out_putclient;
	nfsi = NFS_I(inode);
	down_read(&nfsi->rwsem);
	delegation = nfsi->delegation;
	if (delegation == NULL || (delegation->type & FMODE_WRITE) == 0)
		goto out_iput;
	res->size = i_size_read(inode);
	res->change_attr = delegation->change_attr;
	if (nfsi->npages != 0)
		res->change_attr++;
	res->ctime = inode->i_ctime;
	res->mtime = inode->i_mtime;
	res->bitmap[0] = (FATTR4_WORD0_CHANGE|FATTR4_WORD0_SIZE) &
		args->bitmap[0];
	res->bitmap[1] = (FATTR4_WORD1_TIME_METADATA|FATTR4_WORD1_TIME_MODIFY) &
		args->bitmap[1];
	res->status = 0;
out_iput:
	up_read(&nfsi->rwsem);
	iput(inode);
out_putclient:
	nfs_put_client(clp);
out:
	dprintk("%s: exit with status = %d\n", __func__, ntohl(res->status));
	return res->status;
}

__be32 nfs4_callback_recall(struct cb_recallargs *args, void *dummy)
{
	struct nfs_client *clp;
	struct inode *inode;
	__be32 res;
	
	res = htonl(NFS4ERR_BADHANDLE);
	clp = nfs_find_client(args->addr, 4);
	if (clp == NULL)
		goto out;

	dprintk("NFS: RECALL callback request from %s\n",
		rpc_peeraddr2str(clp->cl_rpcclient, RPC_DISPLAY_ADDR));

	do {
		struct nfs_client *prev = clp;

		inode = nfs_delegation_find_inode(clp, &args->fh);
		if (inode != NULL) {
			/* Set up a helper thread to actually return the delegation */
			switch(nfs_async_inode_return_delegation(inode, &args->stateid)) {
				case 0:
					res = 0;
					break;
				case -ENOENT:
					if (res != 0)
						res = htonl(NFS4ERR_BAD_STATEID);
					break;
				default:
					res = htonl(NFS4ERR_RESOURCE);
			}
			iput(inode);
		}
		clp = nfs_find_client_next(prev);
		nfs_put_client(prev);
	} while (clp != NULL);
out:
	dprintk("%s: exit with status = %d\n", __func__, ntohl(res));
	return res;
}
