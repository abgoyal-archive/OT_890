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
 * s390 implementation of the SHA1 Secure Hash Algorithm.
 *
 * Derived from cryptoapi implementation, adapted for in-place
 * scatterlist interface.  Originally based on the public domain
 * implementation written by Steve Reid.
 *
 * s390 Version:
 *   Copyright IBM Corp. 2003,2007
 *   Author(s): Thomas Spatzier
 *		Jan Glauber (jan.glauber@de.ibm.com)
 *
 * Derived from "crypto/sha1_generic.c"
 *   Copyright (c) Alan Smithee.
 *   Copyright (c) Andrew McDonald <andrew@mcdonald.org.uk>
 *   Copyright (c) Jean-Francois Dive <jef@linuxbe.org>
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

static void sha1_init(struct crypto_tfm *tfm)
{
	struct s390_sha_ctx *sctx = crypto_tfm_ctx(tfm);

	sctx->state[0] = SHA1_H0;
	sctx->state[1] = SHA1_H1;
	sctx->state[2] = SHA1_H2;
	sctx->state[3] = SHA1_H3;
	sctx->state[4] = SHA1_H4;
	sctx->count = 0;
	sctx->func = KIMD_SHA_1;
}

static struct crypto_alg alg = {
	.cra_name	=	"sha1",
	.cra_driver_name=	"sha1-s390",
	.cra_priority	=	CRYPT_S390_PRIORITY,
	.cra_flags	=	CRYPTO_ALG_TYPE_DIGEST,
	.cra_blocksize	=	SHA1_BLOCK_SIZE,
	.cra_ctxsize	=	sizeof(struct s390_sha_ctx),
	.cra_module	=	THIS_MODULE,
	.cra_list	=	LIST_HEAD_INIT(alg.cra_list),
	.cra_u		=	{ .digest = {
	.dia_digestsize	=	SHA1_DIGEST_SIZE,
	.dia_init	=	sha1_init,
	.dia_update	=	s390_sha_update,
	.dia_final	=	s390_sha_final } }
};

static int __init sha1_s390_init(void)
{
	if (!crypt_s390_func_available(KIMD_SHA_1))
		return -EOPNOTSUPP;
	return crypto_register_alg(&alg);
}

static void __exit sha1_s390_fini(void)
{
	crypto_unregister_alg(&alg);
}

module_init(sha1_s390_init);
module_exit(sha1_s390_fini);

MODULE_ALIAS("sha1");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SHA1 Secure Hash Algorithm");
