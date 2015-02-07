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
 * Cryptographic API.
 *
 * Digest operations.
 *
 * Copyright (c) 2002 James Morris <jmorris@intercode.com.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version.
 *
 */

#include <crypto/internal/hash.h>
#include <crypto/scatterwalk.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/hardirq.h>
#include <linux/highmem.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/scatterlist.h>

#include "internal.h"

static int init(struct hash_desc *desc)
{
	struct crypto_tfm *tfm = crypto_hash_tfm(desc->tfm);

	tfm->__crt_alg->cra_digest.dia_init(tfm);
	return 0;
}

static int update2(struct hash_desc *desc,
		   struct scatterlist *sg, unsigned int nbytes)
{
	struct crypto_tfm *tfm = crypto_hash_tfm(desc->tfm);
	unsigned int alignmask = crypto_tfm_alg_alignmask(tfm);

	if (!nbytes)
		return 0;

	for (;;) {
		struct page *pg = sg_page(sg);
		unsigned int offset = sg->offset;
		unsigned int l = sg->length;

		if (unlikely(l > nbytes))
			l = nbytes;
		nbytes -= l;

		do {
			unsigned int bytes_from_page = min(l, ((unsigned int)
							   (PAGE_SIZE)) - 
							   offset);
			char *src = crypto_kmap(pg, 0);
			char *p = src + offset;

			if (unlikely(offset & alignmask)) {
				unsigned int bytes =
					alignmask + 1 - (offset & alignmask);
				bytes = min(bytes, bytes_from_page);
				tfm->__crt_alg->cra_digest.dia_update(tfm, p,
								      bytes);
				p += bytes;
				bytes_from_page -= bytes;
				l -= bytes;
			}
			tfm->__crt_alg->cra_digest.dia_update(tfm, p,
							      bytes_from_page);
			crypto_kunmap(src, 0);
			crypto_yield(desc->flags);
			offset = 0;
			pg++;
			l -= bytes_from_page;
		} while (l > 0);

		if (!nbytes)
			break;
		sg = scatterwalk_sg_next(sg);
	}

	return 0;
}

static int update(struct hash_desc *desc,
		  struct scatterlist *sg, unsigned int nbytes)
{
	if (WARN_ON_ONCE(in_irq()))
		return -EDEADLK;
	return update2(desc, sg, nbytes);
}

static int final(struct hash_desc *desc, u8 *out)
{
	struct crypto_tfm *tfm = crypto_hash_tfm(desc->tfm);
	unsigned long alignmask = crypto_tfm_alg_alignmask(tfm);
	struct digest_alg *digest = &tfm->__crt_alg->cra_digest;

	if (unlikely((unsigned long)out & alignmask)) {
		unsigned long align = alignmask + 1;
		unsigned long addr = (unsigned long)crypto_tfm_ctx(tfm);
		u8 *dst = (u8 *)ALIGN(addr, align) +
			  ALIGN(tfm->__crt_alg->cra_ctxsize, align);

		digest->dia_final(tfm, dst);
		memcpy(out, dst, digest->dia_digestsize);
	} else
		digest->dia_final(tfm, out);

	return 0;
}

static int nosetkey(struct crypto_hash *tfm, const u8 *key, unsigned int keylen)
{
	crypto_hash_clear_flags(tfm, CRYPTO_TFM_RES_MASK);
	return -ENOSYS;
}

static int setkey(struct crypto_hash *hash, const u8 *key, unsigned int keylen)
{
	struct crypto_tfm *tfm = crypto_hash_tfm(hash);

	crypto_hash_clear_flags(hash, CRYPTO_TFM_RES_MASK);
	return tfm->__crt_alg->cra_digest.dia_setkey(tfm, key, keylen);
}

static int digest(struct hash_desc *desc,
		  struct scatterlist *sg, unsigned int nbytes, u8 *out)
{
	if (WARN_ON_ONCE(in_irq()))
		return -EDEADLK;

	init(desc);
	update2(desc, sg, nbytes);
	return final(desc, out);
}

int crypto_init_digest_ops(struct crypto_tfm *tfm)
{
	struct hash_tfm *ops = &tfm->crt_hash;
	struct digest_alg *dalg = &tfm->__crt_alg->cra_digest;

	if (dalg->dia_digestsize > PAGE_SIZE / 8)
		return -EINVAL;
	
	ops->init	= init;
	ops->update	= update;
	ops->final	= final;
	ops->digest	= digest;
	ops->setkey	= dalg->dia_setkey ? setkey : nosetkey;
	ops->digestsize	= dalg->dia_digestsize;
	
	return 0;
}

void crypto_exit_digest_ops(struct crypto_tfm *tfm)
{
}

static int digest_async_nosetkey(struct crypto_ahash *tfm_async, const u8 *key,
			unsigned int keylen)
{
	crypto_ahash_clear_flags(tfm_async, CRYPTO_TFM_RES_MASK);
	return -ENOSYS;
}

static int digest_async_setkey(struct crypto_ahash *tfm_async, const u8 *key,
			unsigned int keylen)
{
	struct crypto_tfm    *tfm        = crypto_ahash_tfm(tfm_async);
	struct digest_alg    *dalg       = &tfm->__crt_alg->cra_digest;

	crypto_ahash_clear_flags(tfm_async, CRYPTO_TFM_RES_MASK);
	return dalg->dia_setkey(tfm, key, keylen);
}

static int digest_async_init(struct ahash_request *req)
{
	struct crypto_tfm *tfm  = req->base.tfm;
	struct digest_alg *dalg = &tfm->__crt_alg->cra_digest;

	dalg->dia_init(tfm);
	return 0;
}

static int digest_async_update(struct ahash_request *req)
{
	struct crypto_tfm *tfm = req->base.tfm;
	struct hash_desc  desc = {
		.tfm   = __crypto_hash_cast(tfm),
		.flags = req->base.flags,
	};

	update(&desc, req->src, req->nbytes);
	return 0;
}

static int digest_async_final(struct ahash_request *req)
{
	struct crypto_tfm *tfm  = req->base.tfm;
	struct hash_desc  desc = {
		.tfm   = __crypto_hash_cast(tfm),
		.flags = req->base.flags,
	};

	final(&desc, req->result);
	return 0;
}

static int digest_async_digest(struct ahash_request *req)
{
	struct crypto_tfm *tfm  = req->base.tfm;
	struct hash_desc  desc = {
		.tfm   = __crypto_hash_cast(tfm),
		.flags = req->base.flags,
	};

	return digest(&desc, req->src, req->nbytes, req->result);
}

int crypto_init_digest_ops_async(struct crypto_tfm *tfm)
{
	struct ahash_tfm  *crt  = &tfm->crt_ahash;
	struct digest_alg *dalg = &tfm->__crt_alg->cra_digest;

	if (dalg->dia_digestsize > PAGE_SIZE / 8)
		return -EINVAL;

	crt->init       = digest_async_init;
	crt->update     = digest_async_update;
	crt->final      = digest_async_final;
	crt->digest     = digest_async_digest;
	crt->setkey     = dalg->dia_setkey ? digest_async_setkey :
						digest_async_nosetkey;
	crt->digestsize = dalg->dia_digestsize;

	return 0;
}
