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
 * s390 implementation of the SHA256 Secure Hash Algorithm.
 *
 * s390 Version:
 *   Copyright IBM Corp. 2005,2007
 *   Author(s): Jan Glauber (jang@de.ibm.com)
 *
 * Derived from "crypto/sha256_generic.c"
 * and "arch/s390/crypto/sha1_s390.c"
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
#include <crypto/sha.h>

#include "crypt_s390.h"
#include "sha.h"

static void sha256_init(struct crypto_tfm *tfm)
{
	struct s390_sha_ctx *sctx = crypto_tfm_ctx(tfm);

	sctx->state[0] = SHA256_H0;
	sctx->state[1] = SHA256_H1;
	sctx->state[2] = SHA256_H2;
	sctx->state[3] = SHA256_H3;
	sctx->state[4] = SHA256_H4;
	sctx->state[5] = SHA256_H5;
	sctx->state[6] = SHA256_H6;
	sctx->state[7] = SHA256_H7;
	sctx->count = 0;
	sctx->func = KIMD_SHA_256;
}

static struct crypto_alg alg = {
	.cra_name	=	"sha256",
	.cra_driver_name =	"sha256-s390",
	.cra_priority	=	CRYPT_S390_PRIORITY,
	.cra_flags	=	CRYPTO_ALG_TYPE_DIGEST,
	.cra_blocksize	=	SHA256_BLOCK_SIZE,
	.cra_ctxsize	=	sizeof(struct s390_sha_ctx),
	.cra_module	=	THIS_MODULE,
	.cra_list	=	LIST_HEAD_INIT(alg.cra_list),
	.cra_u		=	{ .digest = {
	.dia_digestsize	=	SHA256_DIGEST_SIZE,
	.dia_init	=	sha256_init,
	.dia_update	=	s390_sha_update,
	.dia_final	=	s390_sha_final } }
};

static int sha256_s390_init(void)
{
	if (!crypt_s390_func_available(KIMD_SHA_256))
		return -EOPNOTSUPP;

	return crypto_register_alg(&alg);
}

static void __exit sha256_s390_fini(void)
{
	crypto_unregister_alg(&alg);
}

module_init(sha256_s390_init);
module_exit(sha256_s390_fini);

MODULE_ALIAS("sha256");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SHA256 Secure Hash Algorithm");
