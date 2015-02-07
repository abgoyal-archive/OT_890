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
 *  smb_fs_sb.h
 *
 *  Copyright (C) 1995 by Paal-Kr. Engstad and Volker Lendecke
 *  Copyright (C) 1997 by Volker Lendecke
 *
 */

#ifndef _SMB_FS_SB
#define _SMB_FS_SB

#include <linux/types.h>
#include <linux/smb.h>

/*
 * Upper limit on the total number of active smb_request structs.
 */
#define MAX_REQUEST_HARD       256

enum smb_receive_state {
	SMB_RECV_START,		/* No data read, looking for length + sig */
	SMB_RECV_HEADER,	/* Reading the header data */
	SMB_RECV_HCOMPLETE,	/* Done with the header */
	SMB_RECV_PARAM,		/* Reading parameter words */
	SMB_RECV_DATA,		/* Reading data bytes */
	SMB_RECV_END,		/* End of request */
	SMB_RECV_DROP,		/* Dropping this SMB */
	SMB_RECV_REQUEST,	/* Received a request and not a reply */
};

/* structure access macros */
#define server_from_inode(inode) SMB_SB((inode)->i_sb)
#define server_from_dentry(dentry) SMB_SB((dentry)->d_sb)
#define SB_of(server) ((server)->super_block)

struct smb_sb_info {
	/* List of all smbfs superblocks */
	struct list_head entry;

        enum smb_conn_state state;
	struct file * sock_file;
	int conn_error;
	enum smb_receive_state rstate;

	atomic_t nr_requests;
	struct list_head xmitq;
	struct list_head recvq;
	u16 mid;

        struct smb_mount_data_kernel *mnt;

	/* Connections are counted. Each time a new socket arrives,
	 * generation is incremented.
	 */
	unsigned int generation;
	struct pid *conn_pid;
	struct smb_conn_opt opt;
	wait_queue_head_t conn_wq;
	int conn_complete;
	struct semaphore sem;

	unsigned char      header[SMB_HEADER_LEN + 20*2 + 2];
	u32                header_len;
	u32                smb_len;
	u32                smb_read;

        /* We use our own data_ready callback, but need the original one */
        void *data_ready;

	/* nls pointers for codepage conversions */
	struct nls_table *remote_nls;
	struct nls_table *local_nls;

	struct smb_ops *ops;

	struct super_block *super_block;
};

static inline int
smb_lock_server_interruptible(struct smb_sb_info *server)
{
	return down_interruptible(&(server->sem));
}

static inline void
smb_lock_server(struct smb_sb_info *server)
{
	down(&(server->sem));
}

static inline void
smb_unlock_server(struct smb_sb_info *server)
{
	up(&(server->sem));
}

#endif
