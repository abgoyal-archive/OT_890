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

#ifndef __CODA_PSDEV_H
#define __CODA_PSDEV_H

#include <linux/magic.h>

#define CODA_PSDEV_MAJOR 67
#define MAX_CODADEVS  5	   /* how many do we allow */

struct kstatfs;

/* communication pending/processing queues */
struct venus_comm {
	u_long		    vc_seq;
	wait_queue_head_t   vc_waitq; /* Venus wait queue */
	struct list_head    vc_pending;
	struct list_head    vc_processing;
	int                 vc_inuse;
	struct super_block *vc_sb;
};


static inline struct venus_comm *coda_vcp(struct super_block *sb)
{
	return (struct venus_comm *)((sb)->s_fs_info);
}


/* upcalls */
int venus_rootfid(struct super_block *sb, struct CodaFid *fidp);
int venus_getattr(struct super_block *sb, struct CodaFid *fid,
		  struct coda_vattr *attr);
int venus_setattr(struct super_block *, struct CodaFid *, struct coda_vattr *);
int venus_lookup(struct super_block *sb, struct CodaFid *fid, 
		 const char *name, int length, int *type, 
		 struct CodaFid *resfid);
int venus_close(struct super_block *sb, struct CodaFid *fid, int flags,
		vuid_t uid);
int venus_open(struct super_block *sb, struct CodaFid *fid, int flags,
	       struct file **f);
int venus_mkdir(struct super_block *sb, struct CodaFid *dirfid, 
		const char *name, int length, 
		struct CodaFid *newfid, struct coda_vattr *attrs);
int venus_create(struct super_block *sb, struct CodaFid *dirfid, 
		 const char *name, int length, int excl, int mode,
		 struct CodaFid *newfid, struct coda_vattr *attrs) ;
int venus_rmdir(struct super_block *sb, struct CodaFid *dirfid, 
		const char *name, int length);
int venus_remove(struct super_block *sb, struct CodaFid *dirfid, 
		 const char *name, int length);
int venus_readlink(struct super_block *sb, struct CodaFid *fid, 
		   char *buffer, int *length);
int venus_rename(struct super_block *, struct CodaFid *new_fid, 
		 struct CodaFid *old_fid, size_t old_length, 
		 size_t new_length, const char *old_name, 
		 const char *new_name);
int venus_link(struct super_block *sb, struct CodaFid *fid, 
		  struct CodaFid *dirfid, const char *name, int len );
int venus_symlink(struct super_block *sb, struct CodaFid *fid,
		  const char *name, int len, const char *symname, int symlen);
int venus_access(struct super_block *sb, struct CodaFid *fid, int mask);
int venus_pioctl(struct super_block *sb, struct CodaFid *fid,
		 unsigned int cmd, struct PioctlData *data);
int coda_downcall(int opcode, union outputArgs *out, struct super_block *sb);
int venus_fsync(struct super_block *sb, struct CodaFid *fid);
int venus_statfs(struct dentry *dentry, struct kstatfs *sfs);


/* messages between coda filesystem in kernel and Venus */
struct upc_req {
	struct list_head    uc_chain;
	caddr_t	            uc_data;
	u_short	            uc_flags;
	u_short             uc_inSize;  /* Size is at most 5000 bytes */
	u_short	            uc_outSize;
	u_short	            uc_opcode;  /* copied from data to save lookup */
	int		    uc_unique;
	wait_queue_head_t   uc_sleep;   /* process' wait queue */
};

#define REQ_ASYNC  0x1
#define REQ_READ   0x2
#define REQ_WRITE  0x4
#define REQ_ABORT  0x8


/*
 * Statistics
 */

extern struct venus_comm coda_comms[];

#endif
