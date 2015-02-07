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
 * s390 implementation of the SHA512 and SHA38 Secure Hash Algorithm.
 *
 * Copyright IBM Corp. 2007
 * Author(s): Jan Glauber (jang@de.ibm.com)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/crypto.h>

#include "sha.h"
#include "crypt_s390.h"

static void sha512_init(struct crypto_tfm *tfm)
{
	struct s390_sha_ctx *ctx = crypto_tfm_ctx(tfm);

	*(__u64 *)&ctx->state[0] = 0x6a09e667f3bcc908ULL;
	*(__u64 *)&ctx->state[2] = 0xbb67ae8584caa73bULL;
	*(__u64 *)&ctx->state[4] = 0x3c6ef372fe94f82bULL;
	*(__u64 *)&ctx->state[6] = 0xa54ff53a5f1d36f1ULL;
	*(__u64 *)&ctx->state[8] = 0x510e527fade682d1ULL;
	*(__u64 *)&ctx->state[10] = 0x9b05688c2b3e6c1fULL;
	*(__u64 *)&ctx->state[12] = 0x1f83d9abfb41bd6bULL;
	*(__u64 *)&ctx->state[14] = 0x5be0cd19137e2179ULL;
	ctx->count = 0;
	ctx->func = KIMD_SHA_512;
}

static struct crypto_alg sha512_alg = {
	.cra_name	=	"sha512",
	.cra_driver_name =	"sha512-s390",
	.cra_priority	=	CRYPT_S390_PRIORITY,
	.cra_flags	=	CRYPTO_ALG_TYPE_DIGEST,
	.cra_blocksize	=	SHA512_BLOCK_SIZE,
	.cra_ctxsize	=	sizeof(struct s390_sha_ctx),
	.cra_module	=	THIS_MODULE,
	.cra_list	=	LIST_HEAD_INIT(sha512_alg.cra_list),
	.cra_u		=	{ .digest = {
	.dia_digestsize	=	SHA512_DIGEST_SIZE,
	.dia_init	=	sha512_init,
	.dia_update	=	s390_sha_update,
	.dia_final	=	s390_sha_final } }
};

MODULE_ALIAS("sha512");

static void sha384_init(struct crypto_tfm *tfm)
{
	struct s390_sha_ctx *ctx = crypto_tfm_ctx(tfm);

	*(__u64 *)&ctx->state[0] = 0xcbbb9d5dc1059ed8ULL;
	*(__u64 *)&ctx->state[2] = 0x629a292a367cd507ULL;
	*(__u64 *)&ctx->state[4] = 0x9159015a3070dd17ULL;
	*(__u64 *)&ctx->state[6] = 0x152fecd8f70e5939ULL;
	*(__u64 *)&ctx->state[8] = 0x67332667ffc00b31ULL;
	*(__u64 *)&ctx->state[10] = 0x8eb44a8768581511ULL;
	*(__u64 *)&ctx->state[12] = 0xdb0c2e0d64f98fa7ULL;
	*(__u64 *)&ctx->state[14] = 0x47b5481dbefa4fa4ULL;
	ctx->count = 0;
	ctx->func = KIMD_SHA_512;
}

static struct crypto_alg sha384_alg = {
	.cra_name	=	"sha384",
	.cra_driver_name =	"sha384-s390",
	.cra_priority	=	CRYPT_S390_PRIORITY,
	.cra_flags	=	CRYPTO_ALG_TYPE_DIGEST,
	.cra_blocksize	=	SHA384_BLOCK_SIZE,
	.cra_ctxsize	=	sizeof(struct s390_sha_ctx),
	.cra_module	=	THIS_MODULE,
	.cra_list	=	LIST_HEAD_INIT(sha384_alg.cra_list),
	.cra_u		=	{ .digest = {
	.dia_digestsize	=	SHA384_DIGEST_SIZE,
	.dia_init	=	sha384_init,
	.dia_update	=	s390_sha_update,
	.dia_final	=	s390_sha_final } }
};

MODULE_ALIAS("sha384");

static int __init init(void)
{
	int ret;

	if (!crypt_s390_func_available(KIMD_SHA_512))
		return -EOPNOTSUPP;
	if ((ret = crypto_register_alg(&sha512_alg)) < 0)
		goto out;
	if ((ret = crypto_register_alg(&sha384_alg)) < 0)
		crypto_unregister_alg(&sha512_alg);
out:
	return ret;
}

static void __exit fini(void)
{
	crypto_unregister_alg(&sha512_alg);
	crypto_unregister_alg(&sha384_alg);
}

module_init(init);
module_exit(fini);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SHA512 and SHA-384 Secure Hash Algorithm");
