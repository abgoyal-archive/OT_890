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
 * linux/include/linux/sunrpc/auth.h
 *
 * Declarations for the RPC client authentication machinery.
 *
 * Copyright (C) 1996, Olaf Kirch <okir@monad.swb.de>
 */

#ifndef _LINUX_SUNRPC_AUTH_H
#define _LINUX_SUNRPC_AUTH_H

#ifdef __KERNEL__

#include <linux/sunrpc/sched.h>
#include <linux/sunrpc/msg_prot.h>
#include <linux/sunrpc/xdr.h>

#include <asm/atomic.h>
#include <linux/rcupdate.h>

/* size of the nodename buffer */
#define UNX_MAXNODENAME	32

/* Work around the lack of a VFS credential */
struct auth_cred {
	uid_t	uid;
	gid_t	gid;
	struct group_info *group_info;
	unsigned char machine_cred : 1;
};

/*
 * Client user credentials
 */
struct rpc_auth;
struct rpc_credops;
struct rpc_cred {
	struct hlist_node	cr_hash;	/* hash chain */
	struct list_head	cr_lru;		/* lru garbage collection */
	struct rcu_head		cr_rcu;
	struct rpc_auth *	cr_auth;
	const struct rpc_credops *cr_ops;
#ifdef RPC_DEBUG
	unsigned long		cr_magic;	/* 0x0f4aa4f0 */
#endif
	unsigned long		cr_expire;	/* when to gc */
	unsigned long		cr_flags;	/* various flags */
	atomic_t		cr_count;	/* ref count */

	uid_t			cr_uid;

	/* per-flavor data */
};
#define RPCAUTH_CRED_NEW	0
#define RPCAUTH_CRED_UPTODATE	1
#define RPCAUTH_CRED_HASHED	2

#define RPCAUTH_CRED_MAGIC	0x0f4aa4f0

/*
 * Client authentication handle
 */
#define RPC_CREDCACHE_HASHBITS	4
#define RPC_CREDCACHE_NR	(1 << RPC_CREDCACHE_HASHBITS)
struct rpc_cred_cache {
	struct hlist_head	hashtable[RPC_CREDCACHE_NR];
	spinlock_t		lock;
};

struct rpc_authops;
struct rpc_auth {
	unsigned int		au_cslack;	/* call cred size estimate */
				/* guess at number of u32's auth adds before
				 * reply data; normally the verifier size: */
	unsigned int		au_rslack;
				/* for gss, used to calculate au_rslack: */
	unsigned int		au_verfsize;

	unsigned int		au_flags;	/* various flags */
	const struct rpc_authops *au_ops;		/* operations */
	rpc_authflavor_t	au_flavor;	/* pseudoflavor (note may
						 * differ from the flavor in
						 * au_ops->au_flavor in gss
						 * case) */
	atomic_t		au_count;	/* Reference counter */

	struct rpc_cred_cache *	au_credcache;
	/* per-flavor data */
};

/* Flags for rpcauth_lookupcred() */
#define RPCAUTH_LOOKUP_NEW		0x01	/* Accept an uninitialised cred */

/*
 * Client authentication ops
 */
struct rpc_authops {
	struct module		*owner;
	rpc_authflavor_t	au_flavor;	/* flavor (RPC_AUTH_*) */
	char *			au_name;
	struct rpc_auth *	(*create)(struct rpc_clnt *, rpc_authflavor_t);
	void			(*destroy)(struct rpc_auth *);

	struct rpc_cred *	(*lookup_cred)(struct rpc_auth *, struct auth_cred *, int);
	struct rpc_cred *	(*crcreate)(struct rpc_auth*, struct auth_cred *, int);
};

struct rpc_credops {
	const char *		cr_name;	/* Name of the auth flavour */
	int			(*cr_init)(struct rpc_auth *, struct rpc_cred *);
	void			(*crdestroy)(struct rpc_cred *);

	int			(*crmatch)(struct auth_cred *, struct rpc_cred *, int);
	void			(*crbind)(struct rpc_task *, struct rpc_cred *);
	__be32 *		(*crmarshal)(struct rpc_task *, __be32 *);
	int			(*crrefresh)(struct rpc_task *);
	__be32 *		(*crvalidate)(struct rpc_task *, __be32 *);
	int			(*crwrap_req)(struct rpc_task *, kxdrproc_t,
						void *, __be32 *, void *);
	int			(*crunwrap_resp)(struct rpc_task *, kxdrproc_t,
						void *, __be32 *, void *);
};

extern const struct rpc_authops	authunix_ops;
extern const struct rpc_authops	authnull_ops;

void __init		rpc_init_authunix(void);
void __init		rpc_init_generic_auth(void);
void __init		rpcauth_init_module(void);
void __exit		rpcauth_remove_module(void);
void __exit		rpc_destroy_generic_auth(void);

struct rpc_cred *	rpc_lookup_cred(void);
struct rpc_cred *	rpc_lookup_machine_cred(void);
int			rpcauth_register(const struct rpc_authops *);
int			rpcauth_unregister(const struct rpc_authops *);
struct rpc_auth *	rpcauth_create(rpc_authflavor_t, struct rpc_clnt *);
void			rpcauth_release(struct rpc_auth *);
struct rpc_cred *	rpcauth_lookup_credcache(struct rpc_auth *, struct auth_cred *, int);
void			rpcauth_init_cred(struct rpc_cred *, const struct auth_cred *, struct rpc_auth *, const struct rpc_credops *);
struct rpc_cred *	rpcauth_lookupcred(struct rpc_auth *, int);
void			rpcauth_bindcred(struct rpc_task *, struct rpc_cred *, int);
void			rpcauth_generic_bind_cred(struct rpc_task *, struct rpc_cred *);
void			put_rpccred(struct rpc_cred *);
void			rpcauth_unbindcred(struct rpc_task *);
__be32 *		rpcauth_marshcred(struct rpc_task *, __be32 *);
__be32 *		rpcauth_checkverf(struct rpc_task *, __be32 *);
int			rpcauth_wrap_req(struct rpc_task *task, kxdrproc_t encode, void *rqstp, __be32 *data, void *obj);
int			rpcauth_unwrap_resp(struct rpc_task *task, kxdrproc_t decode, void *rqstp, __be32 *data, void *obj);
int			rpcauth_refreshcred(struct rpc_task *);
void			rpcauth_invalcred(struct rpc_task *);
int			rpcauth_uptodatecred(struct rpc_task *);
int			rpcauth_init_credcache(struct rpc_auth *);
void			rpcauth_destroy_credcache(struct rpc_auth *);
void			rpcauth_clear_credcache(struct rpc_cred_cache *);

static inline
struct rpc_cred *	get_rpccred(struct rpc_cred *cred)
{
	atomic_inc(&cred->cr_count);
	return cred;
}

#endif /* __KERNEL__ */
#endif /* _LINUX_SUNRPC_AUTH_H */
