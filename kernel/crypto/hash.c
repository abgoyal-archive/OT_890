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
 * Cryptographic Hash operations.
 * 
 * Copyright (c) 2006 Herbert Xu <herbert@gondor.apana.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version.
 */

#include <crypto/internal/hash.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/seq_file.h>

#include "internal.h"

static unsigned int crypto_hash_ctxsize(struct crypto_alg *alg, u32 type,
					u32 mask)
{
	return alg->cra_ctxsize;
}

static int hash_setkey_unaligned(struct crypto_hash *crt, const u8 *key,
		                 unsigned int keylen)
{
	struct crypto_tfm *tfm = crypto_hash_tfm(crt);
	struct hash_alg *alg = &tfm->__crt_alg->cra_hash;
	unsigned long alignmask = crypto_hash_alignmask(crt);
	int ret;
	u8 *buffer, *alignbuffer;
	unsigned long absize;

	absize = keylen + alignmask;
	buffer = kmalloc(absize, GFP_ATOMIC);
	if (!buffer)
		return -ENOMEM;

	alignbuffer = (u8 *)ALIGN((unsigned long)buffer, alignmask + 1);
	memcpy(alignbuffer, key, keylen);
	ret = alg->setkey(crt, alignbuffer, keylen);
	memset(alignbuffer, 0, keylen);
	kfree(buffer);
	return ret;
}

static int hash_setkey(struct crypto_hash *crt, const u8 *key,
		       unsigned int keylen)
{
	struct crypto_tfm *tfm = crypto_hash_tfm(crt);
	struct hash_alg *alg = &tfm->__crt_alg->cra_hash;
	unsigned long alignmask = crypto_hash_alignmask(crt);

	if ((unsigned long)key & alignmask)
		return hash_setkey_unaligned(crt, key, keylen);

	return alg->setkey(crt, key, keylen);
}

static int hash_async_setkey(struct crypto_ahash *tfm_async, const u8 *key,
			unsigned int keylen)
{
	struct crypto_tfm  *tfm      = crypto_ahash_tfm(tfm_async);
	struct crypto_hash *tfm_hash = __crypto_hash_cast(tfm);
	struct hash_alg    *alg      = &tfm->__crt_alg->cra_hash;

	return alg->setkey(tfm_hash, key, keylen);
}

static int hash_async_init(struct ahash_request *req)
{
	struct crypto_tfm *tfm = req->base.tfm;
	struct hash_alg   *alg = &tfm->__crt_alg->cra_hash;
	struct hash_desc  desc = {
		.tfm = __crypto_hash_cast(tfm),
		.flags = req->base.flags,
	};

	return alg->init(&desc);
}

static int hash_async_update(struct ahash_request *req)
{
	struct crypto_tfm *tfm = req->base.tfm;
	struct hash_alg   *alg = &tfm->__crt_alg->cra_hash;
	struct hash_desc  desc = {
		.tfm = __crypto_hash_cast(tfm),
		.flags = req->base.flags,
	};

	return alg->update(&desc, req->src, req->nbytes);
}

static int hash_async_final(struct ahash_request *req)
{
	struct crypto_tfm *tfm = req->base.tfm;
	struct hash_alg   *alg = &tfm->__crt_alg->cra_hash;
	struct hash_desc  desc = {
		.tfm = __crypto_hash_cast(tfm),
		.flags = req->base.flags,
	};

	return alg->final(&desc, req->result);
}

static int hash_async_digest(struct ahash_request *req)
{
	struct crypto_tfm *tfm = req->base.tfm;
	struct hash_alg   *alg = &tfm->__crt_alg->cra_hash;
	struct hash_desc  desc = {
		.tfm = __crypto_hash_cast(tfm),
		.flags = req->base.flags,
	};

	return alg->digest(&desc, req->src, req->nbytes, req->result);
}

static int crypto_init_hash_ops_async(struct crypto_tfm *tfm)
{
	struct ahash_tfm *crt = &tfm->crt_ahash;
	struct hash_alg  *alg = &tfm->__crt_alg->cra_hash;

	crt->init       = hash_async_init;
	crt->update     = hash_async_update;
	crt->final      = hash_async_final;
	crt->digest     = hash_async_digest;
	crt->setkey     = hash_async_setkey;
	crt->digestsize = alg->digestsize;

	return 0;
}

static int crypto_init_hash_ops_sync(struct crypto_tfm *tfm)
{
	struct hash_tfm *crt = &tfm->crt_hash;
	struct hash_alg *alg = &tfm->__crt_alg->cra_hash;

	crt->init       = alg->init;
	crt->update     = alg->update;
	crt->final      = alg->final;
	crt->digest     = alg->digest;
	crt->setkey     = hash_setkey;
	crt->digestsize = alg->digestsize;

	return 0;
}

static int crypto_init_hash_ops(struct crypto_tfm *tfm, u32 type, u32 mask)
{
	struct hash_alg *alg = &tfm->__crt_alg->cra_hash;

	if (alg->digestsize > PAGE_SIZE / 8)
		return -EINVAL;

	if ((mask & CRYPTO_ALG_TYPE_HASH_MASK) != CRYPTO_ALG_TYPE_HASH_MASK)
		return crypto_init_hash_ops_async(tfm);
	else
		return crypto_init_hash_ops_sync(tfm);
}

static void crypto_hash_show(struct seq_file *m, struct crypto_alg *alg)
	__attribute__ ((unused));
static void crypto_hash_show(struct seq_file *m, struct crypto_alg *alg)
{
	seq_printf(m, "type         : hash\n");
	seq_printf(m, "blocksize    : %u\n", alg->cra_blocksize);
	seq_printf(m, "digestsize   : %u\n", alg->cra_hash.digestsize);
}

const struct crypto_type crypto_hash_type = {
	.ctxsize = crypto_hash_ctxsize,
	.init = crypto_init_hash_ops,
#ifdef CONFIG_PROC_FS
	.show = crypto_hash_show,
#endif
};
EXPORT_SYMBOL_GPL(crypto_hash_type);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Generic cryptographic hash type");
