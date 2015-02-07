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

#ifndef _CRYPTO_INTERNAL_SKCIPHER_H
#define _CRYPTO_INTERNAL_SKCIPHER_H

#include <crypto/algapi.h>
#include <crypto/skcipher.h>
#include <linux/types.h>

struct rtattr;

struct crypto_skcipher_spawn {
	struct crypto_spawn base;
};

extern const struct crypto_type crypto_givcipher_type;

static inline void crypto_set_skcipher_spawn(
	struct crypto_skcipher_spawn *spawn, struct crypto_instance *inst)
{
	crypto_set_spawn(&spawn->base, inst);
}

int crypto_grab_skcipher(struct crypto_skcipher_spawn *spawn, const char *name,
			 u32 type, u32 mask);

static inline void crypto_drop_skcipher(struct crypto_skcipher_spawn *spawn)
{
	crypto_drop_spawn(&spawn->base);
}

static inline struct crypto_alg *crypto_skcipher_spawn_alg(
	struct crypto_skcipher_spawn *spawn)
{
	return spawn->base.alg;
}

static inline struct crypto_ablkcipher *crypto_spawn_skcipher(
	struct crypto_skcipher_spawn *spawn)
{
	return __crypto_ablkcipher_cast(
		crypto_spawn_tfm(&spawn->base, crypto_skcipher_type(0),
				 crypto_skcipher_mask(0)));
}

int skcipher_null_givencrypt(struct skcipher_givcrypt_request *req);
int skcipher_null_givdecrypt(struct skcipher_givcrypt_request *req);
const char *crypto_default_geniv(const struct crypto_alg *alg);

struct crypto_instance *skcipher_geniv_alloc(struct crypto_template *tmpl,
					     struct rtattr **tb, u32 type,
					     u32 mask);
void skcipher_geniv_free(struct crypto_instance *inst);
int skcipher_geniv_init(struct crypto_tfm *tfm);
void skcipher_geniv_exit(struct crypto_tfm *tfm);

static inline struct crypto_ablkcipher *skcipher_geniv_cipher(
	struct crypto_ablkcipher *geniv)
{
	return crypto_ablkcipher_crt(geniv)->base;
}

static inline int skcipher_enqueue_givcrypt(
	struct crypto_queue *queue, struct skcipher_givcrypt_request *request)
{
	return ablkcipher_enqueue_request(queue, &request->creq);
}

static inline struct skcipher_givcrypt_request *skcipher_dequeue_givcrypt(
	struct crypto_queue *queue)
{
	return container_of(ablkcipher_dequeue_request(queue),
			    struct skcipher_givcrypt_request, creq);
}

static inline void *skcipher_givcrypt_reqctx(
	struct skcipher_givcrypt_request *req)
{
	return ablkcipher_request_ctx(&req->creq);
}

static inline void ablkcipher_request_complete(struct ablkcipher_request *req,
					       int err)
{
	req->base.complete(&req->base, err);
}

static inline void skcipher_givcrypt_complete(
	struct skcipher_givcrypt_request *req, int err)
{
	ablkcipher_request_complete(&req->creq, err);
}

static inline u32 ablkcipher_request_flags(struct ablkcipher_request *req)
{
	return req->base.flags;
}

#endif	/* _CRYPTO_INTERNAL_SKCIPHER_H */

