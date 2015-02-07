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
 * Symmetric key ciphers.
 * 
 * Copyright (c) 2007 Herbert Xu <herbert@gondor.apana.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version.
 *
 */

#ifndef _CRYPTO_SKCIPHER_H
#define _CRYPTO_SKCIPHER_H

#include <linux/crypto.h>
#include <linux/kernel.h>
#include <linux/slab.h>

/**
 *	struct skcipher_givcrypt_request - Crypto request with IV generation
 *	@seq: Sequence number for IV generation
 *	@giv: Space for generated IV
 *	@creq: The crypto request itself
 */
struct skcipher_givcrypt_request {
	u64 seq;
	u8 *giv;

	struct ablkcipher_request creq;
};

static inline struct crypto_ablkcipher *skcipher_givcrypt_reqtfm(
	struct skcipher_givcrypt_request *req)
{
	return crypto_ablkcipher_reqtfm(&req->creq);
}

static inline int crypto_skcipher_givencrypt(
	struct skcipher_givcrypt_request *req)
{
	struct ablkcipher_tfm *crt =
		crypto_ablkcipher_crt(skcipher_givcrypt_reqtfm(req));
	return crt->givencrypt(req);
};

static inline int crypto_skcipher_givdecrypt(
	struct skcipher_givcrypt_request *req)
{
	struct ablkcipher_tfm *crt =
		crypto_ablkcipher_crt(skcipher_givcrypt_reqtfm(req));
	return crt->givdecrypt(req);
};

static inline void skcipher_givcrypt_set_tfm(
	struct skcipher_givcrypt_request *req, struct crypto_ablkcipher *tfm)
{
	req->creq.base.tfm = crypto_ablkcipher_tfm(tfm);
}

static inline struct skcipher_givcrypt_request *skcipher_givcrypt_cast(
	struct crypto_async_request *req)
{
	return container_of(ablkcipher_request_cast(req),
			    struct skcipher_givcrypt_request, creq);
}

static inline struct skcipher_givcrypt_request *skcipher_givcrypt_alloc(
	struct crypto_ablkcipher *tfm, gfp_t gfp)
{
	struct skcipher_givcrypt_request *req;

	req = kmalloc(sizeof(struct skcipher_givcrypt_request) +
		      crypto_ablkcipher_reqsize(tfm), gfp);

	if (likely(req))
		skcipher_givcrypt_set_tfm(req, tfm);

	return req;
}

static inline void skcipher_givcrypt_free(struct skcipher_givcrypt_request *req)
{
	kfree(req);
}

static inline void skcipher_givcrypt_set_callback(
	struct skcipher_givcrypt_request *req, u32 flags,
	crypto_completion_t complete, void *data)
{
	ablkcipher_request_set_callback(&req->creq, flags, complete, data);
}

static inline void skcipher_givcrypt_set_crypt(
	struct skcipher_givcrypt_request *req,
	struct scatterlist *src, struct scatterlist *dst,
	unsigned int nbytes, void *iv)
{
	ablkcipher_request_set_crypt(&req->creq, src, dst, nbytes, iv);
}

static inline void skcipher_givcrypt_set_giv(
	struct skcipher_givcrypt_request *req, u8 *giv, u64 seq)
{
	req->giv = giv;
	req->seq = seq;
}

#endif	/* _CRYPTO_SKCIPHER_H */

