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
 * linux/include/linux/nfsd/xdr.h
 *
 * XDR types for nfsd. This is mainly a typing exercise.
 */

#ifndef LINUX_NFSD_H
#define LINUX_NFSD_H

#include <linux/fs.h>
#include <linux/vfs.h>
#include <linux/nfs.h>

struct nfsd_fhandle {
	struct svc_fh		fh;
};

struct nfsd_sattrargs {
	struct svc_fh		fh;
	struct iattr		attrs;
};

struct nfsd_diropargs {
	struct svc_fh		fh;
	char *			name;
	unsigned int		len;
};

struct nfsd_readargs {
	struct svc_fh		fh;
	__u32			offset;
	__u32			count;
	int			vlen;
};

struct nfsd_writeargs {
	svc_fh			fh;
	__u32			offset;
	int			len;
	int			vlen;
};

struct nfsd_createargs {
	struct svc_fh		fh;
	char *			name;
	unsigned int		len;
	struct iattr		attrs;
};

struct nfsd_renameargs {
	struct svc_fh		ffh;
	char *			fname;
	unsigned int		flen;
	struct svc_fh		tfh;
	char *			tname;
	unsigned int		tlen;
};

struct nfsd_readlinkargs {
	struct svc_fh		fh;
	char *			buffer;
};
	
struct nfsd_linkargs {
	struct svc_fh		ffh;
	struct svc_fh		tfh;
	char *			tname;
	unsigned int		tlen;
};

struct nfsd_symlinkargs {
	struct svc_fh		ffh;
	char *			fname;
	unsigned int		flen;
	char *			tname;
	unsigned int		tlen;
	struct iattr		attrs;
};

struct nfsd_readdirargs {
	struct svc_fh		fh;
	__u32			cookie;
	__u32			count;
	__be32 *		buffer;
};

struct nfsd_attrstat {
	struct svc_fh		fh;
	struct kstat		stat;
};

struct nfsd_diropres  {
	struct svc_fh		fh;
	struct kstat		stat;
};

struct nfsd_readlinkres {
	int			len;
};

struct nfsd_readres {
	struct svc_fh		fh;
	unsigned long		count;
	struct kstat		stat;
};

struct nfsd_readdirres {
	int			count;

	struct readdir_cd	common;
	__be32 *		buffer;
	int			buflen;
	__be32 *		offset;
};

struct nfsd_statfsres {
	struct kstatfs		stats;
};

/*
 * Storage requirements for XDR arguments and results.
 */
union nfsd_xdrstore {
	struct nfsd_sattrargs	sattr;
	struct nfsd_diropargs	dirop;
	struct nfsd_readargs	read;
	struct nfsd_writeargs	write;
	struct nfsd_createargs	create;
	struct nfsd_renameargs	rename;
	struct nfsd_linkargs	link;
	struct nfsd_symlinkargs	symlink;
	struct nfsd_readdirargs	readdir;
};

#define NFS2_SVC_XDRSIZE	sizeof(union nfsd_xdrstore)


int nfssvc_decode_void(struct svc_rqst *, __be32 *, void *);
int nfssvc_decode_fhandle(struct svc_rqst *, __be32 *, struct nfsd_fhandle *);
int nfssvc_decode_sattrargs(struct svc_rqst *, __be32 *,
				struct nfsd_sattrargs *);
int nfssvc_decode_diropargs(struct svc_rqst *, __be32 *,
				struct nfsd_diropargs *);
int nfssvc_decode_readargs(struct svc_rqst *, __be32 *,
				struct nfsd_readargs *);
int nfssvc_decode_writeargs(struct svc_rqst *, __be32 *,
				struct nfsd_writeargs *);
int nfssvc_decode_createargs(struct svc_rqst *, __be32 *,
				struct nfsd_createargs *);
int nfssvc_decode_renameargs(struct svc_rqst *, __be32 *,
				struct nfsd_renameargs *);
int nfssvc_decode_readlinkargs(struct svc_rqst *, __be32 *,
				struct nfsd_readlinkargs *);
int nfssvc_decode_linkargs(struct svc_rqst *, __be32 *,
				struct nfsd_linkargs *);
int nfssvc_decode_symlinkargs(struct svc_rqst *, __be32 *,
				struct nfsd_symlinkargs *);
int nfssvc_decode_readdirargs(struct svc_rqst *, __be32 *,
				struct nfsd_readdirargs *);
int nfssvc_encode_void(struct svc_rqst *, __be32 *, void *);
int nfssvc_encode_attrstat(struct svc_rqst *, __be32 *, struct nfsd_attrstat *);
int nfssvc_encode_diropres(struct svc_rqst *, __be32 *, struct nfsd_diropres *);
int nfssvc_encode_readlinkres(struct svc_rqst *, __be32 *, struct nfsd_readlinkres *);
int nfssvc_encode_readres(struct svc_rqst *, __be32 *, struct nfsd_readres *);
int nfssvc_encode_statfsres(struct svc_rqst *, __be32 *, struct nfsd_statfsres *);
int nfssvc_encode_readdirres(struct svc_rqst *, __be32 *, struct nfsd_readdirres *);

int nfssvc_encode_entry(void *, const char *name,
			int namlen, loff_t offset, u64 ino, unsigned int);

int nfssvc_release_fhandle(struct svc_rqst *, __be32 *, struct nfsd_fhandle *);

/* Helper functions for NFSv2 ACL code */
__be32 *nfs2svc_encode_fattr(struct svc_rqst *rqstp, __be32 *p, struct svc_fh *fhp);
__be32 *nfs2svc_decode_fh(__be32 *p, struct svc_fh *fhp);

#endif /* LINUX_NFSD_H */
